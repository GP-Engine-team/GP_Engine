/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <glad/glad.h>

#pragma once

namespace GPE
{
class Light;

struct ShadowMap
{
    unsigned int depthMapFBO = 0;
    unsigned int depthMap    = 0;
    Light*       pOwner      = nullptr;
    unsigned int height      = 1;
    unsigned int width       = 1;

    ShadowMap(Light& pOwner);
    ~ShadowMap();

    void init(unsigned int w, unsigned int h);

    void resize(unsigned int w, unsigned int h);
};
} // namespace GPE