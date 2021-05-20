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
    std::string m_name;

    std::vector<std::string> m_boneNames;
    std::vector<GPM::Mat4>   m_relativeBones;
    std::vector<size_t>      m_parentBoneIndices;
    std::vector<size_t>      m_childrenBoneIndices;

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

    GETTER_BY_REF(Name, m_name);
};

} /*namespace GPE*/
