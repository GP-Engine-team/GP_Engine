/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/Resources/Animation/Animation.hpp>
#include <Engine/Resources/Animation/Skeleton.hpp>

using namespace GPE;

Animation::Animation(const aiAnimation* aiAnim)
{
    m_duration       = aiAnim->mDuration;
    m_ticksPerSecond = aiAnim->mTicksPerSecond;
    readMissingBones(aiAnim);
}

Animation::Animation(const CreateArgs& args)
{
    m_bones = args.bones;
    m_duration       = args.duration;
    m_ticksPerSecond = args.nbTicksPerSecond;
}

Animation::CreateArgs::CreateArgs(const aiAnimation* aiAnim)
{
    duration         = aiAnim->mDuration;
    nbTicksPerSecond = aiAnim->mTicksPerSecond;

    int size = aiAnim->mNumChannels;

    // reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        aiNodeAnim* channel  = aiAnim->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        bones.push_back(Bone{std::string(channel->mNodeName.data), channel});
    }
}

void Animation::readMissingBones(const aiAnimation* animation)
{
    int size = animation->mNumChannels;

    // reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        aiNodeAnim* channel  = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        m_bones.push_back(Bone{std::string(channel->mNodeName.data), channel});
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