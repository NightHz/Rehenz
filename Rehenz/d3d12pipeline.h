#pragma once
#include "d3d12util.h"

namespace Rehenz
{
    class D3d12GPSOCreator
    {
    public:
        D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc;

    public:
        D3d12GPSOCreator();
        ~D3d12GPSOCreator();

        void SetRSig(ID3D12RootSignature* rs);
        void SetShader(ID3DBlob* vs, ID3DBlob* ps, ID3DBlob* ds = nullptr, ID3DBlob* hs = nullptr, ID3DBlob* gs = nullptr);
        void SetIA(const std::vector<D3D12_INPUT_ELEMENT_DESC>& desc,
            D3D12_PRIMITIVE_TOPOLOGY_TYPE topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
            D3D12_INDEX_BUFFER_STRIP_CUT_VALUE strip_cut = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED);
        void SetRS(const D3D12_RASTERIZER_DESC& rd);
        void SetOMTest(const D3D12_DEPTH_STENCIL_DESC& dsd);
        void SetOMBlend(const D3D12_BLEND_DESC& bd, UINT sample_mask = UINT_MAX);
        void SetRenderTargets(bool msaa = false, UINT count = 1, DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM);

        ComPtr<ID3D12PipelineState> CreatePSO(ID3D12Device8* device);
    };

    class D3d12CPSOCreator
    {
    public:
        D3D12_COMPUTE_PIPELINE_STATE_DESC pso_desc;

    public:
        D3d12CPSOCreator();
        ~D3d12CPSOCreator();

        void SetRSig(ID3D12RootSignature* rs);
        void SetShader(ID3DBlob* cs);

        ComPtr<ID3D12PipelineState> CreatePSO(ID3D12Device8* device);
    };
}