#include "d3d12pipeline.h"

namespace Rehenz
{
    D3d12GPSOCreator::D3d12GPSOCreator() : pso_desc{}
    {
        Reset();
    }

    D3d12GPSOCreator::~D3d12GPSOCreator()
    {
    }

    void D3d12GPSOCreator::Reset()
    {
        // root signature
        pso_desc.pRootSignature = nullptr;

        // shader
        pso_desc.VS.pShaderBytecode = nullptr;
        pso_desc.PS.pShaderBytecode = nullptr;
        pso_desc.DS.pShaderBytecode = nullptr;
        pso_desc.HS.pShaderBytecode = nullptr;
        pso_desc.GS.pShaderBytecode = nullptr;
        pso_desc.VS.BytecodeLength = 0;
        pso_desc.PS.BytecodeLength = 0;
        pso_desc.DS.BytecodeLength = 0;
        pso_desc.HS.BytecodeLength = 0;
        pso_desc.GS.BytecodeLength = 0;

        // SO
        pso_desc.StreamOutput.pSODeclaration = nullptr;
        pso_desc.StreamOutput.NumEntries = 0;
        pso_desc.StreamOutput.pBufferStrides = nullptr;
        pso_desc.StreamOutput.NumStrides = 0;
        pso_desc.StreamOutput.RasterizedStream = 0;

        // OM : blend part
        pso_desc.BlendState.AlphaToCoverageEnable = false;
        pso_desc.BlendState.IndependentBlendEnable = false;
        pso_desc.BlendState.RenderTarget[0].BlendEnable = false;
        pso_desc.BlendState.RenderTarget[0].LogicOpEnable = false;
        pso_desc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
        pso_desc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
        pso_desc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
        pso_desc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
        pso_desc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
        pso_desc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        pso_desc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        pso_desc.SampleMask = UINT_MAX;

        // RS
        pso_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        pso_desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
        pso_desc.RasterizerState.FrontCounterClockwise = false;
        pso_desc.RasterizerState.DepthBias = 0;
        pso_desc.RasterizerState.DepthBiasClamp = 0;
        pso_desc.RasterizerState.SlopeScaledDepthBias = 0;
        pso_desc.RasterizerState.DepthClipEnable = true;
        pso_desc.RasterizerState.MultisampleEnable = false;
        pso_desc.RasterizerState.AntialiasedLineEnable = false;
        pso_desc.RasterizerState.ForcedSampleCount = 0;
        pso_desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        // OM : test part
        pso_desc.DepthStencilState.DepthEnable = true;
        pso_desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        pso_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        pso_desc.DepthStencilState.StencilEnable = false;
        pso_desc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
        pso_desc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
        pso_desc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        pso_desc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        pso_desc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        pso_desc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        pso_desc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        pso_desc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        pso_desc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        pso_desc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

        // IA
        pso_desc.InputLayout.pInputElementDescs = nullptr;
        pso_desc.InputLayout.NumElements = 0;
        pso_desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
        pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        // render target
        pso_desc.NumRenderTargets = 1;
        pso_desc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
        for (UINT i = 1; i < 8; i++)
            pso_desc.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
        pso_desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        pso_desc.SampleDesc.Count = 1;
        pso_desc.SampleDesc.Quality = 0;

        // other
        pso_desc.NodeMask = 0;
        pso_desc.CachedPSO.pCachedBlob = nullptr;
        pso_desc.CachedPSO.CachedBlobSizeInBytes = 0;
        pso_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
    }

    void D3d12GPSOCreator::SetRSig(ID3D12RootSignature* rs)
    {
        pso_desc.pRootSignature = rs;
    }

