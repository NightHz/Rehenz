#include "d3d12object.h"

namespace Rehenz
{
	D3d12RenderTarget::D3d12RenderTarget(UINT _width, UINT _height, DXGI_FORMAT _format, bool _msaa, bool _have_zbuffer,
		Color _target_bgc, UINT _target_rtv, UINT _zbuffer_dsv, D3d12Device* device)
		: width(_width), height(_height), format(_format), msaa(_msaa), have_zbuffer(_have_zbuffer),
		target_bgc(_target_bgc), target_rtv(_target_rtv), zbuffer_dsv(_zbuffer_dsv)
	{
		// create target
		auto rc_desc = D3d12Util::GetTexture2dRcDesc(width, height, 1, format, 1, msaa, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
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
			rc_desc = D3d12Util::GetTexture2dRcDesc(width, height, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, 1, msaa, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
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

	D3d12CBufferBase::D3d12CBufferBase(UINT _struct_size, UINT _struct_count, D3d12Device* device)
		: struct_size(_struct_size), struct_align_size(D3d12Util::Align256(_struct_size)), struct_count(_struct_count), cb_size(struct_align_size* struct_count)
	{
		// create cb
		cb = std::make_shared<D3d12Buffer>(struct_count, struct_align_size, D3D12_HEAP_TYPE_UPLOAD, device->Get());
		if (!cb->Get())
		{
			cb = nullptr;
			return;
		}

		data = nullptr;
	}

	D3d12CBufferBase::~D3d12CBufferBase()
	{
	}

	bool D3d12CBufferBase::MapAll()
	{
		HRESULT hr = cb->Get()->Map(0, nullptr, reinterpret_cast<void**>(&data));
		if (FAILED(hr))
			return false;
		return true;
	}

	void D3d12CBufferBase::UnmapAll()
	{
		cb->Get()->Unmap(0, nullptr);
		data = nullptr;
	}

	bool D3d12CBufferBase::FillCB(UINT i, BYTE* cb_struct, UINT cb_struct_count)
	{
		if (data == nullptr)
		{
			D3D12_RANGE range1{ 0,0 };
			HRESULT hr = cb->Get()->Map(0, &range1, reinterpret_cast<void**>(&data));
			if (FAILED(hr))
				return false;
			for (UINT j = 0; j < cb_struct_count; j++, i++)
				::memcpy(data + i * struct_align_size, cb_struct + j * struct_size, struct_size);
			D3D12_RANGE range2{ struct_size * i,struct_size * (i + cb_struct_count) };
			cb->Get()->Unmap(0, &range2);
			data = nullptr;
		}
		else
		{
			for (UINT j = 0; j < cb_struct_count; j++, i++)
				::memcpy(data + i * struct_align_size, cb_struct + j * struct_size, struct_size);
		}
		return true;
	}

}