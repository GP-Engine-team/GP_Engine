/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Rendering/Window/IWindow.hpp"
#include <memory> //std::unique_ptr

namespace Engine::Core::Renderering
{
class IRenderer
{
  protected:
    std::unique_ptr<IWindow> m_pWindow = nullptr;

  protected:
    virtual void initialize(const WindowCreateArg &arg) noexcept = 0;
    virtual void release() noexcept = 0;

  public:

    [[nodiscard]] inline const IWindow const *getWindow() const noexcept
    {
        return m_pWindow.get();
    }

    virtual void swapBuffer() noexcept = 0;
};
} // namespace Engine::Core::Renderering