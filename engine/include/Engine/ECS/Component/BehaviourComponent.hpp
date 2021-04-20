﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Serialization/ComponentGen.h"

// Generated
#include "Generated/BehaviourComponent.rfk.h"

namespace GPE RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) BehaviourComponent : public Component
    {
    protected:
        RFKField(Inspect(), Serialize()) bool m_useUpdate      = false;
        RFKField(Inspect(), Serialize()) bool m_useFixedUpdate = false;
        RFKField(Inspect(), Serialize()) bool m_useOnGUI       = false;

    public:
        BehaviourComponent(GameObject & owner) noexcept;

        BehaviourComponent() noexcept;
        BehaviourComponent(const BehaviourComponent& other) noexcept = delete;
        BehaviourComponent(BehaviourComponent && other) noexcept;
        virtual ~BehaviourComponent() noexcept;
        BehaviourComponent& operator=(BehaviourComponent const& other) noexcept = delete;
        BehaviourComponent& operator                                            =(BehaviourComponent&& other) noexcept;

        virtual void start()
        {
        }

        virtual void fixedUpdate(float deltaTime)
        {
        }

        virtual void update(float deltaTime)
        {
        }

        virtual void onGUI()
        {
        }

        void enableUpdate(bool flag) noexcept;
        void enableFixedUpdate(bool flag) noexcept;
        void enableOnGUI(bool flag) noexcept;

        BehaviourComponent_GENERATED
    };

} // namespace )
