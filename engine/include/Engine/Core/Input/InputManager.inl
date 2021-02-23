#include "Engine/Core/Input/InputManagerGLFW.hpp"

void GPE::setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    InputManager::GetInstance()->keyCallback(window, key, scancode, action, mods);
}

void GPE::setCursorCallback(GLFWwindow* window, double xpos, double ypos) noexcept
{
    InputManager::GetInstance()->cursorPositionCallback(window, xpos, ypos);
}

void InputManager::bindInput(int key, const std::string& action) noexcept
{
    m_actionMap.emplace(key, action);
}

int InputManager::addComponent(InputComponent* input) noexcept
{
   int key = static_cast<int>(m_inputComponents.size());
    m_inputComponents.emplace(key, input);

    return key;
}

void InputManager::removeComponent(int key) noexcept
{
    m_inputComponents.erase(key);
}

[[nodiscard]] inline const Cursor& InputManager::getCursor() const noexcept
{
    return m_cursor;
}