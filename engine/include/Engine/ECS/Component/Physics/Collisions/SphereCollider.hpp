/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/ECS/Component/Physics/Collisions/CollisionComponent.hpp>
#include <GPM/Vector3.hpp>

namespace GPE
{
class SphereCollider : public CollisionComponent
{
public:
    SphereCollider(GameObject& owner) noexcept;
    ~SphereCollider() noexcept;

private:
    GPM::Vector3 m_center;
    float        m_radius;

public:
    DEFAULT_GETTER_SETTER_BY_REF(Center, m_center);
    DEFAULT_GETTER_SETTER_BY_VALUE(Radius, m_radius);
};
} // namespace GPE
