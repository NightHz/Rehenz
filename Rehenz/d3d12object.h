#pragma once
#include "d3d12util.h"
#include "d3d12device.h"
#include "d3d12resource.h"
#include "color.h"

namespace Rehenz
{
	class D3d12RenderTarget
	{
	private:
		std::shared_ptr<D3d12Texture> target;
		std::shared_ptr<D3d12Texture> zbuffer;

	public:
		const UINT width;
		const UINT height;
		const DXGI_FORMAT format;
		bool msaa;
		bool have_zbuffer;
		const Color target_bgc;
		const UINT target_rtv;
		const UINT zbuffer_dsv;

	public:
		D3d12RenderTarget(UINT _width, UINT _height, DXGI_FORMAT _format, bool _msaa, bool _have_zbuffer,
			Color _target_bgc, UINT _target_rtv, UINT _zbuffer_dsv, D3d12Device* device);
		D3d12RenderTarget(UINT _width, UINT _height, UINT _target_rtv, UINT _zbuffer_dsv, D3d12Device* device)
			: D3d12RenderTarget(_width, _height, DXGI_FORMAT_B8G8R8A8_UNORM, false, true, Color::yellow_green_o, _target_rtv, _zbuffer_dsv, device) {}
		D3d12RenderTarget(const D3d12RenderTarget&) = delete;
		D3d12RenderTarget& operator=(const D3d12RenderTarget&) = delete;
		~D3d12RenderTarget();

		inline operator bool()
		{
			return target != nullptr;
		}
		inline ID3D12Resource2* GetTarget()
		{
			return target->Get();
		}
		inline ID3D12Resource2* GetZbuffer()
		{
			return zbuffer->Get();
		}

		void ClearRenderTargets(D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list);
		void SetRenderTargets(D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list);
	};

	class D3d12Mesh
	{
	private:
		std::shared_ptr<D3d12Buffer> vb, ib;
		std::shared_ptr<D3d12Buffer> vb_upload, ib_upload;

	public:
		const UINT vertex_size;
		const UINT vertex_count;
		const UINT vb_size;
		const std::vector<D3D12_INPUT_ELEMENT_DESC> input_layout;
		const bool have_index_buffer;
		const UINT index_size;
		const UINT index_count;
		const UINT ib_size;
		const D3D_PRIMITIVE_TOPOLOGY topology;

	public:
		D3d12Mesh(void* vertices, UINT _vertex_size, UINT _vertex_count, const std::vector<D3D12_INPUT_ELEMENT_DESC>* _input_layout, UINT16* indices, UINT _index_count,
			D3D_PRIMITIVE_TOPOLOGY _topology, D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list);
		D3d12Mesh(void* vertices, UINT _vertex_size, UINT _vertex_count, UINT16* indices, UINT _index_count, D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list)
			: D3d12Mesh(vertices, _vertex_size, _vertex_count, nullptr, indices, _index_count, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, device, cmd_list) {}
		D3d12Mesh(Mesh* mesh, D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list);
		D3d12Mesh(const D3d12Mesh&) = delete;
		D3d12Mesh& operator=(const D3d12Mesh&) = delete;
		~D3d12Mesh();

		inline operator bool()
		{
			return vb != nullptr;
		}
		inline ID3D12Resource2* GetVB()
		{
			return vb->Get();
		}
		inline ID3D12Resource2* GetIB()
		{
			return ib->Get();
		}

		void CleanUp();

		void SetIA(ID3D12GraphicsCommandList6* cmd_list);
	};

	template<typename T>
	class D3d12CBuffer
	{
	private:
		std::shared_ptr<D3d12Buffer> cb;
		BYTE* data;

	public:
		using CBStruct = T;
		const UINT struct_size;
		const UINT struct_count;
		const UINT cb_size;
		const bool have_cbv;
		const UINT cbv_start;
		const bool have_srv;
		const UINT srv;

