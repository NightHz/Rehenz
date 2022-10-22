#pragma once
#include "d3d12util.h"

namespace Rehenz
{
    class D3d12Buffer
    {
    private:
        UINT count;
        UINT struct_size;
        UINT buffer_size;
        D3D12_HEAP_TYPE heap_type;
        ComPtr<ID3D12Resource2> buffer;

    public:
        D3d12Buffer(UINT _count, UINT _struct_size, D3D12_HEAP_TYPE _heap_type, ID3D12Device8* device,
            D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE);
        D3d12Buffer(const D3d12Buffer&) = delete;
        D3d12Buffer& operator=(const D3d12Buffer&) = delete;
        ~D3d12Buffer();

        inline ID3D12Resource2* Get() { return buffer.Get(); }
        inline UINT GetCount() { return count; }
        inline UINT GetSize() { return buffer_size; }
        inline D3D12_GPU_VIRTUAL_ADDRESS GetGpuLocation(UINT i)
        {
            return buffer->GetGPUVirtualAddress() + i * static_cast<UINT64>(struct_size);
        }
        inline D3D12_CONSTANT_BUFFER_VIEW_DESC GetCbvDesc(UINT i, UINT _count = 1)
        {
            D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc{};
            cbv_desc.BufferLocation = GetGpuLocation(i);
            cbv_desc.SizeInBytes = _count * struct_size;
            return cbv_desc;
        }
        inline D3D12_SHADER_RESOURCE_VIEW_DESC GetSrvDesc(UINT i = 0, UINT _count = 0)
        {
            if (_count == 0)
                _count = count - i;
            D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
            srv_desc.Format = DXGI_FORMAT_UNKNOWN;
            srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srv_desc.Buffer.FirstElement = i;
            srv_desc.Buffer.NumElements = _count;
            srv_desc.Buffer.StructureByteStride = struct_size;
            srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
            return srv_desc;
        }
        inline D3D12_UNORDERED_ACCESS_VIEW_DESC GetUavDesc(UINT i = 0, UINT _count = 0)
        {
            if (_count == 0)
                _count = count - i;
            D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc{};
            uav_desc.Format = DXGI_FORMAT_UNKNOWN;
            uav_desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
            uav_desc.Buffer.FirstElement = i;
            uav_desc.Buffer.NumElements = _count;
            uav_desc.Buffer.StructureByteStride = struct_size;
            uav_desc.Buffer.CounterOffsetInBytes = 0;
            uav_desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
            return uav_desc;
        }
        inline D3D12_VERTEX_BUFFER_VIEW GetVbv()
        {
            D3D12_VERTEX_BUFFER_VIEW vbv{};
            vbv.BufferLocation = GetGpuLocation(0);
            vbv.SizeInBytes = buffer_size;
            vbv.StrideInBytes = struct_size;
            return vbv;
        }
        inline D3D12_INDEX_BUFFER_VIEW GetIbv()
        {
            D3D12_INDEX_BUFFER_VIEW ibv{};
            ibv.BufferLocation = GetGpuLocation(0);
            ibv.SizeInBytes = buffer_size;
            ibv.Format = DXGI_FORMAT_R16_UINT;
            return ibv;
        }
    };

    class D3d12Texture
    {
    private:
        UINT width;
        UINT height;
        UINT16 mip_levels;
        DXGI_FORMAT format;
        UINT16 array_size;
        D3D12_HEAP_TYPE heap_type;
        ComPtr<ID3D12Resource2> texture;

    public:
        D3d12Texture(const D3D12_RESOURCE_DESC& rc_desc, D3D12_HEAP_TYPE _heap_type, ID3D12Device8* device,
            D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_CLEAR_VALUE* clear_value = nullptr);
        D3d12Texture(const D3d12Texture&) = delete;
        D3d12Texture& operator=(const D3d12Texture&) = delete;
        ~D3d12Texture();

        inline ID3D12Resource2* Get() { return texture.Get(); }
        inline D3D12_GPU_VIRTUAL_ADDRESS GetGpuLocation()
        {
            return texture->GetGPUVirtualAddress();
        }
        inline D3D12_SHADER_RESOURCE_VIEW_DESC GetSrvDesc()
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
            srv_desc.Format = format;
            srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srv_desc.Texture2D.MostDetailedMip = 0;
            srv_desc.Texture2D.MipLevels = mip_levels;
            srv_desc.Texture2D.PlaneSlice = 0;
            srv_desc.Texture2D.ResourceMinLODClamp = 0;
            return srv_desc;
        }
        inline D3D12_SHADER_RESOURCE_VIEW_DESC GetSrvDescForArray()
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
            srv_desc.Format = format;
            srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
            srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srv_desc.Texture2DArray.MostDetailedMip = 0;
            srv_desc.Texture2DArray.MipLevels = mip_levels;
            srv_desc.Texture2DArray.FirstArraySlice = 0;
            srv_desc.Texture2DArray.ArraySize = array_size;
            srv_desc.Texture2DArray.PlaneSlice = 0;
            srv_desc.Texture2DArray.ResourceMinLODClamp = 0;
            return srv_desc;
        }
        inline D3D12_SHADER_RESOURCE_VIEW_DESC GetSrvDescForCube()
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
            srv_desc.Format = format;
            srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
            srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srv_desc.TextureCube.MostDetailedMip = 0;
            srv_desc.TextureCube.MipLevels = mip_levels;
            srv_desc.TextureCube.ResourceMinLODClamp = 0;
            return srv_desc;
        }
        inline D3D12_UNORDERED_ACCESS_VIEW_DESC GetUavDesc()
        {
            D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc{};
            uav_desc.Format = format;
            uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
            uav_desc.Texture2D.MipSlice = 0;
            uav_desc.Texture2D.PlaneSlice = 0;
            return uav_desc;
        }
        inline D3D12_RENDER_TARGET_VIEW_DESC GetRtvDescForArray(UINT i, UINT count)
        {
            D3D12_RENDER_TARGET_VIEW_DESC rtv_desc{};
            rtv_desc.Format = format;
            rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
            rtv_desc.Texture2DArray.MipSlice = 0;
            rtv_desc.Texture2DArray.FirstArraySlice = i;
            rtv_desc.Texture2DArray.ArraySize = count;
            rtv_desc.Texture2DArray.PlaneSlice = 0;
            return rtv_desc;
        }
    };
}