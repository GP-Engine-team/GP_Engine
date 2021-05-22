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

    class RFKClass(ComponentGen(), Inspect(), Serialize()) RigidbodyDynamic : public Component, public RigidBodyBase
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
        RFKField(Inspect("setKinematic"), Serialize()) bool m_isKinematic = false;

    protected:
        virtual void updateToSystem() noexcept override;

    public:
        void setKinematic(bool state) noexcept;
        GETTER_BY_VALUE(KinematicState, m_isKinematic);

        /**
         * @brief Update the current shape of the rigidbody
         */
        virtual void updateShape(physx::PxShape & oldShape);

        RigidbodyDynamic_GENERATED
    };
} // namespace )
