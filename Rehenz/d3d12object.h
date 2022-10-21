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

	class D3d12UploadBufferBase
	{
	private:
		std::shared_ptr<D3d12Buffer> buffer;
		BYTE* data;

	public:
		const UINT struct_size;
		const bool as_cbuffer;
		const UINT struct_align_size;
		const UINT struct_count;
		const UINT buffer_size;

	public:
		D3d12UploadBufferBase(UINT _struct_size, bool _as_cbuffer, UINT _struct_count, D3d12Device* device);
		D3d12UploadBufferBase(const D3d12UploadBufferBase&) = delete;
		D3d12UploadBufferBase& operator=(const D3d12UploadBufferBase&) = delete;
		~D3d12UploadBufferBase();

		inline operator bool()
		{
			return buffer != nullptr;
		}
		inline ID3D12Resource2* GetBuffer()
		{
			return buffer->Get();
		}
		inline D3d12Buffer* GetBufferObj()
		{
			return buffer.get();
		}

		bool MapAll();
		void UnmapAll();
		bool FillCB(UINT i, BYTE* struct_ptr, UINT _struct_count);
	};

	template<typename T>
	class D3d12UploadBuffer : public D3d12UploadBufferBase
	{
	public:
		using StructType = T;

	public:
		inline D3d12UploadBuffer(bool _as_cbuffer, UINT _struct_count, D3d12Device* device)
			: D3d12UploadBufferBase(sizeof(StructType), _as_cbuffer, _struct_count, device) {}
		D3d12UploadBuffer(const D3d12UploadBuffer&) = delete;
		D3d12UploadBuffer& operator=(const D3d12UploadBuffer&) = delete;
		inline ~D3d12UploadBuffer() {}

		inline bool FillCB(UINT i, StructType* struct_ptr, UINT _struct_count)
		{
			return D3d12UploadBufferBase::FillCB(i, reinterpret_cast<BYTE*>(struct_ptr), _struct_count);
		}
	};
}