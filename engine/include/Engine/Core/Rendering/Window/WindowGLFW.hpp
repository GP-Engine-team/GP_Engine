/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */
#pragma once

#include "Engine/Core/Rendering/Window/IWindow.hpp"

class GLFWwindow;

namespace Engine::Core::Renderering
{
class WindowGLFW final : public IWindow
{
protected:
    GLFWwindow* m_window = nullptr;

protected:
    void initialize(const WindowCreateArg& arg) noexcept final;
    void release() noexcept final;

public:
    inline WindowGLFW(const WindowCreateArg& arg) noexcept : IWindow()
    {
        initialize(arg);
    }

    inline ~WindowGLFW() noexcept
    {
        release();
    }

    WindowGLFW() noexcept = delete;

    WindowGLFW(const WindowGLFW& other) noexcept = delete;

    WindowGLFW(WindowGLFW&& other) noexcept = delete;

    WindowGLFW& operator=(const WindowGLFW& other) noexcept = delete;

    WindowGLFW& operator=(WindowGLFW&& other) noexcept = delete;

    /**
     * @brief return GLFWwindow pointer with generic forme that allow user to ealsy use another lib
     * @return void*
     */
    [[nodiscard]] inline const void* get() const noexcept final;

    [[nodiscard]] inline void* get() noexcept final;

    /**
     * @brief Get the glfw current window
     * @return
     */
    [[nodiscard]] constexpr inline const GLFWwindow* const getGLFWWindow() const noexcept;

    /**
     * @brief return the size of the window
     * @param width
     * @param height
     * @return
     */
    void getSize(unsigned short& width, unsigned short& height) const noexcept final;
};

} // namespace Engine::Core::Renderering

#include "WindowGLFW.inl"