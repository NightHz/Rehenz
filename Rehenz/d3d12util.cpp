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

    struct D3d12VertexFromRehenzVertex
    {
        XMFLOAT3 pos;
        XMFLOAT3 normal;
        XMFLOAT4 color;
        XMFLOAT2 uv;
        XMFLOAT2 uv2;
    };

    std::vector<D3D12_INPUT_ELEMENT_DESC> D3d12Util::GetRehenzMeshInputLayout()
    {
        std::vector<D3D12_INPUT_ELEMENT_DESC> il(5);
        il[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
        il[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
        il[2] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
        il[3] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
        il[4] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
        return il;
    }

    std::pair<size_t, size_t> D3d12Util::GetRehenzMeshStructSize()
    {
        return std::make_pair(sizeof(D3d12VertexFromRehenzVertex), sizeof(UINT16));
    }

    std::pair<ComPtr<ID3DBlob>, ComPtr<ID3DBlob>> D3d12Util::GetMeshBufferFromRehenzMesh(Mesh* mesh)
    {
        HRESULT hr = S_OK;

        using Vertex = D3d12VertexFromRehenzVertex;

        // create vb & ib
        ComPtr<ID3DBlob> vb, ib;
        size_t vb_size = sizeof(Vertex) * mesh->VertexCount();
        size_t ib_size = sizeof(UINT16) * mesh->IndexCount();
        hr = D3DCreateBlob(vb_size, vb.GetAddressOf());
        if (FAILED(hr))
            return std::make_pair(nullptr, nullptr);
        hr = D3DCreateBlob(ib_size, ib.GetAddressOf());
        if (FAILED(hr))
            return std::make_pair(nullptr, nullptr);

        // copy data
        Vertex* vb_data = reinterpret_cast<Vertex*>(vb->GetBufferPointer());
        UINT16* ib_data = reinterpret_cast<UINT16*>(ib->GetBufferPointer());
        const auto& vertices = mesh->GetVertices();
        const auto& indices = mesh->GetTriangles();
        for (size_t i = 0; i < mesh->VertexCount(); i++)
        {
            vb_data[i].pos = XmFloat3(vertices[i].p);
            vb_data[i].normal = XmFloat3(vertices[i].n);
            vb_data[i].color = XmFloat4(vertices[i].c);
            vb_data[i].uv = XmFloat2(vertices[i].uv);
            vb_data[i].uv2 = XmFloat2(vertices[i].uv2);
        }
        for (size_t i = 0; i < mesh->IndexCount(); i++)
            ib_data[i] = static_cast<UINT16>(indices[i]);

        return std::make_pair(vb, ib);
    }

}