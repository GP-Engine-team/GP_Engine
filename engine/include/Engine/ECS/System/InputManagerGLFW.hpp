/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/ECS/Component/InputComponent.hpp>
#include <Engine/Resources/Cursor.hpp>
#include <string>
#include <unordered_map>

struct GLFWwindow;

namespace GPE
{
class InputManager
{

private:
    std::unordered_multimap<int, std::string> m_actionMap;
    std::unordered_map<int, bool>             m_stateMap;
    std::unordered_map<int, bool>             m_lastStateMap;
    std::unordered_map<int, InputComponent*>  m_inputComponents;
    std::string                               m_currentInputMode = "none";
    Cursor                                    m_cursor;

public:
    InputManager(class Window& window) noexcept;

    /**
     * @brief Callback to get the state of any keyboard keys
     * @param window
     * @param key
     * @param scancode
     * @param action
     * @param mods
     * @return
     */
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;

    /**
     * @brief Callback to get the new position of the mouse cursor
     * @param window
     * @param xpos
     * @param ypos
     * @return
     */
    void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) noexcept;

    /**
     * @brief Throw events to input components
     * @param action
     * @return
     */
    void fireInputComponents(const std::string& action, const int& key) noexcept;

    /**
     * @brief Setup Callbacks (call it one time to setup callbacks function proprely)
     * @param window
     * @return
     */
    void setupCallbacks(GLFWwindow* window) noexcept;

    /**
     * @brief Set cursor mode using GLFW Enum
     */
    void setCursorMode(GLFWwindow* window, int mode) noexcept;

    /**
     * @brief Set the current input mode
     * @param inputMode
     * @return
     */
    inline void setInputMode(const std::string& inputMode) noexcept;

    /**
     * @brief Set the current input mode
     * @param
     * @return const std::string& inputMode
     */
    [[nodiscard]] inline const std::string& getInputMode() noexcept;

    /**
     * @brief Set the cursor tracking state
     * @param trackState
     * @return
     */
    inline void setCursorTrackingState(bool trackState) noexcept;

    /**
     * @brief Set the cursor lock state
     * @param lockState
     * @return
     */
    void setCursorLockState(bool lockState) noexcept;

    /**
     * @brief bind a key to an action
     * @param key
     * @param action
     * @return
     */
    inline void bindInput(int key, const std::string& action) noexcept;

    /**
     * @brief add input component to the component list
     * @param input
     * @return
     */
    inline int addComponent(InputComponent& input) noexcept;

    inline void updateComponent(InputComponent* newPointerInputComponent, int key) noexcept;

    /**
     * @brief remove input component to the component list
     * @param key
     * @return
     */
    inline void removeComponent(int key) noexcept;

    /**
     * @brief Call it to poll events and update inputs
     * @return
     */
    void processInput() noexcept;

    /**
     * @brief get a reference to the cursor
     * @return
     */
    [[nodiscard]] inline const Cursor& getCursor() const noexcept;
};
} // namespace GPE

#include "Engine/ECS/System/InputManagerGLFW.inl"