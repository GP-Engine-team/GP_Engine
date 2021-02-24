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
    std::unordered_map<int, bool>             m_stateMap;
    std::unordered_map<int, InputComponent*>  m_inputComponents;
    Cursor                                    m_cursor;

public:
    /**
     * @brief get a pointeur to the InputManager singleton
     * @return 
    */
    static InputManager* GetInstance();

    /**
     * @brief Callback to get the state of any keyboard keys
     * @param window 
     * @param key 
     * @param scancode 
     * @param action 
     * @param mods 
     * @return 
    */
    void            keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) const noexcept;
    
    /**
     * @brief Callback to get the new position of the mouse cursor
     * @param window 
     * @param xpos 
     * @param ypos 
     * @return 
    */
    void            cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) noexcept;

    /**
     * @brief Throw events to input components
     * @param action 
     * @return 
    */
    void            fireInputComponents(const std::string& action, const int& key) const noexcept;

    /**
     * @brief Setup Callbacks (call it one time to setup callbacks function proprely)
     * @param window 
     * @return 
    */
    void            setupCallbacks(GLFWwindow* window) noexcept;

    /**
     * @brief bind a key to an action
     * @param key 
     * @param action 
     * @return 
    */
    inline void     bindInput(int key, const std::string& action) noexcept;

    /**
     * @brief add input component to the component list
     * @param input 
     * @return 
    */
    inline int      addComponent(InputComponent* input) noexcept;

    /**
     * @brief remove input component to the component list
     * @param key 
     * @return 
    */
    inline void     removeComponent(int key) noexcept;

    /**
     * @brief Call it to poll events and update inputs
     * @return 
    */
    void            processInput() noexcept;
    
    
    /**
     * @brief get a reference to the cursor
     * @return 
    */
    [[nodiscard]] inline const Cursor& getCursor() const noexcept;
};

#include "Engine/Core/Input/InputManager.inl"
} // namespace GPE