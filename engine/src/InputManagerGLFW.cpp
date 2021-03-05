#include "Engine/Core/Input/InputManagerGLFW.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "GLFW/glfw3.h"

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
        auto stateMapIt = m_stateMap.find(key);
        auto lastStateMapIt = m_stateMap.find(key);
        for (int i = 0; i < m_inputComponents.size(); i++)
        {
            auto keyModeMapIt  = m_inputComponents[i]->m_keyModeMap.find(action);
             if (stateMapIt->second == true)
            {
                  switch (keyModeMapIt->second)
                  {
                         case  EKeyMode::KEY_PRESS:
                                          m_inputComponents[i]->fireAction(action);
                                          m_stateMap[key] = false;
                                          break;
                         case  EKeyMode::KEY_REPEAT:
                                          m_inputComponents[i]->fireAction(action);
                                          break;
                  }
            }

            /*else if (keyModeMapIt->second == EKeyMode::KEY_RELEASE && stateMapIt->second == false)
            {
                if (lastStateMapIt->second == true)
                {
                    input->m_inputComponents[i]->fireAction(action);
                }
            }*/
        }
    }
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    if (action != GLFW_REPEAT)
    {
        if (m_stateMap[key])
        {
            auto stateMapIt = m_stateMap.find(key);
            m_lastStateMap[key] = stateMapIt->second;
        }
        m_stateMap[key] = action != GLFW_RELEASE;
    }
}

void InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) noexcept
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

void setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mods);
}

void InputManager::setupCallbacks(GLFWwindow* window) noexcept
{
    glfwSetKeyCallback(window, setKeycallback);
    glfwSetCursorPosCallback(window, setCursorCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
