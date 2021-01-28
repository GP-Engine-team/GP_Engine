/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */
#pragma once

#include "Engine/Core/Rendering/Renderer/IRenderer.hpp"

namespace Engine::Core::Renderering
{
class RendererGLFW_GL46 : public IRenderer
{
  protected:
    void initialize(const WindowCreateArg &arg) noexcept final;

    void release() noexcept final;

  public:
    inline RendererGLFW_GL46(const WindowCreateArg &arg) noexcept : IRenderer()
    {
        initialize(arg);
    }

    inline virtual ~RendererGLFW_GL46() noexcept
    {
        release();
    }

    RendererGLFW_GL46() noexcept = delete;

    RendererGLFW_GL46(const RendererGLFW_GL46 &other) noexcept = delete;

    RendererGLFW_GL46(RendererGLFW_GL46 &&other) noexcept = delete;

    RendererGLFW_GL46 &operator=(RendererGLFW_GL46 const &other) noexcept = delete;

    RendererGLFW_GL46 &operator=(RendererGLFW_GL46 &&other) noexcept = delete;

  public:
    void swapBuffer() noexcept final;
};
} // namespace Engine::Core::Renderering