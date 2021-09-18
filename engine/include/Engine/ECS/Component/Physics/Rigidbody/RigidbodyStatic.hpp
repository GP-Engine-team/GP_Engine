/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Physics/RigidBodyBase.hpp>
#include <Engine/ECS/Component/Component.hpp>
#include <PxRigidStatic.h>

// Generated
#include "Generated/RigidbodyStatic.rfk.h"

namespace GPE RFKNamespace()
{

    class RFKClass(Inspect(), Serialize(), ComponentGen()) RigidbodyStatic : public Component, public RigidBodyBase
    {

    public:
        RigidbodyStatic(GameObject & owner) noexcept;

        RigidbodyStatic() noexcept                             = default;
        RigidbodyStatic(const RigidbodyStatic& other) noexcept = delete;
        RigidbodyStatic(RigidbodyStatic && other) noexcept     = default;
        RigidbodyStatic& operator=(RigidbodyStatic const& other) noexcept = delete;
        RigidbodyStatic& operator=(RigidbodyStatic&& other) noexcept = delete;

        virtual ~RigidbodyStatic() noexcept;

    public:
        physx::PxRigidStatic* rigidbody = nullptr;

    protected:
        virtual void updateToSystem() noexcept override;
        virtual void onPostLoad() noexcept override;

    public:
        /**
         * @brief Update the current shape of the rigidbody
         */
        virtual void updateShape(physx::PxShape & oldShape);

        RFKMethod() void updateTransform();

        RigidbodyStatic_GENERATED
    };
} // namespace )

File_GENERATED