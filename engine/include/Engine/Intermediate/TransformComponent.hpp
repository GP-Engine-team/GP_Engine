/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

namespace Engine::Intermediate
{
class TransformComponent
{
protected:
    bool m_isDirty = false;

public:
    constexpr inline TransformComponent() noexcept = default;

    constexpr inline TransformComponent(const TransformComponent& other) noexcept = default;

    constexpr inline TransformComponent(TransformComponent&& other) noexcept = default;

    inline virtual ~TransformComponent() noexcept = default;

    constexpr inline TransformComponent& operator=(TransformComponent const& other) noexcept = default;

    constexpr inline TransformComponent& operator=(TransformComponent&& other) noexcept = default;

    [[nodiscard]] constexpr inline bool isDirty() noexcept
    {
        return m_isDirty;
    }
};
} /*namespace Engine::Intermediate*/
