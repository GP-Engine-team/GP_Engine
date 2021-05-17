/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Physics/RigidBodyBase.hpp>
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/ECS/Component/Component.hpp>
#include <PxRigidDynamic.h>

// Generated
#include "Generated/RigidbodyDynamic.rfk.h"

namespace GPE RFKNamespace()
{

    class RFKClass(ComponentGen(), Inspect()) RigidbodyDynamic : public Component, RigidBodyBase
    {
    public:
        RigidbodyDynamic(GameObject & owner, EShapeType _type) noexcept;

        RigidbodyDynamic() noexcept                              = default;
        RigidbodyDynamic(const RigidbodyDynamic& other) noexcept = delete;
        RigidbodyDynamic(RigidbodyDynamic && other) noexcept     = default;
        RigidbodyDynamic& operator=(RigidbodyDynamic const& other) noexcept = delete;
        RigidbodyDynamic& operator=(RigidbodyDynamic&& other) noexcept = delete;

        void update() noexcept;

        void updatePosition() noexcept;

        virtual ~RigidbodyDynamic() noexcept;

    public:
        physx::PxRigidDynamic* rigidbody;

    private:
        bool m_isKinematic = false;

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
