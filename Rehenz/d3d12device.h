#pragma once
#include "d3d12util.h"
#include "window.h"
#include <ostream>

namespace Rehenz
{
    class D3d12Device
    {
    private:
        // device & fence
        ComPtr<ID3D12Device8> device;
        ComPtr<ID3D12Fence1> fence;
        UINT64 current_fence_v;

        // command queue & list & allocator
        ComPtr<ID3D12CommandQueue> cmd_queue;
        ComPtr<ID3D12GraphicsCommandList6> cmd_list;
        bool cmd_list_is_closed;
        int cmd_alloc_count;
        int current_cmd_alloc_i;
        std::unique_ptr<ComPtr<ID3D12CommandAllocator>[]> cmd_allocs;
        std::unique_ptr<UINT64[]> cmd_fence_vs;
        // get current command allocator
        inline ID3D12CommandAllocator* CurrentCmdAlloc() { return cmd_allocs[current_cmd_alloc_i].Get(); }
        // get current command fence value
        inline UINT64& CurrentCmdFenceValue() { return cmd_fence_vs[current_cmd_alloc_i]; }
        // next command allocator
        inline void NextCmdAlloc() { current_cmd_alloc_i = (current_cmd_alloc_i + 1) % cmd_alloc_count; }

        // swap chain
        DXGI_FORMAT sc_format;
        UINT sc_buffer_count;
        ComPtr<IDXGISwapChain4> sc;
        std::unique_ptr<ComPtr<ID3D12Resource2>[]> sc_buffers;
        // get current swap chain back buffer
        inline ID3D12Resource2* CurrentScBuffer() { return sc_buffers[sc->GetCurrentBackBufferIndex()].Get(); }

        // descriptor heap
        UINT rtv_size, dsv_size, cbv_size, sampler_size;
        UINT rtv_heap_size, dsv_heap_size, cbv_heap_size, sampler_heap_size;
        ComPtr<ID3D12DescriptorHeap> rtv_heap;
        ComPtr<ID3D12DescriptorHeap> dsv_heap;
        ComPtr<ID3D12DescriptorHeap> cbv_heap; // also srv heap and uav heap
        ComPtr<ID3D12DescriptorHeap> sampler_heap;

        // build-in viewport & scissor rect
        D3D12_VIEWPORT vp;
        D3D12_RECT sr;

        // build-in root sig
        // 0 CBV : space0 : b0
        // 1 DT  : space0 : (b1 b2 b3 b4 b5)
        // 2 DT  : space0 : (t0 t1 t2 t3 t4 t5)
        // 3 DT  : space0 : (u0 u1 u2 u3 u4 u5)
        // 4 DT  : space1 : (s0 s1 s2 s3 s4 s5)
        // ssampler : space0 : (s0 s1 s2 s3 s4 s5)
        ComPtr<ID3D12RootSignature> root_sig;

    public:
        D3d12Device();
        D3d12Device(const D3d12Device&) = delete;
        D3d12Device& operator=(const D3d12Device&) = delete;
        ~D3d12Device();

        // get device
        inline ID3D12Device8* Get() { return device.Get(); }

        // create, return nullptr when error
        ID3D12GraphicsCommandList6* Create(SimpleWindow* window);

        // wait gpu finish all tasks
        bool FlushGpu();

        // check next allocator state
        bool CheckCmdAllocator();
        // wait next allocator to be ready
        bool WaitCmdAllocator();
        // reset command list & allocator, return nullptr when error or allocator is not ready
        ID3D12GraphicsCommandList6* ResetCommand(bool set_buildin_objs = true);
        // submit command list to queue
        bool ExecuteCommand();
        // submit command list to queue and present
        bool ExecuteCommandAndPresent(ID3D12Resource2* image, bool msaa,
            D3D12_RESOURCE_STATES image_start = D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATES image_end = D3D12_RESOURCE_STATE_RENDER_TARGET);

        // get swap chain format
        inline DXGI_FORMAT GetScFormat() { return sc_format; }

