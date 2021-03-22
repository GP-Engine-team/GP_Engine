/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/ECS/Component/Component.hpp>
#include <PxShape.h>

namespace GPE
{
class CollisionComponent : public Component
{
public:
    CollisionComponent(GameObject& owner) noexcept : Component(owner)
    {
    }

    ~CollisionComponent() noexcept = default;

public:
    physx::PxShape* shape     = nullptr;
    bool            isTrigger = false;
    bool            isVisible = false;
};
} // namespace GPE
