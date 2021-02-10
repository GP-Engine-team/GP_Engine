#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"

constexpr inline const GLFWwindow* Engine::Core::Renderering::Window::getGLFWWindow() const noexcept
{
    return m_window;
}

constexpr inline GLFWwindow* Engine::Core::Renderering::Window::getGLFWWindow() noexcept
{
    return m_window;
}