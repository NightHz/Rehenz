#include "d3d12util.h"
#include <fstream>

namespace Rehenz
{
    ComPtr<ID3DBlob> D3d12Util::LoadShaderFile(const std::wstring& filename)
    {
        HRESULT hr = S_OK;

        ComPtr<ID3DBlob> shader_blob;

        // open file
        std::ifstream fin(filename, std::ios::binary);
        if (!fin)
            return nullptr;

        // get size
        fin.seekg(0, std::ios_base::end);
        SIZE_T size = static_cast<SIZE_T>(fin.tellg());
        fin.seekg(0, std::ios_base::beg);

        // read to blob
        hr = D3DCreateBlob(size, shader_blob.GetAddressOf());
        if (FAILED(hr))
        {
            fin.close();
            return nullptr;
        }
        fin.read(static_cast<char*>(shader_blob->GetBufferPointer()), size);
        fin.close();

        return shader_blob;
    }

    ComPtr<ID3DBlob> D3d12Util::CompileShaderFile(const std::wstring& filename, const std::string& shader_type, const std::unordered_map<std::string, bool>& macro)
    {
        HRESULT hr = S_OK;

        ComPtr<ID3DBlob> shader_blob;

        // get defines
        std::vector<D3D_SHADER_MACRO> defines;
        for (const auto& m : macro)
            defines.push_back(D3D_SHADER_MACRO{ m.first.c_str(),(m.second ? "1" : "0") });
        defines.push_back(D3D_SHADER_MACRO{ nullptr,nullptr });

        // compile shader
        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
        flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
        ComPtr<ID3DBlob> error_blob;
        hr = D3DCompileFromFile(filename.c_str(), &defines[0], D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", (shader_type + "_5_1").c_str(),
            flags, 0, shader_blob.GetAddressOf(), error_blob.GetAddressOf());
        if (FAILED(hr))
        {
#ifdef _DEBUG
            if (error_blob)
                OutputDebugString(static_cast<char*>(error_blob->GetBufferPointer()));
#endif
            return nullptr;
        }

        return shader_blob;
    }

    bool D3d12Util::WaitFenceValue(ID3D12Fence1* fence, UINT64 value)
    {
        UINT64 completed_value = fence->GetCompletedValue();
        if (completed_value < value)
        {

            HANDLE event = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
            if (!event)
                return false;
            HRESULT hr = fence->SetEventOnCompletion(value, event);
            if (FAILED(hr))
                return false;
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
        return true;
    }

}