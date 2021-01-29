/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

namespace Engine::Core::Renderering
{
struct WindowCreateArg
{
    std::string name = "";
    unsigned short height = 0;
    unsigned short width = 0;
};

class IWindow
{
protected:
    virtual void initialize(const WindowCreateArg& arg) noexcept = 0;

    virtual void release() noexcept = 0;

public:
    inline virtual const void* get() const noexcept = 0;
    inline virtual void* get() noexcept = 0;

    inline virtual void getSize(unsigned short& width, unsigned short& height) const noexcept = 0;
};
} // namespace Engine::Core::Renderering