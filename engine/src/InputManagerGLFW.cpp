#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include <GLFW/glfw3.h>

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
        auto lastStateMapIt = m_stateMap.find(key);
        for (int i = 0; i < m_inputComponents.size(); i++)
        {
            auto keyModeMapIt = m_inputComponents[i]->m_keyModeMap.find(action);
            if (keyModeMapIt != m_inputComponents[i]->m_keyModeMap.end())
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

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    if (action != GLFW_REPEAT)
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
    m_cursor.deltaPos =
        Vec2{m_cursor.position.x - static_cast<GPM::f32>(xpos), m_cursor.position.y - static_cast<GPM::f32>(ypos)};
    m_cursor.position.x = static_cast<GPM::f32>(xpos);
    m_cursor.position.y = static_cast<GPM::f32>(ypos);
}

void InputManager::cursorLockedPositionCallback(GLFWwindow* window, double xpos, double ypos) noexcept
{
    int x, y;
    glfwGetWindowSize(window, &x, &y);
    m_cursor.center.x   = x / 2.f;
    m_cursor.center.y   = y / 2.f;
    m_cursor.position.x = static_cast<GPM::f32>(xpos);
    m_cursor.position.y = static_cast<GPM::f32>(ypos);
    m_cursor.deltaPos   = m_cursor.position - m_cursor.center;
    glfwSetCursorPos(window, m_cursor.center.x, m_cursor.center.y);
}

void setCursorCallback(GLFWwindow* window, double xpos, double ypos) noexcept
{
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->cursorPositionCallback(window, xpos, ypos);
}

void setLockedCursorCallback(GLFWwindow* window, double xpos, double ypos) noexcept
{
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->cursorLockedPositionCallback(window, xpos, ypos);
}

void setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mods);
}

void InputManager::setupCallbacks(GLFWwindow* window, bool lockMousInCenter) noexcept
{
    glfwSetKeyCallback(window, setKeycallback);
    glfwSetCursorPosCallback(window, lockMousInCenter ? setLockedCursorCallback : setCursorCallback);
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
