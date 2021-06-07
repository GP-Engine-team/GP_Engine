/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/ECS/Component/AnimationComponent.hpp>
#include <Engine/ECS/System/AnimationSystem.hpp>
#include <Engine/Resources/Animation/Animation.hpp>
#include <Engine/Resources/Animation/Skeleton.hpp>

using namespace GPE;

void AnimationSystem::update(double deltaTime) noexcept
{
    for (AnimationComponent* comp : components)
    {
        comp->update(float(deltaTime));
    }
}

void AnimationSystem::addComponent(class AnimationComponent* animationComponent) noexcept
{
    components.push_back(animationComponent);
}
void AnimationSystem::removeComponent(class AnimationComponent* animationComponent) noexcept
{
    auto it = std::find(components.begin(), components.end(), animationComponent);
    if (it != components.end())
    {
        std::swap(components.back(), *it);
        components.pop_back();
    }
}

AnimationSystem::CacheKey AnimationSystem::getOrCreateSkeletonBoneIDToAnimationBoneID(const Animation& anim,
                                                                                       Skeleton&       skeleton)
{
    // Can be optimized by using std::lower_bound and emplace_hint instad of find and emplace.
    auto pair = std::make_pair(anim.m_path, skeleton.m_path);
    auto it   = skeletonBoneIDToAnimationBoneID.find(pair);
    if (it == skeletonBoneIDToAnimationBoneID.end())
    {
        std::vector<size_t> boneIDtoAnimationBoneID;

        boneIDtoAnimationBoneID.resize(skeleton.getNbBones());

        auto getBoneName = [](const std::string& fullName) {
            size_t delimiter = fullName.find(":");
            if (delimiter == fullName.npos)
                return fullName;
            else
            {
                return fullName.substr(delimiter + 1);
            }
        };

        skeleton.forEachAssimpNodeData(skeleton.m_root, [&](AssimpNodeData& node) {
            auto it = std::find_if(anim.m_bones.begin(), anim.m_bones.end(), [&](const Bone& bone) {
                return getBoneName(bone.getName()) == getBoneName(node.name);
            });
            if (it != anim.m_bones.end())
            {
                boneIDtoAnimationBoneID[node.boneID] = it - anim.m_bones.begin();
            }
            else
            {
                boneIDtoAnimationBoneID[node.boneID] = skeleton.getNbBones(); // These bones won't be animated
            }
        });

        return skeletonBoneIDToAnimationBoneID.emplace(pair, std::move(boneIDtoAnimationBoneID)).first;
    }
    else
    {
        return it;
    }
}

void AnimationSystem::clearCache()
{
    skeletonBoneIDToAnimationBoneID.clear();
}