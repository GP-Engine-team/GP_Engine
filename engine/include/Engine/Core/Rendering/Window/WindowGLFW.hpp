﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */
#pragma once

#include <string> //std::string

struct GLFWwindow;

namespace GPE
{
class Window
{
public:
    struct CreateArg
    {
        std::string name   = "";
        int         width  = 0;
        int         height = 0;
        bool        show   = false;
    };

protected:
    GLFWwindow* m_window = nullptr;

public:
    Window(const CreateArg& arg) noexcept;

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
    [[nodiscard]] constexpr inline GLFWwindow*       getGLFWWindow() noexcept;
    
    void close();

    /**
     * @brief return the size of the window
     * @param width
     * @param height
     * @return
     */
    void getSize(int& width, int& height) const noexcept;

    void maximize();
    void minimize();
    void setFullscreen();
    void show();
    void hide();
};

#include "WindowGLFW.inl"

} // namespace GPE
