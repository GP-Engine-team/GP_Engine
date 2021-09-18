/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/Prefab.hpp>
#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/Inspect.hpp>

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/ParticleComponent.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Color.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <memory>

#include "Generated/EvacuationPoint.rfk.h"

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) EvacuationPoint : public GPE::BehaviourComponent
    {
        RFKField(Inspect(), Serialize()) float             m_zoneRadius   = 1.f;
        RFKField(Inspect(), Serialize()) float             m_timeDuration = 1.f;
        RFKField(Inspect(), Serialize(), ReadOnly()) float m_timeCount    = 0.f;

        RFKField(Inspect(), Serialize()) GPE::Linker<GPE::ParticleComponent> m_zoneParticle;
        RFKField() class BasePlayer*                                         m_player = nullptr;

    public:
        EvacuationPoint(GPE::GameObject & owner) noexcept : GPE::BehaviourComponent(owner)
        {
        }

        EvacuationPoint() noexcept          = default;
        virtual ~EvacuationPoint() noexcept = default;

    public:
        void start() override;
        void update(double deltaTime) override;
        void activeEvacutaitonPoint();

        GETTER_BY_VALUE(Timer, m_timeCount)
        GETTER_BY_VALUE(TimerDuration, m_timeDuration)

        EvacuationPoint_GENERATED
    };

} // namespace )

File_GENERATED