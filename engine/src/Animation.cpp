/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/Resources/Animation/Animation.hpp>

using namespace GPE;

Animation::Animation(const std::string& animationPath, Mesh* mesh)
{
    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    auto animation   = scene->mAnimations[0];
    m_duration       = animation->mDuration;
    m_ticksPerSecond = animation->mTicksPerSecond;
    readHierarchyData(m_root, scene->mRootNode);
    readMissingBones(animation, *mesh);
}

void Animation::readMissingBones(const aiAnimation* animation, Mesh& mesh)
{
    int size = animation->mNumChannels;

    auto& boneInfoMap = mesh.m_boneInfoMap; // getting m_boneInfoMap from Model class
    int&  boneCount   = mesh.m_boneCounter; // getting the m_BoneCounter from Model class

    // reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        auto        channel  = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        m_bones.push_back(Bone{std::string(channel->mNodeName.data), boneInfoMap[channel->mNodeName.data].id, channel});
    }

    m_boneInfoMap = boneInfoMap;
}

void Animation::readHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
    assert(src);

    dest.name           = src->mName.data;
    dest.transformation = GPE::toMat4(src->mTransformation);
    dest.childrenCount  = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        readHierarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

Bone* Animation::findBone(const std::string& name)
{
    auto iter = std::find_if(m_bones.begin(), m_bones.end(), [&](const Bone& bone) { return bone.getName() == name; });
    if (iter == m_bones.end())
        return nullptr;
    else
        return &(*iter);
}