/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */
#pragma once

#include "Engine/Core/Rendering/Renderer/IRenderer.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"

namespace Engine::Core::Renderering
{
class RendererGLFW_GL46 final : public IRenderer
{
public:
    RendererGLFW_GL46(WindowGLFW& window) noexcept;

    ~RendererGLFW_GL46() noexcept;

    RendererGLFW_GL46() noexcept = delete;

    RendererGLFW_GL46(const RendererGLFW_GL46& other) noexcept = delete;

    RendererGLFW_GL46(RendererGLFW_GL46&& other) noexcept = delete;

    RendererGLFW_GL46& operator=(RendererGLFW_GL46 const& other) noexcept = delete;

    RendererGLFW_GL46& operator=(RendererGLFW_GL46&& other) noexcept = delete;

public:
    void swapBuffer() noexcept final;
};
} // namespace Engine::Core::Renderering