﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/ECS/Component/Component.hpp>
#include <Engine/ECS/Component/Physics/Collisions/Collider.hpp>
#include <PxRigidDynamic.h>

namespace GPE
{

class RigidbodyDynamic : public Component
{
public:
    RigidbodyDynamic(GameObject& owner) noexcept;

    RigidbodyDynamic() noexcept                              = delete;
    RigidbodyDynamic(const RigidbodyDynamic& other) noexcept = delete;
    RigidbodyDynamic(RigidbodyDynamic&& other) noexcept      = default;
    RigidbodyDynamic& operator=(RigidbodyDynamic const& other) noexcept = delete;
    RigidbodyDynamic& operator=(RigidbodyDynamic&& other) noexcept = delete;

    void update() noexcept;

    virtual ~RigidbodyDynamic() noexcept = default;

public:
    physx::PxRigidDynamic* rigidbody;
    Collider*              collider;
};
} // namespace GPE