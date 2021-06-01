/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/Model.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>
#include <Engine/Serialization/Slider.hpp>
#include <gpm/Quaternion.hpp>
#include <gpm/Vector3.hpp>
#include <gpm/constants.hpp>

#include "Firearm.hpp"

#include <Generated/PPSH41.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(ComponentGen(), Serialize(), Inspect()) PPSH41 : public Firearm
    {
    protected:
        // Recoile info
        RFKField(Serialize()) GPM::Vec3       m_finalPosition;
        RFKField(Serialize()) GPM::Vec3       m_basePosition;
        RFKField(Serialize()) GPM::Quaternion m_finalRotation;
        RFKField(Serialize()) GPM::Quaternion m_baseRotation;

        RFKField(Inspect(), Serialize(), Slider(0.01f, 0.6f)) float  m_knowbackDuration = 0.f; // [0, 1]
        RFKField(Inspect(), Serialize()) float                       m_knowbackStrength = 1.f;
        RFKField(Inspect(), Serialize(), Slider(0.f, HALF_PI)) float m_knowbackMaxAngle = HALF_PI / 2.f;

        RFKField(Inspect(), Serialize()) GPE::Linker<GPE::GameObject>        m_muzzleFlashGO;
        RFKField(Inspect(), Serialize()) GPE::Linker<GPE::ParticleComponent> m_smokeEffect;
        RFKField(Serialize()) float                                          m_muzzleFlashCount = 0.f;

        RFKField(Inspect(), Serialize()) float m_muzzleFlashDuration = 0.1f;
        RFKField(Inspect(), Serialize()) float m_muzzleFlashMinScale = 1.f;
        RFKField(Inspect(), Serialize()) float m_muzzleFlashMaxScale = 4.f;

    public:
        PPSH41(GPE::GameObject & owner) noexcept;

        PPSH41() noexcept          = default;
        virtual ~PPSH41() noexcept = default;

        void onShoot() final;
        void animateRecoil(float t) final;

        void start() override;
        void update(double deltaTime) override;

        PPSH41_GENERATED
    };
} // namespace )
