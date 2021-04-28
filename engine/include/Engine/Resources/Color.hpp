/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <GPM/Vector3.hpp>
#include <GPM/Vector4.hpp>

namespace GPE
{

struct ColorRGB
{
    union {
        struct
        {
            float r, g, b;
        };
        GPM::Vec3 v;
        float     e[3];
    };
};

struct ColorRGBA
{
    union {
        struct
        {
            float r, g, b, a;
        };
        GPM::Vec4 v;
        float     e[4];
    };
};

using RGB  = ColorRGB;
using RGBA = ColorRGBA;

} /*namespace GPE*/
