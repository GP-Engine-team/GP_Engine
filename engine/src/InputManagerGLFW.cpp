#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "GPM/DebugOutput.hpp"
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Engine.hpp>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui.h>

using namespace std;
using namespace GPE;
using namespace GPM;

InputManager::InputManager(Window& window) noexcept
{
    glfwSetWindowUserPointer(window.getGLFWWindow(), this);
}

void InputManager::fireInputComponents(const std::string& action, const int& key) noexcept
{
    if (!action.empty())
    {
        auto stateMapIt     = m_stateMap.find(key);
        auto lastStateMapIt = m_lastStateMap.find(key);
        for (int i = 0; i < m_inputComponents.size(); i++)
        {
            auto inputModeMapIp = m_inputComponents[i]->inputModeMap.find(action);
            if (inputModeMapIp != m_inputComponents[i]->inputModeMap.end() &&
                inputModeMapIp->second == m_currentInputMode)
            {
                auto keyModeMapIt = m_inputComponents[i]->keyModeMap.find(action);
                if (keyModeMapIt != m_inputComponents[i]->keyModeMap.end())
                {
                    if (stateMapIt->second == true)
                    {
                        switch (keyModeMapIt->second)
                        {
                        case EKeyMode::KEY_PRESSED:
                            if (lastStateMapIt->second == false)
                            {
                                lastStateMapIt->second = true;
                                m_inputComponents[i]->fireAction(action);
                            }
                            break;
                        case EKeyMode::KEY_DOWN:
                            m_inputComponents[i]->fireAction(action);
                            break;
                        }
                    }
                    else
                    {
                        switch (keyModeMapIt->second)
                        {
                        case EKeyMode::KEY_RELEASED:
                            if (lastStateMapIt->second == true)
                            {
                                lastStateMapIt->second = false;
                                m_inputComponents[i]->fireAction(action);
                            }
                            break;
                        case EKeyMode::KEY_UP:
                            m_inputComponents[i]->fireAction(action);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    if (action >= 0 && action != GLFW_REPEAT)
    {
        if (m_stateMap.count(key))
        {
            auto stateMapIt     = m_stateMap.find(key);
            m_lastStateMap[key] = stateMapIt->second;
        }
        else
        {
            m_lastStateMap[key] = false;
        }
        m_stateMap[key] = action != GLFW_RELEASE;
    }
}

void InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) noexcept
{
    if (m_cursor.tracked)
    {
        m_cursor.deltaPos =
            Vec2{static_cast<GPM::f32>(xpos) - m_cursor.position.x, static_cast<GPM::f32>(ypos) - m_cursor.position.y};
    }

    m_cursor.position.x = static_cast<GPM::f32>(xpos);
    m_cursor.position.y = static_cast<GPM::f32>(ypos);
}

void setCursorCallback(GLFWwindow* window, double xpos, double ypos) noexcept
{
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->cursorPositionCallback(window, xpos, ypos);
}

void setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mods);
}

void InputManager::setupCallbacks(GLFWwindow* window) noexcept
{
    glfwSetKeyCallback(window, setKeycallback);
    glfwSetCursorPosCallback(window, setCursorCallback);
}

void InputManager::setCursorMode(GLFWwindow* window, int mode) noexcept
{
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void InputManager::processInput() noexcept
{
    m_cursor.deltaPos = {0, 0};
    glfwPollEvents();

    for (auto keyState : m_stateMap)
    {
        auto it = m_actionMap.equal_range(keyState.first);
        for (auto i2 = it.first; i2 != it.second; i2++)
        {
            fireInputComponents(i2->second, keyState.first);
        }
    }
}

void InputManager::setCursorLockState(bool lockState) noexcept
{
    m_cursor.locked = lockState;
    if (lockState)
    {
        setCursorMode(Engine::getInstance()->window.getGLFWWindow(), GLFW_CURSOR_DISABLED);
    }

    else
    {
        setCursorMode(Engine::getInstance()->window.getGLFWWindow(), GLFW_CURSOR_NORMAL);
    }
}