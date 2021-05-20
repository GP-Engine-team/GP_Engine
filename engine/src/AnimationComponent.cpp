#include "Engine/ECS/Component/AnimationComponent.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Resources/Animation.hpp"
#include "Engine/Resources/Skeleton.hpp"
#include <GPM/Calc.hpp>
#include <GPM/conversion.hpp>

// Generated
#include <Generated/AnimationComponent.rfk.h>

File_GENERATED


namespace GPE
{

AnimationComponent::AnimationComponent()
{
    updateToSystem();
}

AnimationComponent::~AnimationComponent()
{
    setActive(false);
}

void AnimationComponent::onPostLoad()
{
    updateToSystem();
}

void AnimationComponent::updateToSystem()
{
    if (m_isActivated)
    {
        Engine::getInstance()->animSystem.addComponent(*this);
    }
    else
    {
        Engine::getInstance()->animSystem.removeComponent(*this);
    }
}

void AnimationComponent::setActive(bool newState) noexcept
{
    if (m_isActivated == newState)
        return;

    m_isActivated = newState;

    updateToSystem();
}

size_t AnimationComponent::getNbBones() const
{
    //if (skeleton == nullptr)
    //    return 0;
    //else
    //    return skeleton->getNbBones();
    return 0;
}

void AnimationComponent::updateRenderFrameBoneFromParent(size_t boneIndex)
{
    //if (!hasBeenUpdated[boneIndex])
    //{
    //    GPM::Mat4 localAnim = GPM::toTransform(blendedKeyFrame.bones[boneIndex]).model;

    //    int parentIndex = skeleton->getParentBoneIndex(boneIndex);
    //    if (parentIndex == -1)
    //    {
    //        // if there is no parent,  TRS = localAnimation *  localBindPose
    //        worldBonesTransform[boneIndex] = localAnim * skeleton->getRelativeBoneTransform(boneIndex);
    //    }
    //    else
    //    {
    //        // call update parent, so even if bones not in order, parent will always be calculated first
    //        updateRenderFrameBoneFromParent(parentIndex);
    //        // if there is no parent,  TRS = localAnimation *  localBindPose * parentTRS
    //        worldBonesTransform[boneIndex] = 
    //            localAnim * skeleton->getRelativeBoneTransform(boneIndex) * worldBonesTransform[parentIndex];
    //    }
    //    renderFrame.worldBonesOffsetsTransform[boneIndex] =
    //        skeleton->getInversedWorldBoneTransform(boneIndex) * worldBonesTransform[boneIndex];

    //    hasBeenUpdated[boneIndex] = true;
    //}
}

void AnimationComponent::updateRenderFrame()
{
    //size_t nbBones = getNbBones();

    //worldBonesTransform.resize(nbBones);
    //renderFrame.worldBonesOffsetsTransform.resize(nbBones);
    //hasBeenUpdated.resize(nbBones);

    //for (int boneIndex = 0; boneIndex < nbBones; boneIndex++)
    //    hasBeenUpdated[boneIndex] = false;

    //for (size_t boneIndex = 0; boneIndex < nbBones; boneIndex++)
    //    updateRenderFrameBoneFromParent(boneIndex);
}

//void AnimationComponent::drawBlend(float currentTime, KeyFrame& buffer, float alphaBlend)
//{
//    if (nextAnim.anim == nullptr)
//    {
//        return currentAnim.anim->getKeyFrame(currentAnim.anim->getPlayTimeRatio(currentTime), buffer);
//    }
//
//    // The animations have different speeds.
//    // This means that while blending, we also have to blend their speed.
//    // This is done by modifying the time scale.
//    float normalizedTimeScale = nextAnim.anim->duration / currentAnim.anim->duration;
//    nextAnim.timeScale        = GPM::lerp(normalizedTimeScale, 1.f, alphaBlend);
//    currentAnim.timeScale     = GPM::lerp(1.f, 1.f / normalizedTimeScale, alphaBlend);
//
//    // Calculate the RenderFrame to Draw
//    currentAnim.anim->getKeyFrame(currentTime, currentAnim.lastBlendedKeyFrame);
//    nextAnim.anim->getKeyFrame(currentTime, nextAnim.lastBlendedKeyFrame);
//    KeyFrame::blend(currentAnim.lastBlendedKeyFrame, nextAnim.lastBlendedKeyFrame, alphaBlend, blendedKeyFrame);
//}

//void AnimationComponent::drawDebugSkeleton(const GPM::Vec4& offset) const
//{
//    // for (int boneIndex = skeleton->firstDrawnBoneIndex; boneIndex <= skeleton->lastDrawnBoneIndex; boneIndex++)
//    for (size_t boneIndex = 0; boneIndex <= getNbBones(); boneIndex++)
//    {
//        int parentIndex = skeleton->getParentBoneIndex(boneIndex);
//        if (parentIndex != -1)
//        {
//            GPM::Mat4 transform       = worldBonesTransform[boneIndex];
//            GPM::Mat4 parentTransform = worldBonesTransform[parentIndex];
//
//            GPM::Vec4 p1 = transform * GPM::Vec4{0, 0, 0, 1} + offset;
//            GPM::Vec4 p2 = parentTransform * GPM::Vec4{0, 0, 0, 1} + offset;
//            GPE::Engine::getInstance()->sceneManager.getCurrentScene()->sceneRenderer.drawDebugLine(p1.xyz, p2.xyz);
//        }
//    }
//}

}