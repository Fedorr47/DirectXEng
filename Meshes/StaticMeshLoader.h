#pragma once

#include <memory>
#include <string>
#include <d3d12.h>

struct MeshGeometry;

class StaticMeshLoader final
{
    public:
    StaticMeshLoader(){}
    ~StaticMeshLoader(){}

    static std::unique_ptr<MeshGeometry> StaticMeshLoader::LoadModelFromTextFile(
        std::string_view filename,
        std::string_view geometryName,
        std::string_view objectName,
        ID3D12Device* device,
        ID3D12GraphicsCommandList* cmdList);
};
