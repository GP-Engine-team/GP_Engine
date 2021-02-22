/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>
#include <glad/glad.h>

namespace GPE
{
class RenderTexture
{
public:
    struct CreateArg
    {
        std::vector<class Texture*> colorBuffers;
        class Texture* depthBuffer;
        class Texture* stencilBuffer;
    };

protected:

    GLuint m_id         = 0;

public:
    RenderTexture(const CreateArg& arg) noexcept;

    RenderTexture() noexcept = delete;

    RenderTexture(const RenderTexture& other) noexcept = delete;

    RenderTexture(RenderTexture&& other) noexcept = delete;

    ~RenderTexture() noexcept;

    RenderTexture& operator=(RenderTexture const& other) noexcept = delete;

    RenderTexture& operator=(RenderTexture&& other) noexcept = delete;
};

} /*namespace GPE*/