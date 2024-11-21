#pragma once
#include <filesystem>
#include <vector>
#include "../LoadM3d.h"
#include "d3dUtil.h"

struct MeshGeometry;
struct SkinnedModelInstance;
class SkinnedData;

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