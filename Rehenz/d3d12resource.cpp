#include "d3d12resource.h"

namespace Rehenz
{
	D3d12Buffer::D3d12Buffer(UINT _count, UINT _struct_size, D3D12_HEAP_TYPE _heap_type, ID3D12Device8* device, D3D12_RESOURCE_STATES state, D3D12_RESOURCE_FLAGS flag)
	{
		count = _count;
		struct_size = _struct_size;
		buffer_size = count * struct_size;
		heap_type = _heap_type;

		auto rc_desc = D3d12Util::GetBufferRcDesc(buffer_size, flag);
		auto heap_prop = D3d12Util::GetHeapProperties(heap_type);
		HRESULT hr = device->CreateCommittedResource(&heap_prop, D3D12_HEAP_FLAG_NONE, &rc_desc, state, nullptr, IID_PPV_ARGS(buffer.GetAddressOf()));
		if (FAILED(hr))
		{
			buffer = nullptr;
			return;
		}
	}

	D3d12Buffer::~D3d12Buffer()
	{
	}

	D3d12Texture::D3d12Texture(const D3D12_RESOURCE_DESC& rc_desc, D3D12_HEAP_TYPE _heap_type, ID3D12Device8* device, D3D12_RESOURCE_STATES state, D3D12_CLEAR_VALUE* clear_value)
	{
		width = static_cast<UINT>(rc_desc.Width);
		height = rc_desc.Height;
		mip_levels = rc_desc.MipLevels;
		format = rc_desc.Format;
		array_size = rc_desc.DepthOrArraySize;
		heap_type = _heap_type;

		auto heap_prop = D3d12Util::GetHeapProperties(heap_type);
		HRESULT hr = device->CreateCommittedResource(&heap_prop, D3D12_HEAP_FLAG_NONE, &rc_desc, state, clear_value, IID_PPV_ARGS(texture.GetAddressOf()));
		if (FAILED(hr))
		{
			texture = nullptr;
			return;
		}
	}

	D3d12Texture::~D3d12Texture()
	{
	}

}