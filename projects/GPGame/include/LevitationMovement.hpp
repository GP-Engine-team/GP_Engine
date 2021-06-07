/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <BasePlayer.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>

// Generated
#include <Generated/LevitationMovement.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) LevitationMovement : public GPE::BehaviourComponent
    {
    private:
        RFKField(Inspect(), Serialize()) float              m_radius = 0.5f;
        RFKField(Inspect(), Serialize(), Separator()) float m_rotationSpeed{1.f};
        RFKField(Inspect(), Serialize(), Separator()) float m_speed{1.f};
        RFKField(Inspect(), Serialize()) float              m_heightIntensity{1.f};
        RFKField(Inspect(), Serialize()) float              m_delay{0.f};
        RFKField(Inspect(), Serialize()) float              m_initialY{0.f};

    public:
        LevitationMovement(GPE::GameObject & owner);
        LevitationMovement() noexcept = default;
        virtual ~LevitationMovement() = default;

        void start() override;
        void update(double deltaTime) override;
        void onPostLoad() override;

        LevitationMovement_GENERATED
    };

} // namespace )