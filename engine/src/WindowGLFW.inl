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

inline void Engine::Core::Renderering::WindowGLFW::getSize(size_t &width, size_t &height) const noexcept
{
    glfwGetWindowSize(m_window, reinterpret_cast<int *>(&width), reinterpret_cast<int *>(&height));
}