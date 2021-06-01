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
}

AnimationComponent::~AnimationComponent() noexcept
{
    setActive(false);
}

void AnimationComponent::updateToSystem() noexcept
{
    if (m_isActivated)
    {
        if (m_model != nullptr)
        {
            m_model->setAnimComponent(this);
        }
        GPE::Engine::getInstance()->animSystem.addComponent(this);
    }
    else
    {
        if (m_model != nullptr)
        {
            m_model->setAnimComponent(nullptr);
        }
        GPE::Engine::getInstance()->animSystem.removeComponent(this);
    }
}

void AnimationComponent::update(float dt)
{
    if (m_currentAnimation)
    {
        m_currentTime += m_currentAnimation->getTicksPerSecond() * dt * m_timeScale;
        m_currentTime = fmod(m_currentTime, m_currentAnimation->getDuration());
        calculateBoneTransform(m_skeleton->getRoot(), GPM::Mat4::identity());
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
        bone->update(m_currentTime);
        nodeTransform = bone->getLocalTransform();
    }

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
    if (newSkeleton == nullptr || newSkeleton == m_skeleton)
        return;

    m_skeleton = newSkeleton;

    m_finalBoneMatrices.reserve(m_skeleton->getNbBones());

    for (int i = 0; i < m_skeleton->getNbBones(); i++)
        m_finalBoneMatrices.emplace_back(GPM::Mat4::identity());
}

void AnimationComponent::setModel(Model* newModel)
{
    if (m_model == newModel)
        return;

    if (m_model != nullptr)
    {
        m_model->setAnimComponent(nullptr);
    }

    if (newModel != nullptr)
    {
        newModel->setAnimComponent(this);
        if (m_skin != nullptr)
            newModel->bindSkin(*m_skin);
    }

    m_model = newModel;
}

void AnimationComponent::setSkin(Skin* skin)
{
    if (skin == nullptr)
    {
        m_skin = skin;
        return;
    }

    if (m_skin != skin)
    {
        m_skin = skin;
        if (m_model != nullptr)
            m_model->bindSkin(*m_skin);
    }
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
    setModel(&m_gameObject->getOrCreateComponent<GPE::Model>());

    if (m_skeleton != nullptr)
    {
        m_finalBoneMatrices.reserve(m_skeleton->getNbBones());
        for (int i = 0; i < m_skeleton->getNbBones(); i++)
            m_finalBoneMatrices.emplace_back(GPM::Mat4::identity());
    }

    if (m_model != nullptr)
        m_model->bindSkin(*m_skin);

    Component::onPostLoad();
}


bool AnimationComponent::isComplete() const
{
    return m_skeleton != nullptr && m_skin != nullptr && m_model != nullptr;
}