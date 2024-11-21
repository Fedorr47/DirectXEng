#include "StaticMeshLoader.h"

#include <D3Dcompiler.h>
#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <DirectXMathVector.inl>
#include <xmmintrin.h>
#include <dxgiformat.h>
#include <filesystem>
#include <fstream>
#include <intsafe.h>
#include <stdexcept>
#include <vector>

#include "d3dUtil.h"
#include "FrameResource/FrameResource.h"


std::unique_ptr<MeshGeometry> StaticMeshLoader::LoadModelFromTextFile(
    std::string_view filename,
    std::string_view geometryName,
    std::string_view objectName,
    ID3D12Device* device,
    ID3D12GraphicsCommandList* cmdList)
{
    std::filesystem::path filePath = filename;
    if (!std::filesystem::exists(filePath))
    {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }
    
    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        throw std::runtime_error("Unable to read file: " + filePath.string());
    }
    
    UINT vertexCount = 0;
    UINT triangleCount = 0;
    std::string ignore;

    fin >> ignore >> vertexCount;
    fin >> ignore >> triangleCount;
    fin >> ignore >> ignore >> ignore >> ignore;
    
    DirectX::XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
    DirectX::XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

    DirectX::XMVECTOR vMinVec = DirectX::XMLoadFloat3(&vMinf3);
    DirectX::XMVECTOR vMaxVec = DirectX::XMLoadFloat3(&vMaxf3);
    
    std::vector<Vertex> vertices(vertexCount);
    for (UINT i = 0; i < vertexCount; ++i)
    {
        fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
        fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;

        DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&vertices[i].Pos);

        vertices[i].TexC = { 0.0f, 0.0f }; // Если текстурные координаты не указаны

        vMinVec = DirectX::XMVectorMin(vMinVec, pos);
        vMaxVec = DirectX::XMVectorMax(vMaxVec, pos);
    }
    
    DirectX::BoundingBox bounds;
    DirectX::XMStoreFloat3(
    &bounds.Center, 
    DirectX::XMVectorScale(DirectX::XMVectorAdd(vMinVec, vMaxVec), 0.5f));
    DirectX::XMStoreFloat3(
        &bounds.Extents, 
        DirectX::XMVectorScale(DirectX::XMVectorSubtract(vMaxVec, vMinVec), 0.5f));

    fin >> ignore;
    fin >> ignore;
    fin >> ignore;
    
    std::vector<std::int32_t> indices(3 * triangleCount);
    for (UINT i = 0; i < triangleCount; ++i)
    {
        fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
    }

    fin.close();

    const UINT vbByteSize = static_cast<UINT>(vertices.size() * sizeof(Vertex));
    const UINT ibByteSize = static_cast<UINT>(indices.size() * sizeof(std::int32_t));
    
    auto geo = std::make_unique<MeshGeometry>();
    geo->Name = std::string(geometryName);
    
    ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
    CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
    
    ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
    CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);
    
    geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(
        device, cmdList, vertices.data(), vbByteSize, geo->VertexBufferUploader);

    geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(
        device, cmdList, indices.data(), ibByteSize, geo->IndexBufferUploader);
    
    geo->VertexByteStride = sizeof(Vertex);
    geo->VertexBufferByteSize = vbByteSize;
    geo->IndexFormat = DXGI_FORMAT_R32_UINT;
    geo->IndexBufferByteSize = ibByteSize;

    SubmeshGeometry submesh;
    submesh.IndexCount = static_cast<UINT>(indices.size());
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;
    submesh.Bounds = bounds;

    geo->DrawArgs[std::string(objectName)] = submesh;

    return geo;
}
