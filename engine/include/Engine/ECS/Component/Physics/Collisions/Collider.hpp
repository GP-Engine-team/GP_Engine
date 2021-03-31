﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/ECS/Component/Component.hpp>
#include <PxShape.h>

namespace GPE
{
class Collider : public Component
{
public:
    Collider(GameObject& owner) noexcept : Component(owner)
    {
    }

    Collider() noexcept                      = delete;
    Collider(const Collider& other) noexcept = delete;
    Collider(Collider&& other) noexcept      = default;
    Collider& operator=(Collider const& other) noexcept = delete;
    Collider& operator=(Collider&& other) noexcept = delete;

    virtual ~Collider() noexcept = default;

public:
    physx::PxShape*    shape     = nullptr;
    physx::PxMaterial* material  = nullptr;
    bool               isTrigger = false;
    bool               isVisible = false;
};
} // namespace GPE