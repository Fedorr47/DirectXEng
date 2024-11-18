#include <gtest/gtest.h>
#include "RenderItem.h"

const int gNumFrameResources = 3;

TEST(RenderItemTest, DefaultInitialization) {
    RenderItem item;
    
    EXPECT_EQ(item.NumFramesDirty, 0);
    EXPECT_EQ(item.ObjCBIndex, -1u);
    EXPECT_EQ(item.Mat, nullptr);
    EXPECT_EQ(item.Geo, nullptr);
    EXPECT_EQ(item.PrimitiveType, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    EXPECT_EQ(item.IndexCount, 0u);
    EXPECT_EQ(item.StartIndexLocation, 0u);
    EXPECT_EQ(item.BaseVertexLocation, 0);
    EXPECT_EQ(item.RenderCBIndex, -1u);
    EXPECT_EQ(item.SkinnedModelInst, nullptr);
    
    EXPECT_EQ(memcmp(&item.World, &MathHelper::Identity4x4(), sizeof(DirectX::XMFLOAT4X4)), 0);
    EXPECT_EQ(memcmp(&item.TexTransform, &MathHelper::Identity4x4(), sizeof(DirectX::XMFLOAT4X4)), 0);
}

TEST(RenderItemTest, InitializationWithNumFrames) {
    RenderItem item(5);

    EXPECT_EQ(item.NumFramesDirty, 5);
}

TEST(RenderItemTest, ModifyValues) {
    RenderItem item;

    item.NumFramesDirty = 3;
    item.ObjCBIndex = 10;
    item.IndexCount = 100;
    item.StartIndexLocation = 20;
    item.BaseVertexLocation = -5;

    EXPECT_EQ(item.NumFramesDirty, 3);
    EXPECT_EQ(item.ObjCBIndex, 10u);
    EXPECT_EQ(item.IndexCount, 100u);
    EXPECT_EQ(item.StartIndexLocation, 20u);
    EXPECT_EQ(item.BaseVertexLocation, -5);
}

TEST(RenderItemTest, AssignMaterialAndGeometry) {
    Material mat;
    MeshGeometry geo;

    RenderItem item;
    item.Mat = &mat;
    item.Geo = &geo;

    EXPECT_EQ(item.Mat, &mat);
    EXPECT_EQ(item.Geo, &geo);
}

TEST(RenderItemTest, ModifyMatrices) {
    RenderItem item;
    
    DirectX::XMFLOAT4X4 newWorldMatrix = {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    item.World = newWorldMatrix;

    EXPECT_EQ(memcmp(&item.World, &newWorldMatrix, sizeof(DirectX::XMFLOAT4X4)), 0);
}

TEST(RenderItemTest, SkinnedModelSeparation) {
    SkinnedModelInstance skinnedInstance;

    RenderItem item;
    item.SkinnedModelInst = &skinnedInstance;
    item.RenderCBIndex = 2;

    EXPECT_EQ(item.SkinnedModelInst, &skinnedInstance);
    EXPECT_EQ(item.RenderCBIndex, 2u);
}