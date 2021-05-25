/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/ECS/Component/AnimationComponent.hpp>
#include <Engine/Engine.hpp>

// Generated
#include <Generated/AnimationComponent.rfk.h>
File_GENERATED

using namespace GPE;

AnimationComponent::AnimationComponent(GameObject& owner) noexcept : Component(owner)
{
    updateToSystem();
}

AnimationComponent::~AnimationComponent() noexcept
{
    setActive(false);
}

void AnimationComponent::updateToSystem() noexcept
{
    if (m_isActivated)
    {
        GPE::Engine::getInstance()->animSystem.addComponent(this);
    }
    else
    {
        GPE::Engine::getInstance()->animSystem.removeComponent(this);
    }
}

void AnimationComponent::update(float dt)
{
    if (m_currentAnimation)
    {
        m_currentTime += m_currentAnimation->getTicksPerSecond() * dt * m_timeScale;
        m_currentTime = fmod(m_currentTime, m_currentAnimation->getDuration());
        calculateBoneTransform(&m_currentAnimation->getRoot(), GPM::Mat4::identity());
    }
}

void AnimationComponent::playAnimation(Animation* pAnimation)
{
    m_currentAnimation = pAnimation;
    m_currentTime      = 0.0f;

    m_finalBoneMatrices.reserve(pAnimation->getNbBones());

    for (int i = 0; i < pAnimation->getNbBones(); i++)
        m_finalBoneMatrices.emplace_back(GPM::Mat4::identity());
}

void AnimationComponent::calculateBoneTransform(const AssimpNodeData* node, const GPM::mat4& parentTransform)
{
    std::string nodeName      = node->name;
    GPM::Mat4   nodeTransform = node->transformation;

    Bone* bone = m_currentAnimation->findBone(nodeName);

    if (bone)
    {
        bone->update(m_currentTime);
        nodeTransform = bone->getLocalTransform();
    }

    GPM::Mat4 globalTransformation = parentTransform * nodeTransform;

    auto boneInfoMap = m_currentAnimation->getBoneInfoMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int       index            = boneInfoMap[nodeName].id;
        GPM::Mat4 offset           = boneInfoMap[nodeName].offset;
        m_finalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
        calculateBoneTransform(&node->children[i], globalTransformation);
}