#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"

inline const void * Engine::Core::Renderering::WindowGLFW::get() const noexcept
{
    return m_window;
}

inline void * Engine::Core::Renderering::WindowGLFW::get() noexcept
{
    return m_window;
}

constexpr inline const GLFWwindow *const Engine::Core::Renderering::WindowGLFW::getGLFWWindow() const noexcept
{
    return m_window;
}