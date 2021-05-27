/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <GPM/Matrix4.hpp>
#include <map>
#include <string>
#include <vector>

namespace GPE
{

class Skeleton
{
public:
    struct BoneInfo
    {
        /**
         * @brief The index of the bone. Used by animations to identify the correct bone.
        */
        int id;

        /*offset matrix transforms vertex from model space to bone space*/
        GPM::mat4 offset;
    };

    // Animation data
    std::map<std::string, BoneInfo> m_boneInfoMap; //
    int                             m_boneCounter = 0;
    //std::vector<VertexBoneData>     m_verticesBoneData; // contains the weights of each vertex

public:
    size_t getNbBones() const
    {
        return m_boneInfoMap.size();
    }
};

} // namespace GPE