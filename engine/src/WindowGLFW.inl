#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"


inline const void* Engine::Core::Renderering::Window::get() const noexcept
{
    return m_window;
}

inline void* Engine::Core::Renderering::Window::get() noexcept
{
    return m_window;
}

constexpr inline const GLFWwindow* Engine::Core::Renderering::Window::getGLFWWindow() const noexcept
{
    return m_window;
}

constexpr inline GLFWwindow* Engine::Core::Renderering::Window::getGLFWWindow() noexcept
{
    return m_window;
}