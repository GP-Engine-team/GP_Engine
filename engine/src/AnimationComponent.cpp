#include "Engine/ECS/Component/AnimationComponent.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Resources/Animation.hpp"
#include "Engine/Resources/Skeleton.hpp"
#include <GPM/Calc.hpp>

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

void AnimationComponent::drawBlend(float currentTime, KeyFrame& buffer)
{
    if (nextAnim.anim == nullptr)
    {
        return currentAnim.anim->getKeyFrame(currentAnim.anim->getPlayTimeRatio(currentTime), buffer);
    }

    // The animations have different speeds.
    // This means that while blending, we also have to blend their speed.
    // This is done by modifying the time scale.
    float normalizedTimeScale = nextAnim.anim->duration / currentAnim.anim->duration;
    nextAnim.timeScale        = GPM::lerp(normalizedTimeScale, 1.f, alphaBlend);
    currentAnim.timeScale     = GPM::lerp(1.f, 1.f / normalizedTimeScale, alphaBlend);

    // Calculate the RenderFrame to Draw
    KeyFrame blendedKeyFrame = KeyFrame::Blend(currentAnim.GetKeyFrame(), nextAnim.GetKeyFrame(), alphaBlend);
    RenderFrameConstructor renderFrameConstructor = RenderFrameConstructor{currentAnim.skeleton, blendedKeyFrame};
    RenderFrame            renderFrame{std::move(renderFrameConstructor)};

    renderFrame.DrawSkeleton();
    renderFrame.DrawMesh();
}

}