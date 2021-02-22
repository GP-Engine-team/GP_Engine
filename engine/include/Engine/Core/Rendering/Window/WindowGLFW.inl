#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"

constexpr inline const GLFWwindow* Window::getGLFWWindow() const noexcept
{
    return m_window;
}

constexpr inline GLFWwindow* Window::getGLFWWindow() noexcept
{
    return m_window;
}