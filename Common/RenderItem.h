#pragma once

#include <DirectXMath.h>
#include "MathHelper.h"
#include "d3dUtil.h"
#include "SkinnedMesh.h"

enum class RenderLayer : int
{
	Opaque = 0,
	SkinnedOpaque,
	Debug,
	Sky,
	Count
};

struct RenderItem
{
    RenderItem() = default;
    RenderItem(const RenderItem& rhs) = delete;

	RenderItem(int inNumFrames) : NumFramesDirty(inNumFrames)
	{}

    DirectX::XMFLOAT4X4 World{MathHelper::Identity4x4()};
    DirectX::XMFLOAT4X4 TexTransform{MathHelper::Identity4x4()};
	
    int NumFramesDirty {0};
	
    UINT ObjCBIndex = -1;
 
    Material* Mat = nullptr;
    MeshGeometry* Geo = nullptr;
	
    D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
    UINT IndexCount = 0;
    UINT StartIndexLocation = 0;
    int BaseVertexLocation = 0;

	// TODO: separate this from RenderItem
	UINT RenderCBIndex = -1;
	SkinnedModelInstance* SkinnedModelInst = nullptr;
};
