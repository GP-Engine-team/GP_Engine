#include <Engine/ECS/System/InputManagerGLFW.hpp>

#include <Engine/Engine.hpp>
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/imgui.h>

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
        for (const auto& [key, ic] : m_inputComponents)
        {
            auto inputModeMapIp = ic->inputModeMap.find(action);
            if (inputModeMapIp != ic->inputModeMap.end() && inputModeMapIp->second == m_currentInputMode)
            {
                auto keyModeMapIt = ic->keyModeMap.find(action);
                if (keyModeMapIt != ic->keyModeMap.end())
                {
                    if (stateMapIt->second == true)
                    {
                        switch (keyModeMapIt->second)
                        {
                        case EKeyMode::KEY_PRESSED:
                            if (lastStateMapIt->second == false)
                            {
                                lastStateMapIt->second = true;
                                ic->fireAction(action);
                            }
                            break;
                        case EKeyMode::KEY_DOWN:
                            ic->fireAction(action);
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
                                ic->fireAction(action);
                            }
                            break;
                        case EKeyMode::KEY_UP:
                            ic->fireAction(action);
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

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept
{
    if (action >= 0 && action != GLFW_REPEAT)
    {
        if (m_stateMap.count(button))
        {
            auto stateMapIt        = m_stateMap.find(button);
            m_lastStateMap[button] = stateMapIt->second;
        }
        else
        {
            m_lastStateMap[button] = false;
        }
        m_stateMap[button] = action != GLFW_RELEASE;
    }
}

void InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) noexcept
{
    if (m_cursor.tracked)
    {
        m_cursor.deltaPos.x = f32(xpos) - m_cursor.position.x;

        // GLFW cursor position is expressed relative to the top-left corner of the screen
        // Internally, we represent deltaPos' y-axis going up, not down like GLFW
        m_cursor.deltaPos.y = m_cursor.position.y - f32(ypos);
    }

    if (m_cursor.locked)
    {
        GLFWwindow* window = Engine::getInstance()->window.getGLFWWindow();
        glfwSetCursorPos(window, double(m_cursor.position.x), double(m_cursor.position.y));
        return;
    }

    m_cursor.position.x = f32(xpos);
    m_cursor.position.y = f32(ypos);
}

void setCursorCallback(GLFWwindow* window, double xpos, double ypos) noexcept
{
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->cursorPositionCallback(window, xpos, ypos);
}

void setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    if (ImGui::GetCurrentContext() != nullptr)
    {
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    }

    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mods);
}

void setMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (ImGui::GetCurrentContext() != nullptr)
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->mouseButtonCallback(window, button, action, mods);
}

void InputManager::setupCallbacks(GLFWwindow* window) noexcept
{
    glfwSetKeyCallback(window, setKeycallback);
    glfwSetMouseButtonCallback(window, setMouseButtonCallback);
    glfwSetCursorPosCallback(window, setCursorCallback);
}

void InputManager::setCursorMode(int mode) noexcept
{
    glfwSetInputMode(GPE::Engine::getInstance()->window.getGLFWWindow(), GLFW_CURSOR, mode);
}

void InputManager::setMousePos(GPM::Vec2 pos)
{
    glfwSetCursorPos(GPE::Engine::getInstance()->window.getGLFWWindow(), double(pos.x), double(pos.y));
    m_cursor.position = pos;
}

void InputManager::processInput() noexcept
{
    m_cursor.deltaPos.x = .0f;
    m_cursor.deltaPos.y = .0f;
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

bool InputManager::getCursorLockState() const noexcept
{
    return m_cursor.locked;
}

void InputManager::setCursorLockState(bool lockState) noexcept
{
    m_cursor.locked = lockState;
    if (lockState)
    {
        setCursorMode(GLFW_CURSOR_DISABLED);
    }

    else
    {
        setCursorMode(GLFW_CURSOR_NORMAL);
    }
}