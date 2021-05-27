/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/Resources/Animation/Skeleton.hpp>
#include <Engine/Resources/Animation/Skin.hpp>
#include <Engine/Resources/Importer/AssimpUtilities.hpp>
#include <Engine/Core/Debug/Assert.hpp>
#include <GPM/Matrix4.hpp>
#include <glad/glad.h>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

using namespace GPE;

Skin::Skin()
{
    glGenBuffers(1, &m_skinVbo);
}

Skin::~Skin()
{
    glDeleteBuffers(1, &m_skinVbo);
}

void Skin::setVertexAttribArray(int boneIDs, int weights)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_skinVbo);

    // ids
    glEnableVertexAttribArray(boneIDs);
    glVertexAttribIPointer(boneIDs, VertexBoneData::maxBoneInfluence, GL_INT, sizeof(m_verticesBoneData.front()),
                           (GLvoid*)offsetof(VertexBoneData, m_boneIDs));

    // weights
    glEnableVertexAttribArray(weights);
    glVertexAttribPointer(weights, VertexBoneData::maxBoneInfluence, GL_FLOAT, GL_FALSE,
                          sizeof(m_verticesBoneData.front()), (GLvoid*)offsetof(VertexBoneData, m_weights));
}

void Skin::uploadBufferData()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_skinVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesBoneData.front()) * m_verticesBoneData.size(),
                 m_verticesBoneData.data(), GL_STATIC_DRAW);
}

void Skin::setVertexBoneDataToDefault(VertexBoneData& vertexBoneData)
{
    for (int i = 0; i < VertexBoneData::maxBoneInfluence; i++)
    {
        vertexBoneData.m_boneIDs[i] = -1;
        vertexBoneData.m_weights[i] = 0.0f;
    }
}

void Skin::setVertexBoneData(VertexBoneData& vertexBoneData, int boneID, float weight)
{
    for (int i = 0; i < VertexBoneData::maxBoneInfluence; ++i)
    {
        if (vertexBoneData.m_boneIDs[i] < 0)
        {
            vertexBoneData.m_weights[i] = weight;
            vertexBoneData.m_boneIDs[i] = boneID;
            break;
        }
    }
}

void GPE::loadSkinAndSkeleton(Skin& skin, Skeleton& skeleton, aiMesh* mesh)
{
    skin.m_verticesBoneData.resize(mesh->mNumVertices);
    for (Skin::VertexBoneData& vertexBoneData : skin.m_verticesBoneData)
    {
        skin.setVertexBoneDataToDefault(vertexBoneData);
    }

    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int         boneID   = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (skeleton.m_boneInfoMap.find(boneName) == skeleton.m_boneInfoMap.end())
        {
            Skeleton::BoneInfo newBoneInfo;
            newBoneInfo.id     = skeleton.m_boneCounter;
            newBoneInfo.offset = GPE::toMat4(mesh->mBones[boneIndex]->mOffsetMatrix);

            skeleton.m_boneInfoMap[boneName] = newBoneInfo;
            boneID                           = skeleton.m_boneCounter;
            skeleton.m_boneCounter++;
        }
        else
        {
            boneID = skeleton.m_boneInfoMap[boneName].id;
        }
        GPE_ASSERT(boneID != -1, "Every bone should be used.");
        auto weights    = mesh->mBones[boneIndex]->mWeights;
        int  numWeights = mesh->mBones[boneIndex]->mNumWeights;

        if (numWeights != 0)
        {
            for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                int   vertexId = weights[weightIndex].mVertexId;
                float weight   = weights[weightIndex].mWeight;
                GPE_ASSERT(vertexId <= skin.m_verticesBoneData.size(), "Index should be valid.");
                skin.setVertexBoneData(skin.m_verticesBoneData[vertexId], boneID, weight);
            }
        }
    }
}