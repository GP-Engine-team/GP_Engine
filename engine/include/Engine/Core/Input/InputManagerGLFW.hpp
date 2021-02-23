/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include "Engine/Core/Input/Cursor.hpp"
#include "Engine/Core/Input/InputComponent.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

struct GLFWwindow;

namespace GPE
{
class InputManager
{
protected:
    InputManager() = default;

    static InputManager* m_inputManager;

public:
    /**
     * Singletons should not be cloneable.
     */
    InputManager(InputManager& other) = delete;

    /**
     * Singletons should not be assignable.
     */
    void operator=(const InputManager&) = delete;

private:
    std::unordered_multimap<int, std::string> m_actionMap;
    std::unordered_map<int, bool>             m_prevStateMap;
    std::unordered_map<int, bool>             m_stateMap;
    std::unordered_map<int, InputComponent*>  m_inputComponents;
    Cursor                                    m_cursor;

public:
    static InputManager* GetInstance();

    [[nodiscard]] bool checkForAction(const std::string& action) const noexcept;
    void            keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) const noexcept;
    void            cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) noexcept;
    void            fireInputComponents(const std::string& action) const noexcept;
    void            setupCallbacks(GLFWwindow* window) noexcept;
    inline void     bindInput(int key, const std::string& action) noexcept;
    inline int      addComponent(InputComponent* input) noexcept;
    inline void     removeComponent(int key) noexcept;
    void            processInput() noexcept;
    void            keyPressed(int key) noexcept;
    [[nodiscard]] inline const Cursor& getCursor() const noexcept;
};
inline void setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;
inline void setCursorCallback(GLFWwindow* window, double xpos, double ypos) noexcept;

#include "Engine/Core/Input/InputManager.inl"
} // namespace GPE