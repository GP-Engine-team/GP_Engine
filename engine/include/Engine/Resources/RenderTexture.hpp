/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

namespace GPE
{
class RenderTexture
{
public:
    struct CreateArg
    {
        unsigned int width  = 0;
        unsigned int height = 0;

    };

protected:
    unsigned int m_width  = 0;
    unsigned int m_height = 0;

    GLuint m_id         = 0;
    GLuint m_textureId  = 0;

public:
    RenderTexture(const CreateArg& arg) noexcept;

    RenderTexture() noexcept = delete;

    RenderTexture(const RenderTexture& other) noexcept = delete;

    RenderTexture(RenderTexture&& other) noexcept = delete;

    ~RenderTexture() noexcept;

    RenderTexture& operator=(RenderTexture const& other) noexcept = delete;

    RenderTexture& operator=(RenderTexture&& other) noexcept = delete;

    void resize(unsigned int width, unsigned int height) noexcept;
};

} /*namespace GPE*/