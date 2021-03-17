/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

namespace GPE
{
class GameObject;

class Component
{
protected:
    GameObject& m_gameObject;
    bool        m_isActivated{true};

public:
    inline Component(GameObject& owner) noexcept;
    inline Component() noexcept                       = delete;
    inline Component(const Component& other) noexcept = delete;
    inline Component(Component&& other) noexcept      = default;
    inline virtual ~Component() noexcept              = default;
    inline Component& operator=(const Component& other) noexcept = delete;
    inline Component& operator=(Component&& other) noexcept = default;

    [[nodiscard]] constexpr inline GameObject& getOwner() noexcept;

    [[nodiscard]] constexpr inline const GameObject& getOwner() const noexcept;

    [[nodiscard]] constexpr inline bool isActivated() const noexcept;

    constexpr inline void setActive(bool newState) noexcept;
};

#include "Component.inl"

} // namespace GPE
