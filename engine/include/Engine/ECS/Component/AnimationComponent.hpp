/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Serialization/ComponentGen.h>
#include <Engine/Resources/Animation.hpp>

// Generated
#include <Generated/AnimationComponent.rfk.h>

namespace GPE RFKNamespace()
{
    class Animation;
    class Skeleton;

    class RFKClass(ComponentGen(), Serialize(), Inspect()) AnimationComponent : public Component
    {
    private:
        struct AnimationBlend
        {
            Animation* anim = nullptr;
            float      timeScale;

        };

        Skeleton*  skeleton    = nullptr;

        AnimationBlend currentAnim;
        AnimationBlend nextAnim;

        float currentTime  = 0.f;
        float timeScale    = 1.f;
        float alphaBlend   = 0.f;

    private:
        void updateToSystem();

    public:
         AnimationComponent();
         AnimationComponent(const AnimationComponent& other) = delete;
         AnimationComponent& operator=(AnimationComponent const& other) = delete;
         virtual ~AnimationComponent();
         AnimationComponent(AnimationComponent&& other) = delete;
         AnimationComponent& operator=(AnimationComponent&& other) = delete;

        virtual void onPostLoad() override;
        virtual void setActive(bool newState) override;

        void drawBlend(float currentTime, KeyFrame& buffer);

        AnimationComponent_GENERATED
    };
} // namespace )
