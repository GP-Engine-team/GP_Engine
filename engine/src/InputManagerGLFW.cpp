#include "Engine/Core/Input/InputManagerGLFW.hpp"
#include "GLFW/glfw3.h"

using namespace std;
using namespace GPE;
using namespace GPM;

InputManager* InputManager::m_inputManager = nullptr;

InputManager* InputManager::GetInstance()
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if (m_inputManager == nullptr)
    {
        m_inputManager = new InputManager();
    }
    return m_inputManager;
}

void InputManager::fireInputComponents(const std::string& action, const int& key) const noexcept
{
    InputManager* input = InputManager::GetInstance();
    if (!action.empty())
    {
        auto it = input->m_stateMap.find(key);
        for (int i = 0; i < input->m_inputComponents.size(); i++)
        {
            auto it2  = input->m_inputComponents[i]->m_keyModeMap.find(action);
            if (it2->second == EKeyMode::KEY_PRESS && it->second == true)
            {
                input->m_inputComponents[i]->fireAction(action);
                input->m_stateMap[key] = false;
            }

            else if (it2->second == EKeyMode::KEY_REPEAT && it->second == true)
            {
                input->m_inputComponents[i]->fireAction(action);
            }
        }
    }
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) const noexcept
{
    InputManager* input = InputManager::GetInstance();

    if (action != GLFW_REPEAT)
    {
        input->m_stateMap[key] = action != GLFW_RELEASE;
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

static void setCursorCallback(GLFWwindow* window, double xpos, double ypos) noexcept
{
    InputManager::GetInstance()->cursorPositionCallback(window, xpos, ypos);
}

static void setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    InputManager::GetInstance()->keyCallback(window, key, scancode, action, mods);
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

    InputManager* input = InputManager::GetInstance();

    for (auto keyState : input->m_stateMap)
    {
        auto it = input->m_actionMap.equal_range(keyState.first);
        for (auto i2 = it.first; i2 != it.second; i2++)
        {
            input->fireInputComponents(i2->second, keyState.first);
        }
    }
}
