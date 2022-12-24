#pragma once
#define WIN32_LEAN_AND_MEAN
#include <wrl/client.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include "type.h"
#include "math.h"
#include "util.h"
#include "mesh.h"

namespace Rehenz
{
    // require link d3d12.lib, dxgi.lib and d3dcompiler.lib

    // smart pointer for COM
    using Microsoft::WRL::ComPtr;

    // DirectX Math
    namespace dxm
    {
        using namespace DirectX;
        using namespace DirectX::PackedVector;
    }
    // DX Math Vector & Matrix
    using dxm::XMVECTOR;
    using dxm::FXMVECTOR; // 1,2,3
    using dxm::GXMVECTOR; // 4
    using dxm::HXMVECTOR; // 5,6
    using dxm::CXMVECTOR; // other
    using dxm::XMFLOAT4;
    using dxm::XMFLOAT3;
    using dxm::XMFLOAT2;
    using dxm::XMMATRIX;
    using dxm::FXMMATRIX; // 1
    using dxm::CXMMATRIX; // other
    using dxm::XMFLOAT4X4;

    // Rehenz Math ==> DX Math
    inline XMVECTOR XmVector(Vector v)
    {
        XMFLOAT4 f4(v.x, v.y, v.z, v.w);
        return dxm::XMLoadFloat4(&f4);
    }
    inline XMFLOAT4 XmFloat4(Vector v)
    {
        return XMFLOAT4(v.x, v.y, v.z, v.w);
    }
    inline XMFLOAT3 XmFloat3(Vector3 v)
    {
        return XMFLOAT3(v.x, v.y, v.z);
    }
    inline XMFLOAT2 XmFloat2(Vector2 v)
    {
        return XMFLOAT2(v.x, v.y);
    }
    inline XMMATRIX XmMatrix(const Matrix& m)
    {
        XMFLOAT4X4 f4x4(
            m(0, 0), m(0, 1), m(0, 2), m(0, 3),
            m(1, 0), m(1, 1), m(1, 2), m(1, 3),
            m(2, 0), m(2, 1), m(2, 2), m(2, 3),
            m(3, 0), m(3, 1), m(3, 2), m(3, 3));
        return dxm::XMLoadFloat4x4(&f4x4);
    }
    inline XMFLOAT4X4 XmFloat4x4(const Matrix& m)
    {
        return XMFLOAT4X4(
            m(0, 0), m(0, 1), m(0, 2), m(0, 3),
            m(1, 0), m(1, 1), m(1, 2), m(1, 3),
            m(2, 0), m(2, 1), m(2, 2), m(2, 3),
            m(3, 0), m(3, 1), m(3, 2), m(3, 3));
    }

    class D3d12Util
    {
    public:
        D3d12Util() = delete;

        inline static D3D12_RESOURCE_BARRIER GetTransitionStruct(ID3D12Resource2* rc,
            D3D12_RESOURCE_STATES start, D3D12_RESOURCE_STATES end, UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES)
        {
            D3D12_RESOURCE_BARRIER rc_barr{};
            rc_barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            rc_barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            rc_barr.Transition.pResource = rc;
            rc_barr.Transition.Subresource = subresource;
            rc_barr.Transition.StateBefore = start;
            rc_barr.Transition.StateAfter = end;
            return rc_barr;
        }

        inline static D3D12_RESOURCE_DESC GetBufferRcDesc(UINT64 buffer_size, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE)
        {
            D3D12_RESOURCE_DESC rc_desc{};
            rc_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            rc_desc.Alignment = 0;
            rc_desc.Width = buffer_size;
            rc_desc.Height = 1;
            rc_desc.DepthOrArraySize = 1;
            rc_desc.MipLevels = 1;
            rc_desc.Format = DXGI_FORMAT_UNKNOWN;
            rc_desc.SampleDesc.Count = 1;
            rc_desc.SampleDesc.Quality = 0;
            rc_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            rc_desc.Flags = flag;
            return rc_desc;
        }

        inline static D3D12_RESOURCE_DESC GetTexture2dRcDesc(UINT width, UINT height, UINT16 mip_levels,
            DXGI_FORMAT format, UINT16 array_size = 1, bool msaa = false, D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE)
        {
            D3D12_RESOURCE_DESC rc_desc{};
            rc_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            rc_desc.Alignment = 0;
            rc_desc.Width = width;
            rc_desc.Height = height;
            rc_desc.DepthOrArraySize = array_size;
            rc_desc.MipLevels = mip_levels;
            rc_desc.Format = format;
            rc_desc.SampleDesc.Count = msaa ? 4 : 1;
            rc_desc.SampleDesc.Quality = 0;
            rc_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            rc_desc.Flags = flag;
            return rc_desc;
        }

