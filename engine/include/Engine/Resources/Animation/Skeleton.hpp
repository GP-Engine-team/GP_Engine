/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Core/Tools/ClassUtility.hpp>
#include <GPM/Matrix4.hpp>
#include <map>
#include <string>
#include <vector>
#include <Engine/Serialization/binary/BinaryLoader.hpp>
#include <Engine/Serialization/binary/BinarySaver.hpp>

struct aiNode;

namespace GPE
{
struct AssimpNodeData
{
    /**
     * @brief The transformation relative to the node's parent.
     */
    GPM::Matrix4                transformation;
    std::string                 name;
    std::vector<AssimpNodeData> children;
};

template <>
inline void load(BinaryLoader& context, AssimpNodeData& loaded, const BinaryLoader::LoadInfo* info)
{
    GPE::load(context, loaded.transformation, nullptr);
    GPE::load(context, loaded.name, nullptr);
    GPE::load(context, loaded.children, nullptr);
}

template <>
inline void save(BinarySaver& context, const AssimpNodeData& saved, const BinarySaver::SaveInfo* info)
{
    GPE::save(context, saved.transformation, nullptr);
    GPE::save(context, saved.name, nullptr);
    GPE::save(context, saved.children, nullptr);
}

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

    struct CreateArgs
    {
        std::map<std::string, BoneInfo> m_boneInfoMap; 
        AssimpNodeData m_root;
    };

    // Animation data
    std::map<std::string, BoneInfo> m_boneInfoMap; 
    int                             m_boneCounter = 0;

    AssimpNodeData m_root;

public:
    void        readHierarchyData(AssimpNodeData& dest, const aiNode* src);
    inline void readHierarchyData(const aiNode* src)
    {
        readHierarchyData(m_root, src);
    }

    inline size_t getNbBones() const
    {
        return m_boneInfoMap.size();
    }

    GETTER_BY_CONST_REF(Root, m_root);
};

} // namespace GPE