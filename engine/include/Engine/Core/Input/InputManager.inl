#include "Engine/Core/Input/InputManagerGLFW.hpp"

void GPE::setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputManager::GetInstance()->keyCallback(window, key, scancode, action, mods);
}

void GPE::setCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    InputManager::GetInstance()->cursorPositionCallback(window, xpos, ypos);
}

void InputManager::bindInput(int key, const std::string& action)
{
    m_actionMap.emplace(key, action);
}

const int InputManager::addComponent(InputComponent* input)
{
   int key = static_cast<int>(m_inputComponents.size());
    m_inputComponents.emplace(key, input);

    return key;
}

void InputManager::removeComponent(int key)
{
    m_inputComponents.erase(key);
}