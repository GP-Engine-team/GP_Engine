/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Tools/ClassUtility.hpp"
#include <GPM/Matrix4.hpp>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <Engine/Resources/Mesh.hpp>

namespace GPE
{
class Skeleton
{
public:
    struct VertexWeight
    {
        unsigned int vertexID;
        float        weight;
    };

    struct CreateArgs
    {
        std::vector<GPM::Vec3> vertices;

        std::vector<std::string>               boneNames;
        std::vector<GPM::Mat4>                 offsetMatrices; // inverse-bind pose matrix
        std::vector<std::vector<VertexWeight>> weights;
    };

private:
    //std::string m_name;

    //std::vector<std::string> m_boneNames;
    //std::vector<GPM::Mat4>   m_relativeBones;
    //std::vector<size_t>      m_parentBoneIndices;
    //std::vector<size_t>      m_childrenBoneIndices;


 //   std::map<std::string, GPE::Mesh::BoneInfo> m_BoneInfoMap; 
	//size_t m_BoneCounter = 0;

public:
    Skeleton()                      = default;
    Skeleton(const Skeleton& other) = default;
    Skeleton(Skeleton&& other)      = default;
    ~Skeleton()                     = default;
    Skeleton& operator=(Skeleton const& other) = default;
    Skeleton& operator=(Skeleton&& other) = default;

    // void printHierarchy() const;

    // int getParentBoneIndex(int currentBone)
    //{
    //    return m_parentBoneIndices[currentBone];
    //}

    // GPM::Mat4& getRelativeBoneTransform(int boneIndex)
    //{
    //    return m_relativeBones[boneIndex];
    //}

    //GPM::Mat4& getInversedWorldBoneTransform(int boneIndex)
    //{
    //    return m_inversedWorldBones[boneIndex];
    //}

    //GETTER_BY_REF(Name, m_name);

    //static void setVertexBoneDataToDefault(GPE::Mesh::Vertex& vertex)
    //{
    //    for (int i = 0; i < GPE::Mesh::Vertex::maxBoneInfluence; i++)
    //    {
    //        vertex.boneIDs[i]   = -1;
    //        vertex.weights[i] = 0.0f;
    //    }
    //}

    //void SetVertexBoneData(GPE::Mesh::Vertex& vertex, int boneID, float weight)
    //{
    //    for (int i = 0; i < GPE::Mesh::Vertex::maxBoneInfluence; ++i)
    //    {
    //        if (vertex.boneIDs[i] < 0)
    //        {
    //            vertex.weights[i] = weight;
    //            vertex.boneIDs[i] = boneID;
    //            break;
    //        }
    //    }
    //}

    //void ExtractBoneWeightForVertices(std::vector<GPE::Mesh::Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
    //{
    //    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    //    {
    //        int         boneID   = -1;
    //        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
    //        if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
    //        {
    //            GPE::Mesh::BoneInfo newBoneInfo;
    //            newBoneInfo.id     = m_BoneCounter;
    //            newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
    //            m_BoneInfoMap[boneName] = newBoneInfo;
    //            boneID                  = m_BoneCounter;
    //            m_BoneCounter++;
    //        }
    //        else
    //        {
    //            boneID = m_BoneInfoMap[boneName].id;
    //        }
    //        assert(boneID != -1);
    //        auto weights    = mesh->mBones[boneIndex]->mWeights;
    //        int  numWeights = mesh->mBones[boneIndex]->mNumWeights;

    //        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
    //        {
    //            int   vertexId = weights[weightIndex].mVertexId;
    //            float weight   = weights[weightIndex].mWeight;
    //            assert(vertexId <= vertices.size());
    //            SetVertexBoneData(vertices[vertexId], boneID, weight);
    //        }
    //    }
    //}

    //Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    //{
    //    std::vector vertices;

    //    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    //    {
    //        Vertex vertex;

    //        SetVertexBoneDataToDefault(vertex);

    //        vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
    //        vertex.Normal   = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);

    //        if (mesh->mTextureCoords[0])
    //        {
    //            glm::vec2 vec;
    //            vec.x            = mesh->mTextureCoords[0][i].x;
    //            vec.y            = mesh->mTextureCoords[0][i].y;
    //            vertex.TexCoords = vec;
    //        }
    //        else
    //            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

    //        vertices.push_back(vertex);
    //    }
    //    ... ExtractBoneWeightForVertices(vertices, mesh, scene);

    //    return Mesh(vertices, indices, textures);
    //}
};

} /*namespace GPE*/
