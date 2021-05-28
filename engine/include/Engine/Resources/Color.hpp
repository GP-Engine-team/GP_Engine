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

    static constexpr ColorRGB red() noexcept
    {
        return {1.f, 0.f, 0.f};
    };

    static constexpr ColorRGB green() noexcept
    {
        return {0.f, 1.f, 0.f};
    };

    static constexpr ColorRGB blue() noexcept
    {
        return {0.f, 0.f, 1.f};
    };

    static constexpr ColorRGB white() noexcept
    {
        return {1.f, 1.f, 1.f};
    };

    static constexpr ColorRGB black() noexcept
    {
        return {0.f, 0.f, 0.f};
    };
};

struct ColorRGBA
{
    union {
        union {
            struct
            {
                ColorRGB rgb;
                float    a;
            };

            struct
            {
                float r, g, b, a;
            };
        };
        GPM::Vec4 v;
        float     e[4];
    };

    static constexpr ColorRGBA red() noexcept
    {
        return {1.f, 0.f, 0.f, 1.f};
    };

    static constexpr ColorRGBA green() noexcept
    {
        return {0.f, 1.f, 0.f, 1.f};
    };

    static constexpr ColorRGBA blue() noexcept
    {
        return {0.f, 0.f, 1.f, 1.f};
    };

    static constexpr ColorRGBA white() noexcept
    {
        return {1.f, 1.f, 1.f, 1.f};
    };

    static constexpr ColorRGBA black() noexcept
    {
        return {0.f, 0.f, 0.f, 1.f};
    };
};

using RGB  = ColorRGB;
using RGBA = ColorRGBA;

} /*namespace GPE*/
