#include "Engine/Core/Input/InputManagerGLFW.hpp"
#include "GLFW/glfw3.h"

using namespace std;
using namespace GPE;

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

void InputManager::fireInputComponents(const std::string& action) const
{
    InputManager* input = InputManager::GetInstance();
    if (!action.empty())
    {
        for (int i = 0; i < input->m_inputComponents.size(); i++)
        {
            input->m_inputComponents[i]->fireAction(action);
        }
    }
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) const
{
    InputManager* input = InputManager::GetInstance();

    input->m_stateMap[key] = action != GLFW_RELEASE;
}

void InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) const
{
}

void InputManager::setupCallbacks(GLFWwindow* window)
{
    glfwSetKeyCallback(window, setKeycallback);
    glfwSetCursorPosCallback(window, setCursorCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputManager::processInput()
{
    InputManager* input = InputManager::GetInstance();

    for (auto keyState : input->m_stateMap)
    {
        if (keyState.second == false)
            continue;

        auto it = input->m_actionMap.equal_range(keyState.first);
        for (auto i2 = it.first; i2 != it.second; i2++)
        {
            input->fireInputComponents(i2->second);
        }
    }
}

void InputManager::keyPressed(int key)
{
    InputManager* input = InputManager::GetInstance();

    auto it = input->m_actionMap.equal_range(key);
    for (auto i = it.first; i != it.second; i++)
    {
        input->fireInputComponents(i->second);
    }
}