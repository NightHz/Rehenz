#include "d3d12device.h"

namespace Rehenz
{
    D3d12Device::D3d12Device() : vp{}, sr{}
    {
        current_fence_v = 0;

        cmd_list_is_closed = true;
        cmd_alloc_count = 2;
        current_cmd_alloc_i = 0;

        sc_format = DXGI_FORMAT_B8G8R8A8_UNORM;
        sc_buffer_count = 2;

        rtv_size = dsv_size = cbv_size = sampler_size = 0;
        rtv_heap_size = 80;
        dsv_heap_size = 20;
        cbv_heap_size = 800;
        sampler_heap_size = 20;
    }

    D3d12Device::~D3d12Device()
    {
    }

    ID3D12GraphicsCommandList6* D3d12Device::Create(SimpleWindow* window)
    {
        if (device)
            return nullptr;

        HRESULT hr = S_OK;

#ifdef _DEBUG
        // enable debug layer
        ComPtr<ID3D12Debug3> debugger;
        hr = D3D12GetDebugInterface(IID_PPV_ARGS(debugger.GetAddressOf()));
        if (FAILED(hr))
            return nullptr;
        debugger->EnableDebugLayer();
#endif

        // create factory
        ComPtr<IDXGIFactory7> factory;
#ifdef _DEBUG
        hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(factory.GetAddressOf()));
#else
        hr = CreateDXGIFactory2(0, IID_PPV_ARGS(factory.GetAddressOf()));
#endif
        if (FAILED(hr))
            return nullptr;