        inline static D3D12_HEAP_PROPERTIES GetHeapProperties(D3D12_HEAP_TYPE type)
        {
            D3D12_HEAP_PROPERTIES heap_prop{};
            heap_prop.Type = type;
            heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            heap_prop.CreationNodeMask = 1;
            heap_prop.VisibleNodeMask = 1;
            return heap_prop;
        }

        inline static D3D12_CLEAR_VALUE GetTexture2dClearStruct(DXGI_FORMAT format, const float color[4])
        {
            D3D12_CLEAR_VALUE clear_value{};
            clear_value.Format = format;
            clear_value.Color[0] = color[0];
            clear_value.Color[1] = color[1];
            clear_value.Color[2] = color[2];
            clear_value.Color[3] = color[3];
            return clear_value;
        }

        inline static D3D12_CLEAR_VALUE GetDepthStencilClearStruct(DXGI_FORMAT format, float depth, UINT8 stencil)
        {
            D3D12_CLEAR_VALUE clear_value{};
            clear_value.Format = format;
            clear_value.DepthStencil.Depth = depth;
            clear_value.DepthStencil.Stencil = stencil;
            return clear_value;
        }

        inline static D3D12_DESCRIPTOR_RANGE GetDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type,
            UINT count, UINT register_i, UINT register_space = 0)
        {
            D3D12_DESCRIPTOR_RANGE dr{};
            dr.RangeType = type;
            dr.NumDescriptors = count;
            dr.BaseShaderRegister = register_i;
            dr.RegisterSpace = register_space;
            dr.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            return dr;
        }

        inline static D3D12_ROOT_PARAMETER GetRootParameterDT(UINT count, D3D12_DESCRIPTOR_RANGE* pdr)
        {
            D3D12_ROOT_PARAMETER rp{};
            rp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rp.DescriptorTable.NumDescriptorRanges = count;
            rp.DescriptorTable.pDescriptorRanges = pdr;
            rp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            return rp;
        }

        inline static D3D12_ROOT_PARAMETER GetRootParameterCBV(UINT register_i, UINT register_space = 0)
        {
            D3D12_ROOT_PARAMETER rp{};
            rp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            rp.Descriptor.ShaderRegister = register_i;
            rp.Descriptor.RegisterSpace = register_space;
            rp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            return rp;
        }

        inline static D3D12_STATIC_SAMPLER_DESC GetSSamplerDesc(UINT register_i, UINT register_space = 0,
            D3D12_FILTER filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE address = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            UINT anisotropy = 1, D3D12_STATIC_BORDER_COLOR border_color = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK)
        {
            D3D12_STATIC_SAMPLER_DESC ssamp_desc{};
            ssamp_desc.Filter = filter;
            ssamp_desc.AddressU = ssamp_desc.AddressV = ssamp_desc.AddressW = address;
            ssamp_desc.MipLODBias = 0;
            ssamp_desc.MaxAnisotropy = anisotropy;
            ssamp_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
            ssamp_desc.BorderColor = border_color;
            ssamp_desc.MinLOD = 0;
            ssamp_desc.MaxLOD = D3D12_FLOAT32_MAX;
            ssamp_desc.ShaderRegister = register_i;
            ssamp_desc.RegisterSpace = register_space;
            ssamp_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            return ssamp_desc;
        }

        inline static D3D12_SAMPLER_DESC GetSamplerDesc(D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE address,
            UINT anisotropy = 1, const std::vector<float> border_color = { 0,0,0,1 })
        {
            D3D12_SAMPLER_DESC samp_desc{};
            samp_desc.Filter = filter;
            samp_desc.AddressU = samp_desc.AddressV = samp_desc.AddressW = address;
            samp_desc.MipLODBias = 0;
            samp_desc.MaxAnisotropy = anisotropy;
            samp_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
            samp_desc.BorderColor[0] = border_color[0];
            samp_desc.BorderColor[1] = border_color[1];
            samp_desc.BorderColor[2] = border_color[2];
            samp_desc.BorderColor[3] = border_color[3];
            samp_desc.MinLOD = 0;
            samp_desc.MaxLOD = D3D12_FLOAT32_MAX;
            return samp_desc;
        }

