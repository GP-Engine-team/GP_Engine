/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Resources/Animation.hpp>
#include <Engine/Serialization/ComponentGen.h>

// Generated
#include <Generated/AnimationComponent.rfk.h>

namespace GPE RFKNamespace()
{
    class Animation;
    class Skeleton;

    class RFKClass(ComponentGen(), Serialize(), Inspect()) AnimationComponent : public Component
    {
    private:
        //struct AnimationBlend
        //{
        //    Animation* anim = nullptr;
        //    KeyFrame   lastBlendedKeyFrame; // Blended between 2 key frames from Animation. Cached.
        //    float      timeScale;
        //};

        //Skeleton* skeleton = nullptr;

        //KeyFrame       blendedKeyFrame; // Blended between 2 animations. Cached.
        //AnimationBlend currentAnim;
        //AnimationBlend nextAnim;

        //AnimRenderFrame renderFrame;

        //std::vector<GPM::Mat4> worldBonesTransform;
        //std::vector<bool>      hasBeenUpdated;

        //float currentTime = 0.f;
        //float timeScale   = 1.f;
        //float alphaBlend  = 0.f;

    private:
        void updateToSystem();

        void updateRenderFrame();
        void updateRenderFrameBoneFromParent(size_t boneIndex);

    public:
        AnimationComponent();
        AnimationComponent(GPE::GameObject& go)
        {
            m_gameObject = &go;
        }
        AnimationComponent(const AnimationComponent& other) = delete;
        AnimationComponent& operator=(AnimationComponent const& other) = delete;
        virtual ~AnimationComponent();
        AnimationComponent(AnimationComponent && other) = delete;
        AnimationComponent& operator=(AnimationComponent&& other) = delete;

        virtual void onPostLoad() override;
        virtual void setActive(bool newState) noexcept override;

        //void drawBlend(float currentTime, KeyFrame& buffer, float alphaBlend);

        size_t getNbBones() const;

        void drawDebugSkeleton(const GPM::Vec4& offset = GPM::Vec4{0, -100, 0}) const;

        AnimationComponent_GENERATED
    };
} // namespace )
