#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "d3dUtil.h"

class TextureManager
{
public:
    TextureManager(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
        : mDevice(device), mCmdList(cmdList) {}

    void AddTexture(const std::string& name, const std::wstring& filename);
    void AddSkinnedTexture(const std::string& textureName);
    const std::unordered_map<std::string, std::unique_ptr<Texture>>& GetTextures() const { return mTextures; }

private:
    ID3D12Device* mDevice;
    ID3D12GraphicsCommandList* mCmdList;
    std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;

    std::wstring StripExtension(const std::string& filename) const;
};
