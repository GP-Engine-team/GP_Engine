/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
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

public:
    static InputManager* GetInstance();

    _NODISCARD bool             checkForAction(const std::string& action) const;
    void                        keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) const;
    void                        cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) const;
    void                        fireInputComponents(const std::string& action) const;
    void                        setupCallbacks(GLFWwindow* window);
    inline void                 bindInput(int key, const std::string& action);
    _NODISCARD inline const int addComponent(InputComponent* input);
    inline void                 removeComponent(int key);
    void                        processInput();
    void                        keyPressed(int key);
};
inline void setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods);
inline void setCursorCallback(GLFWwindow* window, double xpos, double ypos);

#include "Engine/Core/Input/InputManager.inl"
} // namespace GPE