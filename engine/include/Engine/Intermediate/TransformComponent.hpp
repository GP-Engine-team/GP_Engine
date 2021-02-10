/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Intermediate/Component.hpp"

namespace Engine::Intermediate
{
class TransformComponent : public Component
{
protected:
    bool m_isDirty = false;
    
public:

    TransformComponent(GameObject& refGameObject) noexcept
        : Component(refGameObject)
    {}

    inline TransformComponent() noexcept = delete;

    inline TransformComponent(const TransformComponent& other) noexcept = default;

    inline TransformComponent(TransformComponent&& other) noexcept = default;

    inline ~TransformComponent() noexcept = default;

    inline TransformComponent& operator=(TransformComponent const& other) noexcept = default;

    inline TransformComponent& operator=(TransformComponent&& other) noexcept = default;

    [[nodiscard]] constexpr inline
    bool isDirty() const { return m_isDirty; }
};
} /*namespace Engine::Intermediate*/
