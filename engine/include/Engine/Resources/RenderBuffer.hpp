/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Type.hpp"
#include <string>

// in inl
#include "Engine/Core/Debug/Assert.hpp"
#include "glad/glad.h"

namespace GPE
{

class RenderBuffer
{
public:
    enum class EInternalFormat
    {
        DEPTH_COMPONENT16  = GL_DEPTH_COMPONENT16,
        DEPTH_COMPONENT24  = GL_DEPTH_COMPONENT24,
        DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
        DEPTH24_STENCIL8   = GL_DEPTH24_STENCIL8,
        DEPTH32F_STENCIL8  = GL_DEPTH32F_STENCIL8,
        STENCIL_INDEX8     = GL_STENCIL_INDEX8
    };

    struct CreateArg
    {
        unsigned int    width          = 0;
        unsigned int    height         = 0;
        EInternalFormat internalFormat = EInternalFormat::DEPTH_COMPONENT16;
    };

protected:
    unsigned int m_id          = 0;

public:
    RenderBuffer()                          = default;
    RenderBuffer(const RenderBuffer& other) = delete;
    RenderBuffer(RenderBuffer&& other)      = default;

    RenderBuffer(const CreateArg& arg) noexcept;
    ~RenderBuffer() noexcept;

    inline unsigned int getID() const noexcept
    {
        return m_id;
    }
};

} /*namespace GPE*/