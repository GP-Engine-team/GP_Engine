/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/Model.hpp>
#include <Engine/Resources/Animation/Bone.hpp>
#include <Engine/Resources/Importer/AssimpUtilities.hpp>
#include <Engine/Resources/Mesh.hpp>
#include <GPM/Matrix4.hpp>
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure
#include <map>
#include <string>
#include <vector>

namespace GPE
{

struct AssimpNodeData
{
    GPM::Matrix4                transformation;
    std::string                 name;
    int                         childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
private:
    float                                      m_duration;
    int                                        m_ticksPerSecond;
    std::vector<Bone>                          m_bones;
    AssimpNodeData                             m_root;
    //std::map<std::string, GPE::Model::BoneInfo> m_boneInfoMap;

    void readMissingBones(const aiAnimation* animation, class Skeleton& skeleton);
    void readHierarchyData(AssimpNodeData& dest, const aiNode* src);

public:
    Animation() = default;

    Animation(const std::string& animationPath, Skeleton& skeleton);
    Bone* findBone(const std::string& name);

    //size_t getNbBones() const
    //{
    //    return 100;
    //}

public:
    GETTER_BY_VALUE(TicksPerSecond, m_ticksPerSecond);
    GETTER_BY_VALUE(Duration, m_duration);
    GETTER_BY_CONST_REF(Root, m_root);
    //GETTER_BY_CONST_REF(BoneInfoMap, m_boneInfoMap);
};

} // namespace GPE