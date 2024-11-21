#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <cstdint>
#include "../LoadM3d.h"
#include "SkinnedData.h"
#include "d3dUtil.h"

struct MeshGeometry;

class SkinnedMeshLoader final
{
public:
    SkinnedMeshLoader(){}
    ~SkinnedMeshLoader(){}

    static std::unique_ptr<MeshGeometry> LoadSkinnedModel(
        std::string_view skinnedModelFilename,
        ID3D12Device* device,
        ID3D12GraphicsCommandList* cmdList,
        SkinnedData& skinnedInfo,
        std::unique_ptr<SkinnedModelInstance>& skinnedModelInst,
        std::vector<M3DLoader::Subset>& skinnedSubsets,
        std::vector<M3DLoader::M3dMaterial>& mSkinnedMats);
};

std::unique_ptr<MeshGeometry> SkinnedMeshLoader::LoadSkinnedModel(
    std::string_view skinnedModelFilename,
    ID3D12Device* device,
    ID3D12GraphicsCommandList* cmdList,
    SkinnedData& skinnedInfo,
    std::unique_ptr<SkinnedModelInstance>& skinnedModelInst,
    std::vector<M3DLoader::Subset>& skinnedSubsets,
    std::vector<M3DLoader::M3dMaterial>& skinnedMats)
{
    std::filesystem::path filePath = skinnedModelFilename;
    if (!std::filesystem::exists(filePath))
    {
        throw std::runtime_error("File not found: " + filePath.string());
    }
    
    std::vector<M3DLoader::SkinnedVertex> vertices;
    std::vector<std::uint16_t> indices;

    M3DLoader m3dLoader;
    m3dLoader.LoadM3d(filePath.string(), vertices, indices, skinnedSubsets, skinnedMats, skinnedInfo);
    
    skinnedModelInst = std::make_unique<SkinnedModelInstance>();
    skinnedModelInst->SkinnedInfo = &skinnedInfo;
    skinnedModelInst->FinalTransforms.resize(skinnedInfo.BoneCount());
    skinnedModelInst->ClipName = "Take1";
    skinnedModelInst->TimePos = 0.0f;
    
    const UINT vbByteSize = static_cast<UINT>(vertices.size() * sizeof(SkinnedVertex));
    const UINT ibByteSize = static_cast<UINT>(indices.size() * sizeof(std::uint16_t));

    auto geo = std::make_unique<MeshGeometry>();
    // TODO: It is a correct string, but rihgt now I need to use the next one
    //geo->Name = filePath.filename().string();
    geo->Name = skinnedModelFilename;
    
    ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
    CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

    ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
    CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);
    
    geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(
        device, cmdList, vertices.data(), vbByteSize, geo->VertexBufferUploader);

    geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(
        device, cmdList, indices.data(), ibByteSize, geo->IndexBufferUploader);
    
    geo->VertexByteStride = sizeof(M3DLoader::SkinnedVertex);
    geo->VertexBufferByteSize = vbByteSize;
    geo->IndexFormat = DXGI_FORMAT_R16_UINT;
    geo->IndexBufferByteSize = ibByteSize;
    
    for (UINT i = 0; i < static_cast<UINT>(skinnedSubsets.size()); ++i)
    {
        SubmeshGeometry submesh;
        std::string name = "sm_" + std::to_string(i);

        submesh.IndexCount = skinnedSubsets[i].FaceCount * 3;
        submesh.StartIndexLocation = skinnedSubsets[i].FaceStart * 3;
        submesh.BaseVertexLocation = 0;

        geo->DrawArgs[name] = submesh;
    }

    return geo;
}
