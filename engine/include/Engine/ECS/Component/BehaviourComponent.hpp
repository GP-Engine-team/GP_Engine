/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Serialization/ComponentGen.h"

// Generated
#include "Generated/BehaviourComponent.rfk.h"

namespace GPE RFKNamespace()
{
    class RFKClass(Inspect(),ComponentGen) BehaviourComponent : public Component
    {
    protected:
        RFKField(Inspect()) bool m_isUpdated      = false;
        RFKField(Inspect()) bool m_isFixedUpdated = false;

    public:
        BehaviourComponent(GameObject & owner) noexcept;

        BehaviourComponent() noexcept                                = delete;
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

        void enableUpdate(bool flag) noexcept;

        void enableFixedUpdate(bool flag) noexcept;

        BehaviourComponent_GENERATED
    };

} // namespace )

File_GENERATED