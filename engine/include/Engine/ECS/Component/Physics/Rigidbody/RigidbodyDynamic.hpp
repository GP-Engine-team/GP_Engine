/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/ECS/Component/Component.hpp>
#include <Engine/ECS/Component/Physics/Collisions/Collider.hpp>
#include <PxRigidDynamic.h>

// Generated
#include "Generated/RigidbodyDynamic.rfk.h"

namespace GPE RFKNamespace()
{

    class RFKClass(ComponentGen(), Inspect()) RigidbodyDynamic : public Component
    {
    public:
        RigidbodyDynamic(GameObject & owner) noexcept;

        RigidbodyDynamic() noexcept                              = default;
        RigidbodyDynamic(const RigidbodyDynamic& other) noexcept = delete;
        RigidbodyDynamic(RigidbodyDynamic && other) noexcept     = default;
        RigidbodyDynamic& operator=(RigidbodyDynamic const& other) noexcept = delete;
        RigidbodyDynamic& operator=(RigidbodyDynamic&& other) noexcept = delete;

        void update() noexcept;

        virtual ~RigidbodyDynamic() noexcept = default;

    public:
        physx::PxRigidDynamic* rigidbody;
        Collider*              collider;

    private:
        RFKField(Inspect()) bool m_isKinematic = false;

    public:
        void setKinematic(bool state) noexcept;
        GETTER_BY_VALUE(KinematicState, m_isKinematic);

        /**
         * @brief Add or remove current component from it's system which have for effect to enable or disable it
         * @param newState
         * @return
         */
        void setActive(bool newState) noexcept override;

        RigidbodyDynamic_GENERATED
    };
} // namespace )
