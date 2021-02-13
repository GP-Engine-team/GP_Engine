/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

namespace Engine::Intermediate
{
class GameObject;

class Component
{
protected:
    GameObject* m_gameObject = nullptr;
    bool m_isActivated{true};

public:

    inline
    Component(GameObject& refGameObject) noexcept;

    inline
    Component () noexcept 					            = delete;

    inline Component(const Component& other) noexcept = delete;

    inline
    Component (Component&& other) noexcept				= default;

    inline
    virtual ~Component () noexcept				        = default;

    inline
    Component& operator=(const Component& other) noexcept		= delete;

    inline
    Component& operator=(Component && other) noexcept			= default;

    [[nodiscard]] constexpr inline
    GameObject& getGameObject() noexcept;

    [[nodiscard]] constexpr inline
    const GameObject& getGameObject() const noexcept;

    [[nodiscard]] constexpr inline
    bool isActivated() const noexcept;

    constexpr inline
    void setActive(bool newState) noexcept;
};
} // namespace Engine::Intermediate

#include "Component.inl"