	public:
		D3d12CBuffer(CBStruct* cb_init_struct, UINT init_struct_count, UINT _struct_count, bool _have_cbv, UINT _cbv_start,
			bool _have_srv, UINT _srv, D3d12Device* device);
		D3d12CBuffer(UINT _struct_count, D3d12Device* device)
			: D3d12CBuffer(nullptr, 0, _struct_count, false, 0, false, 0, device) {}
		D3d12CBuffer(UINT _struct_count, UINT _cbv_start, D3d12Device* device)
			: D3d12CBuffer(nullptr, 0, _struct_count, true, _cbv_start, false, 0, device) {}
		D3d12CBuffer(UINT _struct_count, UINT _cbv_start, UINT _srv, D3d12Device* device)
			: D3d12CBuffer(nullptr, 0, _struct_count, true, _cbv_start, true, _srv, device) {}
		D3d12CBuffer(const D3d12CBuffer&) = delete;
		D3d12CBuffer& operator=(const D3d12CBuffer&) = delete;
		~D3d12CBuffer();

		inline operator bool()
		{
			return cb != nullptr;
		}
		inline ID3D12Resource2* GetCB()
		{
			return cb->Get();
		}

		bool MapAll();
		void UnmapAll();
		bool FillCB(UINT i, CBStruct* cb_struct, UINT cb_struct_count);

		inline D3D12_GPU_VIRTUAL_ADDRESS GetGpuLocation(UINT i)
		{
			return cb->GetGpuLocation(i);
		}
	};

	template<typename T>
	D3d12CBuffer<T>::D3d12CBuffer(CBStruct* cb_init_struct, UINT init_struct_count, UINT _struct_count, bool _have_cbv, UINT _cbv_start,
		bool _have_srv, UINT _srv, D3d12Device* device)
		: struct_size(D3d12Util::Align256(sizeof(CBStruct))), struct_count(_struct_count), cb_size(struct_size* struct_count),
		have_cbv(_have_cbv), cbv_start(_cbv_start), have_srv(_have_srv), srv(_srv)
	{
		// create cb
		cb = std::make_shared<D3d12Buffer>(struct_count, struct_size, D3D12_HEAP_TYPE_UPLOAD, device->Get());
		if (!cb->Get())
		{
			cb = nullptr;
			return;
		}

		// create cbv & srv
		if (have_cbv)
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc{};
			for (UINT i = 0; i < struct_count; i++)
			{
				cbv_desc = cb->GetCbvDesc(i);
				device->Get()->CreateConstantBufferView(&cbv_desc, device->GetCbv(cbv_start + i));
			}
		}
		if (have_srv)
		{
			auto srv_desc = cb->GetSrvDesc();
			device->Get()->CreateShaderResourceView(cb->Get(), &srv_desc, device->GetSrv(srv));
		}

		// copy init struct
		if (cb_init_struct)
		{
			D3D12_RANGE range{ 0,struct_size * init_struct_count };
			HRESULT hr = cb->Get()->Map(0, &range, reinterpret_cast<void**>(&data));
			if (FAILED(hr))
			{
				cb = nullptr;
				return;
			}
			for (UINT i = 0; i < init_struct_count; i++)
			{
				*reinterpret_cast<CBStruct*>(data) = cb_init_struct[i];
				data += struct_count;
			}
			cb->Get()->Unmap(0, &range);
		}
		data = nullptr;
	}

	template<typename T>
	D3d12CBuffer<T>::~D3d12CBuffer()
	{
	}

	template<typename T>
	bool D3d12CBuffer<T>::MapAll()
	{
		HRESULT hr = cb->Get()->Map(0, nullptr, reinterpret_cast<void**>(&data));
		if (FAILED(hr))
			return false;
		return true;
	}

	template<typename T>
	void D3d12CBuffer<T>::UnmapAll()
	{
		cb->Get()->Unmap(0, nullptr);
		data = nullptr;
	}

	template<typename T>
	bool D3d12CBuffer<T>::FillCB(UINT i, CBStruct* cb_struct, UINT cb_struct_count)
	{
		if (data == nullptr)
		{
			D3D12_RANGE range1{ 0,0 };
			HRESULT hr = cb->Get()->Map(0, &range1, reinterpret_cast<void**>(&data));
			if (FAILED(hr))
				return false;
			for (UINT j = 0; j < cb_struct_count; j++, i++)
				*reinterpret_cast<CBStruct*>(data + i * struct_size) = cb_struct[j];
			D3D12_RANGE range2{ struct_size * i,struct_size * (i + cb_struct_count) };
			cb->Get()->Unmap(0, &range2);
			data = nullptr;
		}
		else
		{
			for (UINT j = 0; j < cb_struct_count; j++, i++)
				*reinterpret_cast<CBStruct*>(data + i * struct_size) = cb_struct[j];
		}
		return true;
	}

}