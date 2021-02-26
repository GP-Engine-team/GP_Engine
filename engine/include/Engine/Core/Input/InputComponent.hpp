/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include "Engine/Intermediate/Component.hpp"
#include "GPM/Vector3.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

enum class EKeyMode
{
    KEY_PRESS   = 0,
    KEY_REPEAT  = 1,
    KEY_RELEASE = 2,
};

namespace GPE
{
class InputComponent : public Component
{
public:
    // InputComponent() = delete;
    InputComponent(const InputComponent& other) noexcept;
    InputComponent(InputComponent&& other) noexcept;
    virtual ~InputComponent() = default;
    InputComponent(GameObject& owner);

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