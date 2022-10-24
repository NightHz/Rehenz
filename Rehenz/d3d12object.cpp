#include "d3d12object.h"

namespace Rehenz
{
	D3d12RenderTarget::D3d12RenderTarget(UINT _width, UINT _height, DXGI_FORMAT _format, bool _msaa, UINT16 _target_count, bool _have_zbuffer,
		Color _target_bgc, UINT _target_rtv, UINT _zbuffer_dsv, D3d12Device* device)
		: width(_width), height(_height), format(_format), msaa(_msaa), multi_target(target_count > 1), target_count(_target_count), have_zbuffer(_have_zbuffer),
		target_bgc(_target_bgc), target_rtv(_target_rtv), zbuffer_dsv(_zbuffer_dsv)
	{
		// create target
		auto rc_desc = D3d12Util::GetTexture2dRcDesc(width, height, 1, format, target_count, msaa, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		auto clear_value = D3d12Util::GetTexture2dClearStruct(format, target_bgc.v);
		target = std::make_shared<D3d12Texture>(rc_desc, D3D12_HEAP_TYPE_DEFAULT, device->Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, &clear_value);
		if (!target->Get())
		{
			target = nullptr;
			return;
		}
		device->Get()->CreateRenderTargetView(target->Get(), nullptr, device->GetRtv(target_rtv));

		// create zbuffer
		if (have_zbuffer)
		{
			rc_desc = D3d12Util::GetTexture2dRcDesc(width, height, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, target_count, msaa, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
			clear_value = D3d12Util::GetDepthStencilClearStruct(DXGI_FORMAT_D24_UNORM_S8_UINT, 1, 0);
			zbuffer = std::make_shared<D3d12Texture>(rc_desc, D3D12_HEAP_TYPE_DEFAULT, device->Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear_value);
			if (!zbuffer->Get())
			{
				target = nullptr;
				return;
			}
			device->Get()->CreateDepthStencilView(zbuffer->Get(), nullptr, device->GetDsv(zbuffer_dsv));
		}
		else
			zbuffer = nullptr;

		// set viewport
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = static_cast<float>(width);
		vp.Height = static_cast<float>(height);
		vp.MinDepth = 0;
		vp.MaxDepth = 1;

		// set scissor rect
		sr.left = 0;
		sr.right = width;
		sr.top = 0;
		sr.bottom = height;
	}

	D3d12RenderTarget::~D3d12RenderTarget()
	{
	}

	void D3d12RenderTarget::ClearRenderTargets(D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list)
	{
		cmd_list->ClearRenderTargetView(device->GetRtv(target_rtv), target_bgc.v, 0, nullptr);
		if (have_zbuffer)
			cmd_list->ClearDepthStencilView(device->GetDsv(zbuffer_dsv), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	}

	void D3d12RenderTarget::SetRenderTargets(D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list)
	{
		auto rtv = device->GetRtv(target_rtv);
		auto dsv = device->GetDsv(zbuffer_dsv);
		if (have_zbuffer)
			cmd_list->OMSetRenderTargets(1, &rtv, true, &dsv);
		else
			cmd_list->OMSetRenderTargets(1, &rtv, true, nullptr);
	}

	void D3d12RenderTarget::SetRS(ID3D12GraphicsCommandList6* cmd_list)
	{
		cmd_list->RSSetViewports(1, &vp);
		cmd_list->RSSetScissorRects(1, &sr);
	}

	void D3d12RenderTarget::CopyTarget(UINT target_i, ID3D12Resource2* dst, UINT dst_subrc, D3D12_RESOURCE_STATES dst_start,
		D3D12_RESOURCE_STATES dst_end, ID3D12GraphicsCommandList6* cmd_list)
	{
		if (!msaa)
		{
			D3D12_RESOURCE_BARRIER rc_barr1[]{
				D3d12Util::GetTransitionStruct(target->Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE),
				D3d12Util::GetTransitionStruct(dst,
				dst_start, D3D12_RESOURCE_STATE_COPY_DEST)
			};
			cmd_list->ResourceBarrier(_countof(rc_barr1), rc_barr1);
			D3D12_TEXTURE_COPY_LOCATION dst_loc{}, src_loc{};
			dst_loc.pResource = dst;
			dst_loc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			dst_loc.SubresourceIndex = dst_subrc;
			src_loc.pResource = target->Get();
			src_loc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			src_loc.SubresourceIndex = target_i;
			cmd_list->CopyTextureRegion(&dst_loc, 0, 0, 0, &src_loc, nullptr);
			D3D12_RESOURCE_BARRIER rc_barr2[]{
				D3d12Util::GetTransitionStruct(target->Get(),
				D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
				D3d12Util::GetTransitionStruct(dst,
				D3D12_RESOURCE_STATE_COPY_DEST, dst_end)
			};
			cmd_list->ResourceBarrier(_countof(rc_barr2), rc_barr2);
		}
		else
		{
			D3D12_RESOURCE_BARRIER rc_barr1[]{
				D3d12Util::GetTransitionStruct(target->Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_RESOLVE_SOURCE),
				D3d12Util::GetTransitionStruct(dst,
				dst_start, D3D12_RESOURCE_STATE_RESOLVE_DEST)
			};
			cmd_list->ResourceBarrier(_countof(rc_barr1), rc_barr1);
			cmd_list->ResolveSubresource(dst, dst_subrc, target->Get(), target_i, format);
			D3D12_RESOURCE_BARRIER rc_barr2[]{
				D3d12Util::GetTransitionStruct(target->Get(),
				D3D12_RESOURCE_STATE_RESOLVE_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
				D3d12Util::GetTransitionStruct(dst,
				D3D12_RESOURCE_STATE_RESOLVE_DEST, dst_end)
			};
			cmd_list->ResourceBarrier(_countof(rc_barr2), rc_barr2);
		}
	}

	D3d12Mesh::D3d12Mesh(void* vertices, UINT _vertex_size, UINT _vertex_count, const std::vector<D3D12_INPUT_ELEMENT_DESC>* _input_layout, UINT16* indices, UINT _index_count,
		D3D_PRIMITIVE_TOPOLOGY _topology, D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list)
		: vertex_size(_vertex_size), vertex_count(_vertex_count), vb_size(_vertex_size* _vertex_count),
		input_layout(_input_layout ? *_input_layout : std::vector<D3D12_INPUT_ELEMENT_DESC>{}),
		have_index_buffer(indices ? true : false), index_size(sizeof(UINT16)), index_count(_index_count), ib_size(_index_count * sizeof(UINT16)), topology(_topology)
	{
		HRESULT hr = S_OK;
		void* data = nullptr;
		D3D12_RESOURCE_BARRIER rc_barr{};

		// create vb
		vb_upload = std::make_shared<D3d12Buffer>(1, vb_size, D3D12_HEAP_TYPE_UPLOAD, device->Get());
		if (!vb_upload->Get())
		{
			vb = nullptr;
			return;
		}
		hr = vb_upload->Get()->Map(0, nullptr, &data);
		if (FAILED(hr))
		{
			vb = nullptr;
			return;
		}
		::memcpy(data, vertices, vb_size);
		vb_upload->Get()->Unmap(0, nullptr);
		vb = std::make_shared<D3d12Buffer>(vertex_count, vertex_size, D3D12_HEAP_TYPE_DEFAULT, device->Get(), D3D12_RESOURCE_STATE_COPY_DEST);
		if (!vb->Get())
		{
			vb = nullptr;
			return;
		}
		cmd_list->CopyBufferRegion(vb->Get(), 0, vb_upload->Get(), 0, vb_size);
		rc_barr = D3d12Util::GetTransitionStruct(vb->Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		cmd_list->ResourceBarrier(1, &rc_barr);

		if (have_index_buffer)
		{
			// create ib
			ib_upload = std::make_shared<D3d12Buffer>(1, ib_size, D3D12_HEAP_TYPE_UPLOAD, device->Get());
			if (!ib_upload->Get())
			{
				vb = nullptr;
				return;
			}
			hr = ib_upload->Get()->Map(0, nullptr, &data);
			if (FAILED(hr))
			{
				vb = nullptr;
				return;
			}
			::memcpy(data, indices, ib_size);
			ib_upload->Get()->Unmap(0, nullptr);
			ib = std::make_shared<D3d12Buffer>(index_count, index_size, D3D12_HEAP_TYPE_DEFAULT, device->Get(), D3D12_RESOURCE_STATE_COPY_DEST);
			if (!ib->Get())
			{
				vb = nullptr;
				return;
			}
			cmd_list->CopyBufferRegion(ib->Get(), 0, ib_upload->Get(), 0, ib_size);
			rc_barr = D3d12Util::GetTransitionStruct(ib->Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
			cmd_list->ResourceBarrier(1, &rc_barr);
		}
	}

	D3d12Mesh::D3d12Mesh(Mesh* mesh, D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list)
		: vertex_size(D3d12Util::GetRehenzMeshStructSize().first), vertex_count(static_cast<UINT>(mesh->VertexCount())), vb_size(vertex_size* vertex_count),
		input_layout(D3d12Util::GetRehenzMeshInputLayout()), have_index_buffer(true),
		index_size(D3d12Util::GetRehenzMeshStructSize().second), index_count(static_cast<UINT>(mesh->IndexCount())), ib_size(index_size* index_count),
		topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		HRESULT hr = S_OK;
		void* data = nullptr;
		D3D12_RESOURCE_BARRIER rc_barr{};

		// get mesh buffer
		auto mesh1 = D3d12Util::GetMeshBufferFromRehenzMesh(mesh);
		if (!mesh1.first)
		{
			vb = nullptr;
			return;
		}

		// create vb
		vb_upload = std::make_shared<D3d12Buffer>(1, vb_size, D3D12_HEAP_TYPE_UPLOAD, device->Get());
		if (!vb_upload->Get())
		{
			vb = nullptr;
			return;
		}
		hr = vb_upload->Get()->Map(0, nullptr, &data);
		if (FAILED(hr))
		{
			vb = nullptr;
			return;
		}
		::memcpy(data, mesh1.first->GetBufferPointer(), vb_size);
		vb_upload->Get()->Unmap(0, nullptr);
		vb = std::make_shared<D3d12Buffer>(vertex_count, vertex_size, D3D12_HEAP_TYPE_DEFAULT, device->Get(), D3D12_RESOURCE_STATE_COPY_DEST);
		if (!vb->Get())
		{
			vb = nullptr;
			return;
		}
		cmd_list->CopyBufferRegion(vb->Get(), 0, vb_upload->Get(), 0, vb_size);
		rc_barr = D3d12Util::GetTransitionStruct(vb->Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		cmd_list->ResourceBarrier(1, &rc_barr);

		if (have_index_buffer)
		{
			// create ib
			ib_upload = std::make_shared<D3d12Buffer>(1, ib_size, D3D12_HEAP_TYPE_UPLOAD, device->Get());
			if (!ib_upload->Get())
			{
				vb = nullptr;
				return;
			}
			hr = ib_upload->Get()->Map(0, nullptr, &data);
			if (FAILED(hr))
			{
				vb = nullptr;
				return;
			}
			::memcpy(data, mesh1.second->GetBufferPointer(), ib_size);
			ib_upload->Get()->Unmap(0, nullptr);
			ib = std::make_shared<D3d12Buffer>(index_count, index_size, D3D12_HEAP_TYPE_DEFAULT, device->Get(), D3D12_RESOURCE_STATE_COPY_DEST);
			if (!ib->Get())
			{
				vb = nullptr;
				return;
			}
			cmd_list->CopyBufferRegion(ib->Get(), 0, ib_upload->Get(), 0, ib_size);
			rc_barr = D3d12Util::GetTransitionStruct(ib->Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
			cmd_list->ResourceBarrier(1, &rc_barr);
		}
	}

	D3d12Mesh::~D3d12Mesh()
	{
	}

	void D3d12Mesh::CleanUp()
	{
		vb_upload = nullptr;
		ib_upload = nullptr;
	}

	void D3d12Mesh::SetIA(ID3D12GraphicsCommandList6* cmd_list)
	{
		auto vbv = vb->GetVbv();
		cmd_list->IASetVertexBuffers(0, 1, &vbv);
		cmd_list->IASetPrimitiveTopology(topology);
		if (have_index_buffer)
		{
			auto ibv = ib->GetIbv();
			cmd_list->IASetIndexBuffer(&ibv);
		}
		else
			cmd_list->IASetIndexBuffer(nullptr);
	}

	D3d12UploadBufferBase::D3d12UploadBufferBase(UINT _struct_size, bool _as_cbuffer, UINT _struct_count, D3d12Device* device)
		: struct_size(_struct_size), as_cbuffer(_as_cbuffer), struct_align_size(_as_cbuffer ? D3d12Util::Align256(_struct_size) : _struct_size),
		struct_count(_struct_count), buffer_size(struct_align_size* struct_count)
	{
		// create buffer
		buffer = std::make_shared<D3d12Buffer>(struct_count, struct_align_size, D3D12_HEAP_TYPE_UPLOAD, device->Get());
		if (!buffer->Get())
		{
			buffer = nullptr;
			return;
		}

		// reset data ptr
		data = nullptr;
	}

	D3d12UploadBufferBase::~D3d12UploadBufferBase()
	{
	}

	bool D3d12UploadBufferBase::MapAll()
	{
		HRESULT hr = buffer->Get()->Map(0, nullptr, reinterpret_cast<void**>(&data));
		if (FAILED(hr))
			return false;
		return true;
	}

	void D3d12UploadBufferBase::UnmapAll()
	{
		buffer->Get()->Unmap(0, nullptr);
		data = nullptr;
	}

	bool D3d12UploadBufferBase::UploadData(UINT i, BYTE* struct_ptr, UINT _struct_count)
	{
		if (data == nullptr)
		{
			D3D12_RANGE range1{ 0,0 };
			HRESULT hr = buffer->Get()->Map(0, &range1, reinterpret_cast<void**>(&data));
			if (FAILED(hr))
				return false;
			for (UINT j = 0, k = i; j < _struct_count; j++, k++)
				::memcpy(data + k * struct_align_size, struct_ptr + j * struct_size, struct_size);
			D3D12_RANGE range2{ struct_align_size * i,struct_align_size * (i + _struct_count) };
			buffer->Get()->Unmap(0, &range2);
			data = nullptr;
		}
		else
		{
			for (UINT j = 0, k = i; j < _struct_count; j++, k++)
				::memcpy(data + k * struct_align_size, struct_ptr + j * struct_size, struct_size);
		}
		return true;
	}

	D3d12DefaultTexture::D3d12DefaultTexture(void* image, UINT _pixel_size, UINT _width, UINT _height, UINT _count, DXGI_FORMAT _format,
		D3D12_RESOURCE_STATES _state, D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list)
		: pixel_size(_pixel_size), width(_width), height(_height), count(_count), format(_format), rc_init_state(_state)
	{
		HRESULT hr = S_OK;

		// create upload
		UINT image_pitch = width * pixel_size;
		UINT upload_pitch = D3d12Util::Align(image_pitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
		UINT upload_page = upload_pitch * height;
		UINT upload_size = upload_page * count;
		UINT pitch_count = height * count;
		upload = std::make_shared<D3d12Buffer>(1, upload_size, D3D12_HEAP_TYPE_UPLOAD, device->Get());
		if (!upload->Get())
		{
			texture = nullptr;
			return;
		}

		// copy from image
		void* data = nullptr;
		hr = upload->Get()->Map(0, nullptr, &data);
		if (FAILED(hr))
		{
			texture = nullptr;
			return;
		}
		for (UINT i = 0; i < pitch_count; i++)
		{
			void* dst = static_cast<BYTE*>(data) + i * upload_pitch;
			void* src = static_cast<BYTE*>(image) + i * image_pitch;
			::memcpy(dst, src, image_pitch);
		}
		upload->Get()->Unmap(0, nullptr);

		// create texture
		auto rc_desc = D3d12Util::GetTexture2dRcDesc(width, height, 1, format, static_cast<UINT16>(count));
		texture = std::make_shared<D3d12Texture>(rc_desc, D3D12_HEAP_TYPE_DEFAULT, device->Get(), D3D12_RESOURCE_STATE_COPY_DEST);
		if (!texture->Get())
		{
			texture = nullptr;
			return;
		}

		// copy from upload
		D3D12_TEXTURE_COPY_LOCATION dst_loc{}, src_loc{};
		for (UINT i = 0; i < count; i++)
		{
			dst_loc.pResource = texture->Get();
			dst_loc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			dst_loc.SubresourceIndex = i;
			src_loc.pResource = upload->Get();
			src_loc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			src_loc.PlacedFootprint.Offset = static_cast<UINT64>(i) * upload_page;
			src_loc.PlacedFootprint.Footprint.Format = format;
			src_loc.PlacedFootprint.Footprint.Width = width;
			src_loc.PlacedFootprint.Footprint.Height = height;
			src_loc.PlacedFootprint.Footprint.Depth = 1;
			src_loc.PlacedFootprint.Footprint.RowPitch = upload_pitch;
			cmd_list->CopyTextureRegion(&dst_loc, 0, 0, 0, &src_loc, nullptr);
		}
		if (rc_init_state != D3D12_RESOURCE_STATE_COPY_DEST)
		{
			auto rc_barr = D3d12Util::GetTransitionStruct(texture->Get(), D3D12_RESOURCE_STATE_COPY_DEST, rc_init_state);
			cmd_list->ResourceBarrier(1, &rc_barr);
		}
	}

	D3d12DefaultTexture::~D3d12DefaultTexture()
	{
	}

}