        inline static D3D12_SAMPLER_DESC GetComparisonSamplerDesc(D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE address,
            D3D12_COMPARISON_FUNC comparison, UINT anisotropy = 1, const std::vector<float> border_color = { 0,0,0,1 })
        {
            D3D12_SAMPLER_DESC samp_desc{};
            samp_desc.Filter = filter;
            samp_desc.AddressU = samp_desc.AddressV = samp_desc.AddressW = address;
            samp_desc.MipLODBias = 0;
            samp_desc.MaxAnisotropy = anisotropy;
            samp_desc.ComparisonFunc = comparison;
            samp_desc.BorderColor[0] = border_color[0];
            samp_desc.BorderColor[1] = border_color[1];
            samp_desc.BorderColor[2] = border_color[2];
            samp_desc.BorderColor[3] = border_color[3];
            samp_desc.MinLOD = 0;
            samp_desc.MaxLOD = D3D12_FLOAT32_MAX;
            return samp_desc;
        }

        inline static D3D12_DESCRIPTOR_HEAP_DESC GetDescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE type,
            bool shader_visible, UINT heap_size)
        {
            D3D12_DESCRIPTOR_HEAP_DESC desc{};
            desc.Type = type;
            desc.Flags = shader_visible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            desc.NumDescriptors = heap_size;
            desc.NodeMask = 0; // for multi-adapter systems
            return desc;
        }

        inline static D3D12_CPU_DESCRIPTOR_HANDLE GetCpuDescriptor(ID3D12DescriptorHeap* heap, UINT descriptor_size, UINT i)
        {
            auto dh = heap->GetCPUDescriptorHandleForHeapStart();
            dh.ptr += descriptor_size * static_cast<SIZE_T>(i);
            return dh;
        }

        inline static D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptor(ID3D12DescriptorHeap* heap, UINT descriptor_size, UINT i)
        {
            auto dh = heap->GetGPUDescriptorHandleForHeapStart();
            dh.ptr += descriptor_size * static_cast<UINT64>(i);
            return dh;
        }

        inline static UINT Align(UINT size, UINT aligned)
        {
            return (size + aligned - 1) / aligned * aligned;
        }

        // align for cbuffer
        inline static UINT Align256(UINT size)
        {
            return (size + 255) & ~255; // (size+255)/256*256
        }

        static ComPtr<ID3DBlob> LoadShaderFile(const std::wstring& filename);
        // shader_type : vs hs ds gs ps cs
        static ComPtr<ID3DBlob> CompileShaderFile(const std::wstring& filename, const std::string& shader_type,
            const std::unordered_map<std::string, std::string>& macro = {}, const std::string& entrypoint_name = "main");

        // forbid value > current fense value
        static bool WaitFenceValue(ID3D12Fence1* fence, UINT64 value);

        static std::vector<D3D12_INPUT_ELEMENT_DESC> GetRehenzMeshInputLayout();

        // return (sizeof(Vertex), sizeof(UINT16))
        static std::pair<UINT, UINT> GetRehenzMeshStructSize();

        // return (vertex buffer, index buufer)
        static std::pair<ComPtr<ID3DBlob>, ComPtr<ID3DBlob>> GetMeshBufferFromRehenzMesh(Mesh* mesh);

    };

    class IndexLoop
    {
    private:
        bool always_dirty;
        bool dirty;
        int i;
        std::vector<UINT> indices;

        inline int PrevI() { return (i + static_cast<int>(indices.size()) - 1) % static_cast<int>(indices.size()); }
        inline int NextI() { return (i + 1) % static_cast<int>(indices.size()); }

    public:
        inline IndexLoop(std::initializer_list<UINT> il, bool _always_dirty = true) : indices{ il }
        {
            always_dirty = _always_dirty;
            dirty = always_dirty;
            i = 0;
            if (indices.size() == 0)
                indices.push_back(0);
        }
        inline IndexLoop(std::vector<UINT> _indices, bool _always_dirty = true) : indices(std::move(_indices))
        {
            always_dirty = _always_dirty;
            dirty = always_dirty;
            i = 0;
            if (indices.size() == 0)
                indices.push_back(0);
        }
        inline IndexLoop(UINT start, UINT end, UINT interval = 1, bool _always_dirty = true)
        {
            for (UINT index = start; index < end; index += interval)
                indices.push_back(index);
            always_dirty = _always_dirty;
            dirty = always_dirty;
            i = 0;
            if (indices.size() == 0)
                indices.push_back(0);
        }
        inline ~IndexLoop() {}

        inline void SetDirty()
        {
            dirty = true;
        }
        inline UINT GetCurrentIndex()
        {
            return indices[i];
        }
        inline UINT UseCurrentIndex()
        {
            if (dirty)
            {
                int index = indices[i];
                i = NextI();
                dirty = always_dirty;
                return index;
            }
            else
                return indices[PrevI()];
        }
    };
}