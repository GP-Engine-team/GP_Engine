/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <BasePlayer.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>

// Generated
#include <Generated/CameraOscilation.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) CameraOscilation : public GPE::BehaviourComponent
    {
    private:
        RFKField(Inspect(), Serialize()) float m_angle{45.f};
        RFKField(Inspect(), Serialize()) float m_speed{1.f};
        RFKField(Serialize()) float            m_delay{0.f};
        RFKField(Serialize()) GPM::Vec3        m_baseEulerRotation;

    public:
        CameraOscilation(GPE::GameObject & owner);
        CameraOscilation() noexcept = default;
        virtual ~CameraOscilation() = default;

        void start() override;
        void update(double deltaTime) override;
        void onPostLoad() override;

        CameraOscilation_GENERATED
    };

} // namespace )

File_GENERATED