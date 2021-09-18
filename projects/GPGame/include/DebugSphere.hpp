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
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Color.hpp>
#include <memory>

#include "Generated/DebugSphere.rfk.h"

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) DebugSphere : public GPE::BehaviourComponent
    {
        RFKField(Inspect(), Serialize()) float          m_radius = 1.f;
        RFKField(Inspect(), Serialize()) GPE::ColorRGBA m_color  = GPE::ColorRGBA::green();

    public:
        DebugSphere(GPE::GameObject & owner) noexcept : GPE::BehaviourComponent(owner)
        {
        }

        DebugSphere() noexcept          = default;
        virtual ~DebugSphere() noexcept = default;

    public:
        virtual void start()
        {
            enableUpdateEditor(true);
        }

        void updateEditor(double deltaTime) final
        {
            drawDebugSphere(transform().getGlobalPosition(), m_radius, m_color, 0.f, true);
        }

        DebugSphere_GENERATED
    };

} // namespace )

File_GENERATED