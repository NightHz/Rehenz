#pragma once
#include "d3d12util.h"

namespace Rehenz
{
    class D3d12BufferBase
    {
    private:
        UINT count;
        UINT struct_size;
        UINT buffer_size;
        D3D12_HEAP_TYPE heap_type;
        ComPtr<ID3D12Resource2> buffer;
        //BYTE* data;

    public:
        D3d12BufferBase(UINT _count, UINT _struct_size, D3D12_HEAP_TYPE _heap_type, ID3D12Device8* device,
            bool is_cbuffer = false, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE,
            D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_GENERIC_READ);
        //D3d12BufferBase(const D3d12BufferBase& _buffer, D3D12_HEAP_TYPE _heap_type, ID3D12Device8* device,
        //    ID3D12GraphicsCommandList6* cmd_list, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE,
        //    D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_GENERIC_READ);
        D3d12BufferBase(const D3d12BufferBase&) = delete;
        D3d12BufferBase& operator=(const D3d12BufferBase&) = delete;
        ~D3d12BufferBase();

        inline ID3D12Resource2* Get() { return buffer.Get(); }
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
    };

    /*template<typename T>
    class D3d12Buffer : public D3d12BufferBase
    {
    public:
        D3d12Buffer(UINT _count, D3D12_HEAP_TYPE _heap_type, ID3D12Device8* device,
            D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE)
            : D3d12BufferBase(_count, sizeof(T), _heap_type, device, flag) {}
        D3d12Buffer(const D3d12Buffer& _buffer, D3D12_HEAP_TYPE _heap_type, ID3D12Device8* device,
            ID3D12GraphicsCommandList6* cmd_list, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE)
            : D3d12BufferBase(_buffer, _heap_type, device, cmd_list, flag) {}
        D3d12Buffer(const D3d12Buffer&) = delete;
        D3d12Buffer& operator=(const D3d12Buffer&) = delete;
        ~D3d12Buffer() {}

        inline bool CopyData(UINT slot, const T& cb_struct)
        {
            return D3d12BufferBase::CopyData(slot, &cb_struct);
        }
    };*/

    class D3d12Texture
    {
    private:
        UINT width;
        UINT height;
        UINT16 mip_levels;
        static const DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
        D3D12_HEAP_TYPE heap_type;
        ComPtr<ID3D12Resource2> texture;

    public:
        D3d12Texture(const D3D12_RESOURCE_DESC& rc_desc, D3D12_HEAP_TYPE _heap_type, ID3D12Device8* device,
            D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_CLEAR_VALUE* clear_value = nullptr);
        //D3d12Texture(const D3d12Texture& _texture, D3D12_HEAP_TYPE _heap_type, ID3D12Device8* device,
        //    ID3D12GraphicsCommandList6* cmd_list, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_GENERIC_READ);
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
        inline D3D12_UNORDERED_ACCESS_VIEW_DESC GetUavDesc()
        {
            D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc{};
            uav_desc.Format = format;
            uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
            uav_desc.Texture2D.MipSlice = 0;
            uav_desc.Texture2D.PlaneSlice = 0;
            return uav_desc;
        }
    };
}