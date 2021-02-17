/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */
#pragma once

#include <string> //std::string

struct GLFWwindow;

namespace Engine::Core::Renderering
{

struct WindowCreateArg
{
    std::string name = "";
    int height = 0;
    int width = 0;
};

class Window
{
protected:
    GLFWwindow* m_window = nullptr;

public:

    Window(const WindowCreateArg& arg) noexcept;

    ~Window() noexcept;

    Window() noexcept = delete;

    Window(const Window& other) noexcept = delete;

    Window(Window&& other) noexcept = delete;

    Window& operator=(const Window& other) noexcept = delete;

    Window& operator=(Window&& other) noexcept = delete;

    /**
     * @brief Get the glfw current window
     * @return
     */
    [[nodiscard]] constexpr inline const GLFWwindow* getGLFWWindow() const noexcept;
    [[nodiscard]] constexpr inline GLFWwindow* getGLFWWindow() noexcept;

    /**
     * @brief return the size of the window
     * @param width
     * @param height
     * @return
     */
    void getSize(int& width, int& height) const noexcept;
};

} // namespace Engine::Core::Renderering

#include "WindowGLFW.inl"
