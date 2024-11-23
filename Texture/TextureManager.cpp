#include "TextureManager.h"

void TextureManager::AddTexture(const std::string& name, const std::wstring& filename)
{
    if (mTextures.find(name) == mTextures.end())
    {
        auto texMap = std::make_unique<Texture>();
        texMap->Name = name;
        texMap->Filename = filename;

        ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(
            mDevice,
            mCmdList,
            texMap->Filename.c_str(),
            texMap->Resource,
            texMap->UploadHeap
        ));

        mTextures[name] = std::move(texMap);
    }
}

void TextureManager::AddSkinnedTexture(const std::string& textureName, std::vector<std::string>& SkinnedTextureNames)
{
    std::wstring filename = L"Textures/" + AnsiToWString(textureName);
    std::string nameWithoutExtension = StripExtension(textureName);
    SkinnedTextureNames.push_back(nameWithoutExtension);
    AddTexture(nameWithoutExtension, filename);
}

std::string TextureManager::StripExtension(const std::string& filename) const
{
    return std::string(filename.begin(), filename.begin() + filename.find_last_of('.'));
}
