/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Engine/ECS/Component/Component.hpp"
#include "GPM/Vector3.hpp"

enum class EKeyMode
{
    KEY_PRESSED  = 0,
    KEY_RELEASED = 2,
    KEY_DOWN     = 3,
    KEY_UP       = 4,
};

namespace GPE
{
class InputComponent : public Component
{
public:
    // InputComponent() = delete;
    InputComponent(const InputComponent& other) noexcept = delete;
    InputComponent(InputComponent&& other);
    virtual ~InputComponent();
    InputComponent(GameObject& owner);

    InputComponent& operator=(InputComponent&& other) noexcept;

private:
    std::unordered_map<std::string, std::function<void()>> m_functionMap;
    int                                                    m_key = -1;

public:
    std::unordered_map<std::string, EKeyMode> m_keyModeMap;
    /**
     * @brief Bind a function to an action
     * @param action
     * @param function
     */
    template <typename T>
    void bindAction(const std::string& action, const EKeyMode& keyMode, T* owner, void (T::*function)()) noexcept
    {
        m_functionMap.emplace(action, std::bind(function, owner));
        m_keyModeMap.emplace(action, keyMode);
    }

    /**
     * @brief launch an action
     * @param action
     */
    void fireAction(const std::string& action) noexcept;
};
} // namespace GPE
