/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/ECS/Component/AnimationComponent.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Resources/Animation/Skeleton.hpp>
#include <Engine/Resources/Animation/Animation.hpp>
#include <Engine/Resources/Animation/Skin.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

// Generated
#include <Generated/AnimationComponent.rfk.h>
File_GENERATED

    using namespace GPE;

AnimationComponent::AnimationComponent(GameObject& owner) noexcept : Component(owner)
{
    updateToSystem();

    m_finalBoneMatrices.reserve(m_skeleton->getNbBones());
    for (int i = 0; i < m_skeleton->getNbBones(); i++)
        m_finalBoneMatrices.emplace_back(GPM::Mat4::identity());
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

void AnimationComponent::update(float deltaTime)
{
    if (m_currentAnimation && isComplete())
    {
        m_currentTime += deltaTime * m_timeScale;
        if ((!shouldLoop) && m_currentTime >= m_currentAnimation->getDuration())
        {
            // TODO : Keep last anim frame
            m_currentAnimation = nullptr;
            return;
        }

        m_currentTime = fmod(m_currentTime, m_currentAnimation->getDuration());
        calculateBoneTransform(m_skeleton->getRoot(), GPM::Mat4::identity());

        //if (m_nextAnimation != nullptr)
        //{
        //    m_nextAnimTime += m_nextAnimation->getTicksPerSecond() * dt * m_timeScale;
        //    m_nextAnimTime = fmod(m_nextAnimTime, m_nextAnimation->getNbTicks());
        //}
    }
}

void AnimationComponent::removeAnimData()
{
    m_model->setAnimComponent(nullptr);
    //m_finalBoneMatrices.clear();
}

void AnimationComponent::updateAnimData(bool wasComplete)
{
    if (!wasComplete && isComplete())
    {
        // Skeleton
        m_finalBoneMatrices.reserve(m_skeleton->getNbBones());
        for (int i = 0; i < m_skeleton->getNbBones(); i++)
            m_finalBoneMatrices.emplace_back(GPM::Mat4::identity());

        // Model
        m_model->setAnimComponent(this);

        // Skin
        m_model->bindSkin(*m_skin);
    }

}

void AnimationComponent::playAnimation(Animation* pAnimation)
{
    GPE_ASSERT(isComplete(), "Animation Component data should be set before playing an animation.");
    if (!isComplete())
        return;

    m_currentAnimation = pAnimation;
    m_currentTime      = 0.0f;
}

void AnimationComponent::calculateBoneTransform(const AssimpNodeData& node, const GPM::mat4& parentTransform)
{
    GPM::Mat4          nodeTransform = node.transformation;

    Bone* bone = m_currentAnimation->findBone(node.name);

    if (bone)
    {
        bone->update(m_currentTime * 1000.f /* to milliseconds */);
        nodeTransform = bone->getLocalTransform();
    }

    //if (m_nextAnimation != nullptr)
    //{
    //    Bone* newAnimBone = m_nextAnimation->findBone(node.name);

    //    if (newAnimBone)
    //    {
    //        newAnimBone->update(m_currentTime);
    //        nodeTransform = newAnimBone->getLocalTransform();
    //    }
    //}

    GPM::Mat4 globalTransformation = parentTransform * nodeTransform;

    auto& boneInfoMap = m_skeleton->m_boneInfoMap;
    auto  it          = boneInfoMap.find(node.name);
    if (it != boneInfoMap.end())
    {
        int       index            = it->second.id;
        GPM::Mat4 offset           = it->second.offset;
        m_finalBoneMatrices[index] = globalTransformation * offset;
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
    m_skeleton = newSkeleton;
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
    m_model = newModel;
    updateAnimData(wasComplete);
}

void AnimationComponent::setSkin(Skin* skin)
{
    if (skin == m_skin)
        return;

    if (isComplete() && m_skin == nullptr)
    {
        removeAnimData();
    }

    bool wasComplete = isComplete();
    m_skin = skin;
    updateAnimData(wasComplete);
}

 void AnimationComponent::drawDebugSkeleton(const GPM::Vec4& offset) const
{
    //// for (int boneIndex = skeleton->firstDrawnBoneIndex; boneIndex <= skeleton->lastDrawnBoneIndex; boneIndex++)
    //for (size_t boneIndex = 0; boneIndex <= m_skeleton->getNbBones(); boneIndex++)
    //{
    //    int parentIndex = m_skeleton->getParentBoneIndex(boneIndex);
    //    if (parentIndex != -1)
    //    {
    //        GPM::Mat4 transform       = worldBonesTransform[boneIndex];
    //        GPM::Mat4 parentTransform = worldBonesTransform[parentIndex];

    //        GPM::Vec4 p1 = transform * GPM::Vec4{0, 0, 0, 1} + offset;
    //        GPM::Vec4 p2 = parentTransform * GPM::Vec4{0, 0, 0, 1} + offset;
    //        GPE::Engine::getInstance()->sceneManager.getCurrentScene()->sceneRenderer.drawDebugLine(p1.xyz, p2.xyz);
    //    }
    //}
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