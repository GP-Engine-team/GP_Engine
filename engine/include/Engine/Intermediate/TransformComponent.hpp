/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/Component.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include <vector>

namespace Engine::Intermediate
{
class TransformComponent : public Component
{
protected:
    bool m_isDirty = false;

public:
    TransformComponent(GameObject& refGameObject) noexcept : Component(refGameObject)
    {
    }

    TransformComponent() noexcept                                = delete;
    TransformComponent(const TransformComponent& other) noexcept = delete;
    TransformComponent(TransformComponent&& other) noexcept      = default;
    virtual ~TransformComponent() noexcept                       = default;
    TransformComponent& operator=(TransformComponent const& other) noexcept = delete;
    TransformComponent& operator=(TransformComponent&& other) noexcept = default;

    [[nodiscard]] constexpr inline bool isDirty() const
    {
        return m_isDirty;
    }
};
} /*namespace Engine::Intermediate*/
