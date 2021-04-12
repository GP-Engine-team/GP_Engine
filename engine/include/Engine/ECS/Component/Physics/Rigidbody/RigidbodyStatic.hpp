/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/ECS/Component/Component.hpp>
#include <Engine/ECS/Component/Physics/Collisions/Collider.hpp>
#include <PxRigidStatic.h>

namespace GPE
{

class RigidbodyStatic : public Component
{
public:
    RigidbodyStatic(GameObject& owner) noexcept;

    RigidbodyStatic() noexcept                             = delete;
    RigidbodyStatic(const RigidbodyStatic& other) noexcept = delete;
    RigidbodyStatic(RigidbodyStatic&& other) noexcept      = default;
    RigidbodyStatic& operator=(RigidbodyStatic const& other) noexcept = delete;
    RigidbodyStatic& operator=(RigidbodyStatic&& other) noexcept = delete;

    virtual ~RigidbodyStatic() noexcept = default;

public:
    physx::PxRigidStatic* rigidbody;
    Collider*             collider;
};
} // namespace GPE
