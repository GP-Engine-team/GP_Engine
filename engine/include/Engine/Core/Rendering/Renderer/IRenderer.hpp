/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Rendering/Window/IWindow.hpp"
#include <memory> //std::unique_ptr

namespace Engine::Core::Renderering
{
class IRenderer
{
  protected:
    IWindow* m_pWindow = nullptr;

  public:

    IRenderer(IWindow * pWin) noexcept 
        : m_pWindow {pWin}
    {
    //    GPE_ASSERT(m_pWindow != nullptr);
    }

    virtual void swapBuffer() noexcept = 0;
};
} // namespace Engine::Core::Renderering