    void D3d12GPSOCreator::SetShader(ID3DBlob* vs, ID3DBlob* ps, ID3DBlob* ds, ID3DBlob* hs, ID3DBlob* gs)
    {
        pso_desc.VS.pShaderBytecode = (vs ? vs->GetBufferPointer() : nullptr);
        pso_desc.VS.BytecodeLength = (vs ? vs->GetBufferSize() : 0);
        pso_desc.PS.pShaderBytecode = (ps ? ps->GetBufferPointer() : nullptr);
        pso_desc.PS.BytecodeLength = (ps ? ps->GetBufferSize() : 0);
        pso_desc.DS.pShaderBytecode = (ds ? ds->GetBufferPointer() : nullptr);
        pso_desc.DS.BytecodeLength = (ds ? ds->GetBufferSize() : 0);
        pso_desc.HS.pShaderBytecode = (hs ? hs->GetBufferPointer() : nullptr);
        pso_desc.HS.BytecodeLength = (hs ? hs->GetBufferSize() : 0);
        pso_desc.GS.pShaderBytecode = (gs ? gs->GetBufferPointer() : nullptr);
        pso_desc.GS.BytecodeLength = (gs ? gs->GetBufferSize() : 0);
    }

    void D3d12GPSOCreator::SetIA(const std::vector<D3D12_INPUT_ELEMENT_DESC>& desc, D3D12_PRIMITIVE_TOPOLOGY_TYPE topology, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE strip_cut)
    {
        pso_desc.InputLayout.pInputElementDescs = &desc[0];
        pso_desc.InputLayout.NumElements = static_cast<UINT>(desc.size());
        pso_desc.IBStripCutValue = strip_cut;
        pso_desc.PrimitiveTopologyType = topology;
    }

    void D3d12GPSOCreator::SetRS(const D3D12_RASTERIZER_DESC& rd)
    {
        pso_desc.RasterizerState = rd;
    }

    void D3d12GPSOCreator::SetOMTest(const D3D12_DEPTH_STENCIL_DESC& dsd)
    {
        pso_desc.DepthStencilState = dsd;
    }

    void D3d12GPSOCreator::SetOMBlend(const D3D12_BLEND_DESC& bd, UINT sample_mask)
    {
        pso_desc.BlendState = bd;
        pso_desc.SampleMask = sample_mask;
    }

    void D3d12GPSOCreator::SetRenderTargets(bool msaa, UINT count, DXGI_FORMAT format)
    {
        pso_desc.NumRenderTargets = count;
        for (UINT i = 0; i < count; i++)
            pso_desc.RTVFormats[i] = format;
        for (UINT i = count; i < 8; i++)
            pso_desc.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
        pso_desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        pso_desc.SampleDesc.Count = (msaa ? 4 : 1);
        pso_desc.SampleDesc.Quality = 0;
    }

    ComPtr<ID3D12PipelineState> D3d12GPSOCreator::CreatePSO(ID3D12Device8* device)
    {
        ComPtr<ID3D12PipelineState> pso;
        HRESULT hr = device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(pso.GetAddressOf()));
        if (FAILED(hr))
            return nullptr;
        return pso;
    }

    D3d12CPSOCreator::D3d12CPSOCreator() : pso_desc{}
    {
        Reset();
    }

    D3d12CPSOCreator::~D3d12CPSOCreator()
    {
    }

    void D3d12CPSOCreator::Reset()
    {
        // root signature
        pso_desc.pRootSignature = nullptr;

        // shader
        pso_desc.CS.pShaderBytecode = nullptr;
        pso_desc.CS.BytecodeLength = 0;

        // other
        pso_desc.NodeMask = 0;
        pso_desc.CachedPSO.pCachedBlob = nullptr;
        pso_desc.CachedPSO.CachedBlobSizeInBytes = 0;
        pso_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
    }

    void D3d12CPSOCreator::SetRSig(ID3D12RootSignature* rs)
    {
        pso_desc.pRootSignature = rs;
    }

    void D3d12CPSOCreator::SetShader(ID3DBlob* cs)
    {
        pso_desc.CS.pShaderBytecode = (cs ? cs->GetBufferPointer() : nullptr);
        pso_desc.CS.BytecodeLength = (cs ? cs->GetBufferSize() : 0);
    }

    ComPtr<ID3D12PipelineState> D3d12CPSOCreator::CreatePSO(ID3D12Device8* device)
    {
        ComPtr<ID3D12PipelineState> pso;
        HRESULT hr = device->CreateComputePipelineState(&pso_desc, IID_PPV_ARGS(pso.GetAddressOf()));
        if (FAILED(hr))
            return nullptr;
        return pso;
    }

}