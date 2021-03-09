/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */
#pragma once

namespace GPE
{
class Window;

class Renderer
{
protected:
    Window* m_pWindow = nullptr;

public:
    Renderer(class Window& window) noexcept;

    ~Renderer() noexcept;

    Renderer() noexcept = delete;

    Renderer(const Renderer& other) noexcept = delete;

    Renderer(Renderer&& other) noexcept = delete;

    Renderer& operator=(Renderer const& other) noexcept = delete;

    Renderer& operator=(Renderer&& other) noexcept = delete;

public:
    void swapBuffer() noexcept;

    const Window* getWindow() const noexcept
    {
        return m_pWindow;
    }

    Window* getWindow() noexcept
    {
        return m_pWindow;
    }
};
} // namespace GPE