        // get rtv descriptor
        inline D3D12_CPU_DESCRIPTOR_HANDLE GetRtv(UINT i) { return D3d12Util::GetCpuDescriptor(rtv_heap.Get(), rtv_size, i); }
        // get dsv descriptor
        inline D3D12_CPU_DESCRIPTOR_HANDLE GetDsv(UINT i) { return D3d12Util::GetCpuDescriptor(dsv_heap.Get(), dsv_size, i); }
        // get cbv / srv / uav descriptors
        inline D3D12_CPU_DESCRIPTOR_HANDLE GetCbv(UINT i) { return D3d12Util::GetCpuDescriptor(cbv_heap.Get(), cbv_size, i); }
        // get cbv / srv / uav gpu descriptor
        inline D3D12_GPU_DESCRIPTOR_HANDLE GetCbvGpu(UINT i) { return D3d12Util::GetGpuDescriptor(cbv_heap.Get(), cbv_size, i); }
        // get cbv / srv / uav descriptor
        inline D3D12_CPU_DESCRIPTOR_HANDLE GetSrv(UINT i) { return GetCbv(i); }
        // get cbv / srv / uav gpu descriptor
        inline D3D12_GPU_DESCRIPTOR_HANDLE GetSrvGpu(UINT i) { return GetCbvGpu(i); }
        // get cbv / srv / uav descriptor
        inline D3D12_CPU_DESCRIPTOR_HANDLE GetUav(UINT i) { return GetCbv(i); }
        // get cbv / srv / uav gpu descriptor
        inline D3D12_GPU_DESCRIPTOR_HANDLE GetUavGpu(UINT i) { return GetCbvGpu(i); }
        // get sampler descriptor
        inline D3D12_CPU_DESCRIPTOR_HANDLE GetSampler(UINT i) { return D3d12Util::GetCpuDescriptor(sampler_heap.Get(), sampler_size, i); }
        // get sampler gpu descriptor
        inline D3D12_GPU_DESCRIPTOR_HANDLE GetSamplerGpu(UINT i) { return D3d12Util::GetGpuDescriptor(sampler_heap.Get(), sampler_size, i); }

        // get build-in viewport
        inline const D3D12_VIEWPORT& GetViewport() { return vp; }
        // get build-in scissor rect
        inline const D3D12_RECT& GetScissorRect() { return sr; }

        // get build-in root signature
        inline ID3D12RootSignature* GetRSig() { return root_sig.Get(); }
        // 0 CBV : space0 : b0
        inline void SetRSigCbvFast(D3D12_GPU_VIRTUAL_ADDRESS gpu_loc) { cmd_list->SetGraphicsRootConstantBufferView(0, gpu_loc); }
        // 1 DT  : space0 : (b1 b2 b3 b4 b5)
        inline void SetRSigCbv(D3D12_GPU_DESCRIPTOR_HANDLE gpu_dh) { cmd_list->SetGraphicsRootDescriptorTable(1, gpu_dh); }
        // 2 DT  : space0 : (t0 t1 t2 t3 t4 t5)
        inline void SetRSigSrv(D3D12_GPU_DESCRIPTOR_HANDLE gpu_dh) { cmd_list->SetGraphicsRootDescriptorTable(2, gpu_dh); }
        // 3 DT  : space0 : (u0 u1 u2 u3 u4 u5)
        inline void SetRSigUav(D3D12_GPU_DESCRIPTOR_HANDLE gpu_dh) { cmd_list->SetGraphicsRootDescriptorTable(3, gpu_dh); }
        // 4 DT  : space1 : (s0 s1 s2 s3 s4 s5)
        inline void SetRSigSampler(D3D12_GPU_DESCRIPTOR_HANDLE gpu_dh) { cmd_list->SetGraphicsRootDescriptorTable(4, gpu_dh); }

        // check feature
        D3D_FEATURE_LEVEL CheckMaxFeatureLevel();
        UINT CheckMsaa4xQuality();

        // print info
        void PrintSupportInfo(std::ostream& out);
        static void PrintAdapterInfo(std::wostream& out);
        static void PrintAdapterOutputInfo(std::wostream& out);
        static void PrintAdapterOutputDisplayInfo(std::wostream& out);
    };
}