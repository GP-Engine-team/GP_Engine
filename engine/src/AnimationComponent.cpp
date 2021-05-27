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

void AnimationComponent::setupAnims(bool newValue)
{
    if (debugAnimUpdateCallback != newValue)
    {
        debugAnimUpdateCallback = newValue;

        // TODO : To move in Importer.cpp
        // Load anim data
        unsigned int postProcessflags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                                        aiProcess_SortByPType | aiProcess_GenNormals | aiProcess_GenUVCoords |
                                        aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
                                        aiProcess_LimitBoneWeights;

        const char* srcPath = "C:\\Users\\Utilisateur\\Downloads\\GP_Engine - Copy2\\GP_Engine - Copy\\projects\\GPGame\\resources\\Character\\Taunt.fbx";

        Assimp::Importer importer;
        const aiScene*   scene = importer.ReadFile(srcPath, postProcessflags);
        if (!scene)
        {
            FUNCT_ERROR(importer.GetErrorString());
            return;
        }

        // subModel
        m_model = m_gameObject->getComponent<GPE::Model>();
        m_model->setAnimComponent(this);

        // Mesh
        for (size_t i = 0; i < scene->mNumMeshes; ++i)
        {
            aiMesh* pMesh = scene->mMeshes[i];

            // Skin / Skeleton
            // TODO : Delete / LEAKS
            m_skin     = new Skin();
            Skeleton* skeleton = new Skeleton();
            skeleton->readHierarchyData(scene->mRootNode);
            loadSkinAndSkeleton(*m_skin, *skeleton, pMesh);
            setSkeleton(skeleton);
            m_model->bindSkin(*m_skin);
        }

        // animation
        // TODO : Delete / LEAKS
        m_currentAnimation = new Animation(scene->mAnimations[0], *m_skeleton);
        playAnimation(m_currentAnimation);

    }
}

void AnimationComponent::setSkeleton(Skeleton* newSkeleton)
{
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
    }

    m_model = newModel;
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