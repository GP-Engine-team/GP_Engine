#include "Engine/Core/Input/InputManagerGLFW.hpp"

void GPE::setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputManager::GetInstance()->keyCallback(window, key, scancode, action, mods);
}

void InputManager::bindInput(const int& key, const std::string& action)
{
    m_actionMap.emplace(key, action);
}

const int& InputManager::addComponent(InputComponent* input)
{
   int key = static_cast<int>(m_inputComponents.size());
    //m_inputComponents.emplace(key, input);

    return key;
}

void InputManager::removeComponent(const int& key)
{
    m_inputComponents.erase(key);
}