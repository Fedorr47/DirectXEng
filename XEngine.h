﻿#pragma once

#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <array>

#include "Window/d3dApp.h"
#include "Math/MathHelper.h"
#include "Common/UploadBuffer.h"
#include "Geometry/GeometryGenerator.h"
#include "Camera/Camera.h"
#include "FrameResource/FrameResource.h"
#include "Shadow/ShadowMap.h"
#include "SSAO/Ssao.h"
#include "Common/RenderItem.h"
#include "Meshes/SkinnedMeshLoader.h"
#include "LoadM3d.h"

class TextureManager;

class DirextXEng : public D3DApp
{
public:
    DirextXEng(HINSTANCE hInstance);
    DirextXEng(const DirextXEng& rhs) = delete;
    DirextXEng& operator=(const DirextXEng& rhs) = delete;
    ~DirextXEng();

    virtual bool Initialize()override;

private:
    virtual void CreateRtvAndDsvDescriptorHeaps() override;
    virtual void OnResize()override;
    virtual void Update(const GameTimer& gt) override;
    virtual void Draw(const GameTimer& gt) override;

    void OnKeyboardInput(const GameTimer& gt);
	void AnimateMaterials(const GameTimer& gt);
	void UpdateObjectCBs(const GameTimer& gt);
    void UpdateSkinnedCBs(const GameTimer& gt);
	void UpdateMaterialBuffer(const GameTimer& gt);
    void UpdateShadowTransform(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);
    void UpdateShadowPassCB(const GameTimer& gt);
    void UpdateSsaoCB(const GameTimer& gt);

	void LoadTextures();
    void BuildRootSignature();
    void BuildSsaoRootSignature();
	void BuildDescriptorHeaps();
    void BuildShadersAndInputLayout();
    void BuildShapeGeometry();
	void LoadSkinnedModel();
    void BuildPSOs();
    void BuildFrameResources();
    void BuildMaterials();
    void BuildRenderItems();
    void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);
    void DrawSceneToShadowMap();
	void DrawNormalsAndDepth();

    CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuSrv(int index)const;
    CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuSrv(int index)const;
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetDsv(int index)const;
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtv(int index)const;

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> GetStaticSamplers();
	
    std::vector<std::unique_ptr<FrameResource>> mFrameResources;
    FrameResource* mCurrFrameResource = nullptr;
    int mCurrFrameResourceIndex = 0;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mSsaoRootSignature = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3DBlob>> mShaders;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
    std::vector<D3D12_INPUT_ELEMENT_DESC> mSkinnedInputLayout;
	
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	// Render items divided by PSO.
	std::vector<RenderItem*> mRitemLayer[(int)RenderLayer::Count];

	UINT mSkyTexHeapIndex = 0;
    UINT mShadowMapHeapIndex = 0;
    UINT mSsaoHeapIndexStart = 0;
    UINT mSsaoAmbientMapIndex = 0;

    UINT mNullCubeSrvIndex = 0;
    UINT mNullTexSrvIndex1 = 0;
    UINT mNullTexSrvIndex2 = 0;

    CD3DX12_GPU_DESCRIPTOR_HANDLE mNullSrv;

    PassConstants mMainPassCB;  // index 0 of pass cbuffer.
    PassConstants mShadowPassCB;// index 1 of pass cbuffer.

	// TODO: It should be move to skinned mesh logic
    UINT mSkinnedSrvHeapStart = 0;
    std::string mSkinnedModelFilename = "Models\\soldier.m3d";
    std::unique_ptr<SkinnedModelInstance> mSkinnedModelInst; 
    SkinnedData mSkinnedInfo;
    std::vector<M3DLoader::Subset> mSkinnedSubsets;
    std::vector<M3DLoader::M3dMaterial> mSkinnedMats;
    std::vector<std::string> mSkinnedTextureNames;

	std::shared_ptr<Camera> mCamera;

    std::unique_ptr<ShadowMap> mShadowMap;
    std::unique_ptr<Ssao> mSsao;
    DirectX::BoundingSphere mSceneBounds;

    float mLightNearZ = 0.0f;
    float mLightFarZ = 0.0f;
    DirectX::XMFLOAT3 mLightPosW;
    DirectX::XMFLOAT4X4 mLightView = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 mLightProj = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 mShadowTransform = MathHelper::Identity4x4();

    float mLightRotationAngle = 0.0f;
	// TODO: Lights should be dynamic object not static
    DirectX::XMFLOAT3 mBaseLightDirections[3] = {
        DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f),
        DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f),
        DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f)
    };
    DirectX::XMFLOAT3 mRotatedLightDirections[3];

	// TODO: move to control class
    POINT mLastMousePos;
	
	std::unique_ptr<TextureManager> mTextureManager{nullptr};

	// imgui
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mImGuiSrvHeap;
	float mSliderValue{1.0f};
};
