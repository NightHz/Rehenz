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

	class D3d12CBufferBase
	{
	private:
		std::shared_ptr<D3d12Buffer> cb;
		BYTE* data;

	public:
		const UINT struct_size;
		const UINT struct_align_size;
		const UINT struct_count;
		const UINT cb_size;
		const bool have_cbv;
		const UINT cbv_start;
		const bool have_srv;
		const UINT srv;

	public:
		D3d12CBufferBase(UINT _struct_size, UINT _struct_count, bool _have_cbv, UINT _cbv_start, bool _have_srv, UINT _srv, D3d12Device* device);
		D3d12CBufferBase(const D3d12CBufferBase&) = delete;
		D3d12CBufferBase& operator=(const D3d12CBufferBase&) = delete;
		~D3d12CBufferBase();

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
		bool FillCB(UINT i, BYTE* cb_struct, UINT cb_struct_count);

		inline D3D12_GPU_VIRTUAL_ADDRESS GetGpuLocation(UINT i)
		{
			return cb->GetGpuLocation(i);
		}
	};

	template<typename T>
	class D3d12CBuffer : public D3d12CBufferBase
	{
	public:
		using CBStruct = T;

	public:
		inline D3d12CBuffer(UINT _struct_count, bool _have_cbv, UINT _cbv_start, bool _have_srv, UINT _srv, D3d12Device* device)
			: D3d12CBufferBase(sizeof(CBStruct), _struct_count, _have_cbv, _cbv_start, _have_srv, _srv, device) {}
		inline D3d12CBuffer(UINT _struct_count, D3d12Device* device)
			: D3d12CBuffer(_struct_count, false, 0, false, 0, device) {}
		inline D3d12CBuffer(UINT _struct_count, UINT _cbv_start, D3d12Device* device)
			: D3d12CBuffer(_struct_count, true, _cbv_start, false, 0, device) {}
		inline D3d12CBuffer(UINT _struct_count, UINT _cbv_start, UINT _srv, D3d12Device* device)
			: D3d12CBuffer(_struct_count, true, _cbv_start, true, _srv, device) {}
		D3d12CBuffer(const D3d12CBuffer&) = delete;
		D3d12CBuffer& operator=(const D3d12CBuffer&) = delete;
		inline ~D3d12CBuffer() {}

		inline bool FillCB(UINT i, CBStruct* cb_struct, UINT cb_struct_count)
		{
			return D3d12CBufferBase::FillCB(i, reinterpret_cast<BYTE*>(cb_struct), cb_struct_count);
		}
	};
}