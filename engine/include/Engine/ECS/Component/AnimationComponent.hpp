/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/Component.hpp>
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
        struct AnimationBlend
        {
            Animation* anim = nullptr;
        };

        Skeleton*  skeleton  = nullptr;
        std::vector<AnimationBlend> animations;
        
        float currentTime  = 0.f;
        float animDuration = 5.f;
        float timeScale    = 1.f;

    public:
        //AnimationComponent();
        //AnimationComponent(const AnimationComponent& other) = delete;
        //AnimationComponent& operator=(AnimationComponent const& other) = delete;
        //virtual ~AnimationComponent();
        //AnimationComponent(AnimationComponent && other);
        //AnimationComponent& operator=(AnimationComponent&& other);

        ///**
        // * @brief Add or remove current component from it's system which have for effect to enable or disable it
        // * @param newState
        // * @return
        // */
        //void setActive(bool newState) noexcept override;

        //virtual void awake() override;

        AnimationComponent_GENERATED
    };
} // namespace )
