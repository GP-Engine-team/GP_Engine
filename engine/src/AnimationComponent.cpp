/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Animation/Animation.hpp>
#include <Engine/Resources/Animation/Skeleton.hpp>
#include <Engine/Resources/Animation/Skin.hpp>
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include <Generated/AnimationComponent.cpp.h>

    using namespace GPE;

AnimationComponent::AnimationComponent(GameObject& owner) noexcept : Component(owner)
{
    updateToSystem();

    if (m_skeleton != nullptr)
    {
        finalBoneMatrices.clear();
        finalBoneMatrices.reserve(m_skeleton->getNbBones());
        for (int i = 0; i < m_skeleton->getNbBones(); i++)
            finalBoneMatrices.emplace_back(GPM::Mat4::identity());
    }
}

AnimationComponent::~AnimationComponent() noexcept
{
    setSkeleton(nullptr);
    setModel(nullptr);
    setSkin(nullptr);
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

void AnimationComponent::setSubModelIndex(int newSubModelIndex)
{
    if (m_model == nullptr)
    {
        m_subModelIndex = -1;
        return;
    }

    if (m_subModelIndex == newSubModelIndex)
        return;

    if (newSubModelIndex != -1)
    {
        if (m_model->canAssignAnimComponent(newSubModelIndex))
        {
            if (m_subModelIndex != -1)
            {
                removeAnimData();
            }
            m_subModelIndex = newSubModelIndex;
            updateAnimData(false);
        }
    }
    else
    {
        if (m_subModelIndex != -1)
        {
            removeAnimData();
        }
        m_subModelIndex = newSubModelIndex;
    }
}

void AnimationComponent::setNextAnimAsCurrent()
{
    m_currentAnimation    = m_nextAnimation;
    m_currentTime         = m_nextAnimTime;
    shouldLoop            = shouldNextLoop;
    m_timeScale           = m_nextTimeScale;
    m_nextAnimTime        = 0.f;
    m_nextAnimation       = nullptr;
    shouldNextLoop        = true;
    m_nextTimeScale       = 1.f;
    m_currentAnimCacheKey = m_nextAnimCacheKey;
}

void AnimationComponent::update(float deltaTime)
{
    if (m_currentAnimation && isComplete())
    {
        if (m_nextAnimation != nullptr)
        {
            m_nextAnimTime += deltaTime * m_nextTimeScale;
            if (m_nextAnimTime >= m_blendTime)
            {
                setNextAnimAsCurrent();
            }
        }

        m_currentTime += deltaTime * m_timeScale;
        if ((!shouldLoop) && m_currentTime >= m_currentAnimation->getDuration())
        {
            // TODO : Keep last anim frame
            if (m_nextAnimation != nullptr)
            {
                setNextAnimAsCurrent();
            }
            else
            {
                m_currentAnimation = nullptr;
            }
            return;
        }

        m_currentTime = fmod(m_currentTime, m_currentAnimation->getDuration());
        calculateBoneTransform(m_skeleton->getRoot(), GPM::Mat4::identity());
    }
}

void AnimationComponent::setNextAnim(Animation* nextAnim, float blendTime, float nextTimeScale)
{
    if (m_currentAnimation == nextAnim || m_nextAnimation == nextAnim)
        return;

    if (m_currentAnimation == nullptr)
    {
        playAnimation(nextAnim, 0.f, nextTimeScale);
    }
    else if (m_nextAnimation == nullptr)
    {
        m_nextAnimation    = nextAnim;
        m_blendTime        = blendTime;
        m_nextTimeScale    = nextTimeScale;
        m_nextAnimCacheKey = GPE::Engine::getInstance()->animSystem.getOrCreateSkeletonBoneIDToAnimationBoneID(
            *m_currentAnimation, *m_skeleton);
    }
}

void AnimationComponent::removeAnimData()
{
    m_model->setAnimComponent(nullptr, m_subModelIndex);
}

void AnimationComponent::updateAnimData(bool wasComplete)
{
    if (!wasComplete && isComplete())
    {
        // Skeleton
        finalBoneMatrices.clear();
        finalBoneMatrices.reserve(m_skeleton->getNbBones());
        for (int i = 0; i < m_skeleton->getNbBones(); i++)
            finalBoneMatrices.emplace_back(GPM::Mat4::identity());

        // Model
        m_model->setAnimComponent(this, m_subModelIndex);

        // Skin
        m_model->bindSkin(*m_skin, m_subModelIndex);

        Animation* anim    = m_currentAnimation;
        m_currentAnimation = nullptr;
        playAnimation(anim);
    }
}

void AnimationComponent::playAnimation(Animation* pAnimation, float startTime, float nextTimeScale)
{
    if (m_currentAnimation == pAnimation)
        return;

    if (pAnimation == nullptr)
    {
        m_currentAnimation = pAnimation;
        return;
    }

    if (!isComplete())
        return;

    m_currentAnimation = pAnimation;
    m_currentTime      = startTime;
    m_timeScale        = nextTimeScale;

    m_currentAnimCacheKey = GPE::Engine::getInstance()->animSystem.getOrCreateSkeletonBoneIDToAnimationBoneID(
        *m_currentAnimation, *m_skeleton);
}

void AnimationComponent::setCurrentTime(float newTime)
{
    m_currentTime = fmod(newTime, m_currentAnimation->getDuration());
}

void AnimationComponent::calculateBoneTransform(const AssimpNodeData& node, const GPM::mat4& parentTransform)
{
    GPM::Mat4 nodeTransform = node.transformation;

    // Bone* bone = m_currentAnimation->findBone(node.name);
    if (node.boneID < m_currentAnimCacheKey->second.size() &&
        m_currentAnimCacheKey->second[node.boneID] < m_currentAnimation->m_bones.size())
    {

        Bone* bone = &m_currentAnimation->m_bones[m_currentAnimCacheKey->second[node.boneID]];

        if (bone)
        {
            if (m_nextAnimation == nullptr)
            {
                bone->update(m_currentTime * 1000.f /* to milliseconds */);
                nodeTransform = bone->getLocalTransform();
            }
            else
            {
                Bone* otherBone = &m_nextAnimation->m_bones[m_nextAnimCacheKey->second[node.boneID]];

                float           currentTimeMs = m_currentTime * 1000.f; /* to milliseconds */
                const GPM::Vec3 translation1  = bone->getInterpolatedPosition(currentTimeMs);
                const GPM::Quat rotation1     = bone->getInterpolatedRotation(currentTimeMs);
                const GPM::Vec3 scale1        = bone->getInterpolatedScale(currentTimeMs);

                float nextAnimTimeMs =
                    fmod(m_nextAnimTime, m_nextAnimation->getDuration()) * 1000.f; /* to milliseconds */
                const GPM::Vec3 translation2 = otherBone->getInterpolatedPosition(nextAnimTimeMs);
                const GPM::Quat rotation2    = otherBone->getInterpolatedRotation(nextAnimTimeMs);
                const GPM::Vec3 scale2       = otherBone->getInterpolatedScale(nextAnimTimeMs);

                float           alpha       = m_nextAnimTime / m_blendTime;
                const GPM::Mat4 translation = GPM::Transform::translation(translation1.lerp(translation2, alpha));
                const GPM::Mat4 rotation    = GPM::toMatrix4(rotation1.nlerp(rotation2, alpha));
                const GPM::Mat4 scale       = GPM::Transform::scaling(scale1.lerp(scale2, alpha));

                nodeTransform = translation * rotation * scale;
            }
        }
    }

    GPM::Mat4 globalTransformation = parentTransform * nodeTransform;

    if (node.boneID >= 0 && node.boneID < m_skeleton->m_boneInfo.size())
    {
        GPM::Mat4 offset = m_skeleton->m_boneInfo[node.boneID].offset;
        if (node.boneID < finalBoneMatrices.size())
            finalBoneMatrices[node.boneID] = globalTransformation * offset;
    }

    for (const GPE::AssimpNodeData& node : node.children)
    {
        calculateBoneTransform(node, globalTransformation);
    }
}

void AnimationComponent::setSkeleton(Skeleton* newSkeleton)
{
    if (newSkeleton == m_skeleton)
        return;

    if (isComplete() && newSkeleton == nullptr)
    {
        removeAnimData();
    }

    bool wasComplete = isComplete();
    m_skeleton       = newSkeleton;
    updateAnimData(wasComplete);
}

void AnimationComponent::setModel(Model* newModel)
{
    if (m_model == newModel)
        return;

    if (isComplete() && newModel == nullptr)
    {
        removeAnimData();
    }

    bool wasComplete = isComplete();
    m_model          = newModel;
    updateAnimData(wasComplete);
}

void AnimationComponent::setSkin(Skin* skin)
{
    if (skin == m_skin)
        return;

    if (isComplete() && skin == nullptr)
    {
        removeAnimData();
    }
    else
    {

        bool wasComplete = isComplete();
        m_skin           = skin;
        updateAnimData(false);
    }
}

void AnimationComponent::setCurrentAnimDuration(float newDuration)
{
    if (m_currentAnimation != nullptr)
    {
        m_timeScale = newDuration / m_currentAnimation->getDuration();
    }
}

void AnimationComponent::onPostLoad()
{
    m_model = &m_gameObject->getOrCreateComponent<GPE::Model>();
    updateAnimData(false);

    Component::onPostLoad();
}

bool AnimationComponent::isComplete() const
{
    return m_skeleton != nullptr && m_skin != nullptr && m_model != nullptr && m_skeleton->getNbBones() > 0;
}