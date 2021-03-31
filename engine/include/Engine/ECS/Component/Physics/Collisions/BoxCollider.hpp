﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/ECS/Component/Physics/Collisions/collider.hpp>
#include <GPM/Vector3.hpp>

namespace GPE
{
class BoxCollider : public Collider
{
public:
    BoxCollider(GameObject& owner) noexcept;

    virtual ~BoxCollider() noexcept = default;

    BoxCollider() noexcept                         = delete;
    BoxCollider(const BoxCollider& other) noexcept = delete;
    BoxCollider(BoxCollider&& other) noexcept      = default;
    BoxCollider& operator=(BoxCollider const& other) noexcept = delete;
    BoxCollider& operator=(BoxCollider&& other) noexcept = delete;

private:
    GPM::Vector3 m_center;
    GPM::Vector3 m_dimensions;

public:
    DEFAULT_GETTER_SETTER_BY_REF(Center, m_center);
    GETTER_BY_VALUE(Dimensions, m_dimensions);
    void setDimensions(GPM::Vec3 newDimensions) noexcept;
};
} // namespace GPE