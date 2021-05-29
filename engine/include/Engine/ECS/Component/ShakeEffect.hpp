/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>

#include "Generated/ShakeEffect.rfk.h"

namespace GPE RFKNamespace()
{
    /**
     * @brief Warning, shake immobilize the user during it's update
     */
    class RFKClass(ComponentGen, Serialize()) ShakeEffect : public BehaviourComponent
    {
    protected:
        RFKField(Serialize()) float m_durationCount; // The initial shake duration, set when ShakeCamera is called.
        RFKField(Serialize()) bool  m_isRunning = false;
        RFKField(Serialize())
            GPM::Vec3 m_basePosition; // Corresponde to the position of the entity befor the camera shake.

    public:
        RFKField(Serialize(), Inspect()) float shakeStrength = 3.f; // The amount to shake this frame.
        RFKField(Serialize(), Inspect()) float shakeDuration = 3.f; // In second

    public:
        ShakeEffect()          = default;
        virtual ~ShakeEffect() = default;

        void start();
        void stop();

        void update(double deltaTime) override;
        void updateEditor(double deltaTime) override;

        void inspect(InspectContext & context) override;

        ShakeEffect_GENERATED
    };

} // namespace )