        // create device
        hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(device.GetAddressOf()));
        if (FAILED(hr))
            return nullptr;

        // create fence
        hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
        if (FAILED(hr))
            return nullptr;
        current_fence_v = fence->GetCompletedValue();

        // create command queue & list & allocator
        D3D12_COMMAND_QUEUE_DESC cmd_queue_desc{};
        cmd_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        cmd_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        cmd_queue_desc.Priority = 0;
        cmd_queue_desc.NodeMask = 0;
        hr = device->CreateCommandQueue(&cmd_queue_desc, IID_PPV_ARGS(cmd_queue.GetAddressOf()));
        if (FAILED(hr))
            return nullptr;
        current_cmd_alloc_i = 0;
        cmd_allocs = std::make_unique<ComPtr<ID3D12CommandAllocator>[]>(cmd_alloc_count);
        cmd_fence_vs = std::make_unique<UINT64[]>(cmd_alloc_count);
        for (int i = 0; i < cmd_alloc_count; i++)
        {
            hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmd_allocs[i].GetAddressOf()));
            if (FAILED(hr))
                return nullptr;
        }
        hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CurrentCmdAlloc(), nullptr, IID_PPV_ARGS(cmd_list.GetAddressOf()));
        if (FAILED(hr))
            return nullptr;
        cmd_list_is_closed = false;

        // create swap chain
        DXGI_SWAP_CHAIN_DESC1 sc_desc1{};
        sc_desc1.Width = window->GetWidth();
        sc_desc1.Height = window->GetHeight();
        sc_desc1.Format = sc_format;
        sc_desc1.Stereo = false;
        sc_desc1.SampleDesc.Count = 1;
        sc_desc1.SampleDesc.Quality = 0;
        sc_desc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sc_desc1.BufferCount = sc_buffer_count;
        sc_desc1.Scaling = DXGI_SCALING_STRETCH;
        sc_desc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        sc_desc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        sc_desc1.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        ComPtr<IDXGISwapChain1> sc1;
        hr = factory->CreateSwapChainForHwnd(cmd_queue.Get(), window->GetHwnd(), &sc_desc1, nullptr, nullptr, sc1.GetAddressOf());
        if (FAILED(hr))
            return nullptr;
        hr = sc1.As(&sc);
        if (FAILED(hr))
            return nullptr;
        // get buffer of swap chain
        sc_buffers = std::make_unique<ComPtr<ID3D12Resource2>[]>(sc_buffer_count);
        for (UINT i = 0; i < sc_buffer_count; i++)
        {
            hr = sc->GetBuffer(i, IID_PPV_ARGS(sc_buffers[i].GetAddressOf()));
            if (FAILED(hr))
                return nullptr;
        }

        // get descriptor size
        rtv_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        dsv_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        cbv_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        sampler_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
        // create descriptor heap
        D3D12_DESCRIPTOR_HEAP_DESC dh_desc{};
        if (rtv_heap_size > 0)
        {
            dh_desc = D3d12Util::GetDescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, rtv_heap_size);
            hr = device->CreateDescriptorHeap(&dh_desc, IID_PPV_ARGS(rtv_heap.GetAddressOf()));
            if (FAILED(hr))
                return nullptr;
        }
        if (dsv_heap_size > 0)
        {
            dh_desc = D3d12Util::GetDescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, dsv_heap_size);
            hr = device->CreateDescriptorHeap(&dh_desc, IID_PPV_ARGS(dsv_heap.GetAddressOf()));
            if (FAILED(hr))
                return nullptr;
        }
        if (cbv_heap_size > 0)
        {
            dh_desc = D3d12Util::GetDescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, cbv_heap_size);
            hr = device->CreateDescriptorHeap(&dh_desc, IID_PPV_ARGS(cbv_heap.GetAddressOf()));
            if (FAILED(hr))
                return nullptr;
        }
        if (sampler_heap_size > 0)
        {
            dh_desc = D3d12Util::GetDescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, sampler_heap_size);
            hr = device->CreateDescriptorHeap(&dh_desc, IID_PPV_ARGS(sampler_heap.GetAddressOf()));
            if (FAILED(hr))
                return nullptr;
        }

        // set viewport
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        vp.Width = static_cast<float>(window->GetWidth());
        vp.Height = static_cast<float>(window->GetHeight());
        vp.MinDepth = 0;
        vp.MaxDepth = 1;

        // set scissor rect
        sr.left = 0;
        sr.right = window->GetWidth();
        sr.top = 0;
        sr.bottom = window->GetHeight();

        // create root sig
        // 0 CBV : space0 : b0
        // 1 DT  : space0 : (b1 b2 b3 b4 b5)
        // 2 DT  : space0 : (t0 t1 t2 t3 t4 t5)
        // 3 DT  : space0 : (u0 u1 u2 u3 u4 u5)
        // 4 DT  : space1 : (s0 s1 s2 s3 s4 s5)
        // ssampler : space0 : (s0 s1 s2 s3 s4 s5)
        D3D12_DESCRIPTOR_RANGE dr1[1]{};
        dr1[0] = D3d12Util::GetDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 5, 1); // b1 b2 b3 b4 b5
        D3D12_DESCRIPTOR_RANGE dr2[1]{};
        dr2[0] = D3d12Util::GetDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 6, 0); // t0 t1 t2 t3 t4 t5
        D3D12_DESCRIPTOR_RANGE dr3[1]{};
        dr3[0] = D3d12Util::GetDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 6, 0); // u0 u1 u2 u3 u4 u5
        D3D12_DESCRIPTOR_RANGE dr4[1]{};
        dr4[0] = D3d12Util::GetDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 6, 0, 1); // s0 s1 s2 s3 s4 s5 (space1)
        D3D12_ROOT_PARAMETER rp[5]{};
        rp[0] = D3d12Util::GetRootParameterCBV(0); // b0
        rp[1] = D3d12Util::GetRootParameterDT(_countof(dr1), dr1);
        rp[2] = D3d12Util::GetRootParameterDT(_countof(dr2), dr2);
        rp[3] = D3d12Util::GetRootParameterDT(_countof(dr3), dr3);
        rp[4] = D3d12Util::GetRootParameterDT(_countof(dr4), dr4);
        D3D12_STATIC_SAMPLER_DESC ssampler_desc[6]{};
        ssampler_desc[0] = D3d12Util::GetSSamplerDesc(0, 0, D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP);
        ssampler_desc[1] = D3d12Util::GetSSamplerDesc(1, 0, D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
        ssampler_desc[2] = D3d12Util::GetSSamplerDesc(2, 0, D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_WRAP);
        ssampler_desc[3] = D3d12Util::GetSSamplerDesc(3, 0, D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
        ssampler_desc[4] = D3d12Util::GetSSamplerDesc(4, 0, D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_WRAP);
        ssampler_desc[5] = D3d12Util::GetSSamplerDesc(5, 0, D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
        D3D12_ROOT_SIGNATURE_DESC rs_desc{};
        rs_desc.NumParameters = _countof(rp);
        rs_desc.pParameters = rp;
        rs_desc.NumStaticSamplers = _countof(ssampler_desc);
        rs_desc.pStaticSamplers = ssampler_desc;
        rs_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        ComPtr<ID3DBlob> root_sig_blob;
        hr = D3D12SerializeRootSignature(&rs_desc, D3D_ROOT_SIGNATURE_VERSION_1, root_sig_blob.GetAddressOf(), nullptr);
        if (FAILED(hr))
            return nullptr;
        hr = device->CreateRootSignature(0, root_sig_blob->GetBufferPointer(), root_sig_blob->GetBufferSize(), IID_PPV_ARGS(root_sig.GetAddressOf()));
        if (FAILED(hr))
            return nullptr;

        return cmd_list.Get();
    }

    bool D3d12Device::FlushGpu()
    {
        if (!device)
            return false;
        current_fence_v++;
        HRESULT hr = cmd_queue->Signal(fence.Get(), current_fence_v);
        if (FAILED(hr))
            return false;
        if (!D3d12Util::WaitFenceValue(fence.Get(), current_fence_v))
            return false;
        return true;
    }

    bool D3d12Device::CheckCmdAllocator()
    {
        if (!device)
            return false;
        return CurrentCmdFenceValue() <= fence->GetCompletedValue();
    }

    bool D3d12Device::WaitCmdAllocator()
    {
        if (!device)
            return false;
        if (!D3d12Util::WaitFenceValue(fence.Get(), CurrentCmdFenceValue()))
            return false;
        return true;
    }

    ID3D12GraphicsCommandList6* D3d12Device::ResetCommand(bool set_buildin_objs)
    {
        if (!device)
            return nullptr;

        HRESULT hr = S_OK;

        // check command allocator state
        if (!CheckCmdAllocator())
            return nullptr;

        // reset command list
        auto cmd_alloc = CurrentCmdAlloc();
        hr = cmd_alloc->Reset();
        if (FAILED(hr))
            return nullptr;
        hr = cmd_list->Reset(cmd_alloc, nullptr);
        if (FAILED(hr))
            return nullptr;
        cmd_list_is_closed = false;

        // set heaps
        ID3D12DescriptorHeap* dhs[]{ cbv_heap.Get(), sampler_heap.Get() };
        cmd_list->SetDescriptorHeaps(_countof(dhs), dhs);

        if (set_buildin_objs)
        {
            // set viewport & scissor rect
            cmd_list->RSSetViewports(1, &vp);
            cmd_list->RSSetScissorRects(1, &sr);

            // set root sig
            cmd_list->SetGraphicsRootSignature(root_sig.Get());
        }

        return cmd_list.Get();
    }

    bool D3d12Device::ExecuteCommand()
    {
        if (!device)
            return false;

        HRESULT hr = S_OK;

        // finish command list
        hr = cmd_list->Close();
        if (FAILED(hr))
            return false;
        cmd_list_is_closed = true;

        // execute commands
        ID3D12CommandList* lists[]{ cmd_list.Get() };
        cmd_queue->ExecuteCommandLists(1, lists);

        // mark in fence
        current_fence_v++;
        CurrentCmdFenceValue() = current_fence_v;
        hr = cmd_queue->Signal(fence.Get(), CurrentCmdFenceValue());
        if (FAILED(hr))
            return false;

        // next
        NextCmdAlloc();

        return true;
    }

    bool D3d12Device::ExecuteCommandAndPresent(ID3D12Resource2* image, bool msaa)
    {
        if (!device)
            return false;

        HRESULT hr = S_OK;

        // copy to swap chain back buffer
        if (image != nullptr)
        {
            if (!msaa)
            {
                D3D12_RESOURCE_BARRIER rc_barr1[]{
                    D3d12Util::GetTransitionStruct(image,
                    D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE),
                    D3d12Util::GetTransitionStruct(CurrentScBuffer(),
                    D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_DEST)
                };
                cmd_list->ResourceBarrier(_countof(rc_barr1), rc_barr1);
                cmd_list->CopyResource(CurrentScBuffer(), image);
                D3D12_RESOURCE_BARRIER rc_barr2[]{
                    D3d12Util::GetTransitionStruct(image,
                    D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
                    D3d12Util::GetTransitionStruct(CurrentScBuffer(),
                    D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT)
                };
                cmd_list->ResourceBarrier(_countof(rc_barr2), rc_barr2);
            }
            else
            {
                D3D12_RESOURCE_BARRIER rc_barr1[]{
                    D3d12Util::GetTransitionStruct(image,
                    D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_RESOLVE_SOURCE),
                    D3d12Util::GetTransitionStruct(CurrentScBuffer(),
                    D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RESOLVE_DEST)
                };
                cmd_list->ResourceBarrier(_countof(rc_barr1), rc_barr1);
                cmd_list->ResolveSubresource(CurrentScBuffer(), 0, image, 0, sc_format);
                D3D12_RESOURCE_BARRIER rc_barr2[]{
                    D3d12Util::GetTransitionStruct(image,
                    D3D12_RESOURCE_STATE_RESOLVE_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
                    D3d12Util::GetTransitionStruct(CurrentScBuffer(),
                    D3D12_RESOURCE_STATE_RESOLVE_DEST, D3D12_RESOURCE_STATE_PRESENT)
                };
                cmd_list->ResourceBarrier(_countof(rc_barr2), rc_barr2);
            }
        }

        // finish command list
        hr = cmd_list->Close();
        if (FAILED(hr))
            return false;
        cmd_list_is_closed = true;

        // execute commands
        ID3D12CommandList* lists[]{ cmd_list.Get() };
        cmd_queue->ExecuteCommandLists(1, lists);

        // mark in fence
        current_fence_v++;
        CurrentCmdFenceValue() = current_fence_v;
        hr = cmd_queue->Signal(fence.Get(), CurrentCmdFenceValue());
        if (FAILED(hr))
            return false;

        // swap
        hr = sc->Present(0, 0);
        if (FAILED(hr))
            return false;

        // next
        NextCmdAlloc();

        return true;
    }

    D3D_FEATURE_LEVEL D3d12Device::CheckMaxFeatureLevel()
    {
        if (!device)
            return static_cast<D3D_FEATURE_LEVEL>(0);

        HRESULT hr = S_OK;

        D3D12_FEATURE_DATA_FEATURE_LEVELS levels{};
        std::vector<D3D_FEATURE_LEVEL> fls{
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1,
            D3D_FEATURE_LEVEL_1_0_CORE };
        levels.pFeatureLevelsRequested = &fls[0];
        levels.NumFeatureLevels = static_cast<UINT>(fls.size());
        hr = device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &levels, sizeof(levels));
        if (FAILED(hr))
            return static_cast<D3D_FEATURE_LEVEL>(0);

        return levels.MaxSupportedFeatureLevel;
    }

    UINT D3d12Device::CheckMsaa4xQuality()
    {
        if (!device)
            return 0;

        HRESULT hr = S_OK;

        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS levels{};
        levels.Format = sc_format;
        levels.SampleCount = 4;
        levels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
        hr = device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &levels, sizeof(levels));
        if (FAILED(hr))
            return 0;

        return levels.NumQualityLevels;
    }

    void D3d12Device::PrintSupportInfo(std::ostream& out)
    {
        if (!device)
            return;

        D3D_FEATURE_LEVEL level = CheckMaxFeatureLevel();
        out << "MaxSupportedFeatureLevel : ";
        switch (level)
        {
        case D3D_FEATURE_LEVEL_12_1: out << "D3D_FEATURE_LEVEL_12_1"; break;
        case D3D_FEATURE_LEVEL_12_0: out << "D3D_FEATURE_LEVEL_12_0"; break;
        case D3D_FEATURE_LEVEL_11_1: out << "D3D_FEATURE_LEVEL_11_1"; break;
        case D3D_FEATURE_LEVEL_11_0: out << "D3D_FEATURE_LEVEL_11_0"; break;
        case D3D_FEATURE_LEVEL_10_1: out << "D3D_FEATURE_LEVEL_10_1"; break;
        case D3D_FEATURE_LEVEL_10_0: out << "D3D_FEATURE_LEVEL_10_0"; break;
        case D3D_FEATURE_LEVEL_9_3: out << "D3D_FEATURE_LEVEL_9_3"; break;
        case D3D_FEATURE_LEVEL_9_2: out << "D3D_FEATURE_LEVEL_9_2"; break;
        case D3D_FEATURE_LEVEL_9_1: out << "D3D_FEATURE_LEVEL_9_1"; break;
        case D3D_FEATURE_LEVEL_1_0_CORE: out << "D3D_FEATURE_LEVEL_1_0_CORE"; break;
        default: out << "unknown or error";
        }
        out << std::endl;

        UINT multisample_level = CheckMsaa4xQuality();
        out << "Msaa4x NumQualityLevels : " << multisample_level << std::endl;
    }

    void D3d12Device::PrintAdapterInfo(std::wostream& out)
    {
        HRESULT hr = S_OK;

        ComPtr<IDXGIFactory> factory;
        hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));
        if (FAILED(hr))
            return;

        out << L"adapters :" << std::endl;
        for (int i = 0; true; i++)
        {
            ComPtr<IDXGIAdapter> adapter;
            DXGI_ADAPTER_DESC desc;
            hr = factory->EnumAdapters(i, adapter.GetAddressOf());
            if (hr == DXGI_ERROR_NOT_FOUND)
                break;
            if (FAILED(hr))
                return;
            hr = adapter->GetDesc(&desc);
            if (FAILED(hr))
                return;
            out << L"  " << desc.Description << std::endl;
        }
    }

    void D3d12Device::PrintAdapterOutputInfo(std::wostream& out)
    {
        HRESULT hr = S_OK;

        ComPtr<IDXGIFactory> factory;
        hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));
        if (FAILED(hr))
            return;

        out << L"adapters :" << std::endl;
        for (int i = 0; true; i++)
        {
            ComPtr<IDXGIAdapter> adapter;
            DXGI_ADAPTER_DESC desc;
            hr = factory->EnumAdapters(i, adapter.GetAddressOf());
            if (hr == DXGI_ERROR_NOT_FOUND)
                break;
            if (FAILED(hr))
                return;
            hr = adapter->GetDesc(&desc);
            if (FAILED(hr))
                return;
            out << L"  " << desc.Description << std::endl;
            for (int j = 0; true; j++)
            {
                ComPtr<IDXGIOutput> output;
                DXGI_OUTPUT_DESC desc2;
                hr = adapter->EnumOutputs(j, output.GetAddressOf());
                if (hr == DXGI_ERROR_NOT_FOUND)
                    break;
                if (FAILED(hr))
                    return;
                hr = output->GetDesc(&desc2);
                if (FAILED(hr))
                    return;
                out << L"    " << desc2.DeviceName << std::endl;
            }
        }
    }

    void D3d12Device::PrintAdapterOutputDisplayInfo(std::wostream& out)
    {
        HRESULT hr = S_OK;

        ComPtr<IDXGIFactory> factory;
        hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));
        if (FAILED(hr))
            return;

        out << L"adapters :" << std::endl;
        for (int i = 0; true; i++)
        {
            ComPtr<IDXGIAdapter> adapter;
            DXGI_ADAPTER_DESC desc;
            hr = factory->EnumAdapters(i, adapter.GetAddressOf());
            if (hr == DXGI_ERROR_NOT_FOUND)
                break;
            if (FAILED(hr))
                return;
            hr = adapter->GetDesc(&desc);
            if (FAILED(hr))
                return;
            out << L"  " << desc.Description << std::endl;
            for (int j = 0; true; j++)
            {
                ComPtr<IDXGIOutput> output;
                DXGI_OUTPUT_DESC desc2;
                hr = adapter->EnumOutputs(j, output.GetAddressOf());
                if (hr == DXGI_ERROR_NOT_FOUND)
                    break;
                if (FAILED(hr))
                    return;
                hr = output->GetDesc(&desc2);
                if (FAILED(hr))
                    return;
                out << L"    " << desc2.DeviceName << std::endl;

                DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
                UINT count = 0;
                hr = output->GetDisplayModeList(format, 0, &count, nullptr);
                if (FAILED(hr))
                    return;
                if (count > 0)
                {
                    std::vector<DXGI_MODE_DESC> descs3(count);
                    hr = output->GetDisplayModeList(format, 0, &count, &descs3[0]);
                    if (FAILED(hr))
                        return;
                    for (auto& desc3 : descs3)
                    {
                        out << L"      " << desc3.Height << L" x " << desc3.Width;
                        out << L" (" << desc3.RefreshRate.Numerator << L"/" << desc3.RefreshRate.Denominator << L")" << std::endl;
                    }
                }
            }
        }
    }

}