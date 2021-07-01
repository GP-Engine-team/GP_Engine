/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Core/Debug/Log.hpp>
#include <glad/glad.h>

namespace GPE
{
size_t getTypeSize(GLenum type)
{
    switch (type)
    {
    case (GL_FLOAT):
        return sizeof(float);
    case (GL_FLOAT_VEC2):
        return sizeof(float) * 2;
    case (GL_FLOAT_VEC3):
        return sizeof(float) * 3;
    case (GL_FLOAT_VEC4):
        return sizeof(float) * 4;
    case (GL_FLOAT_MAT2):
        return sizeof(float) * 4;
    case (GL_FLOAT_MAT3):
        return sizeof(float) * 9;
    case (GL_FLOAT_MAT4):
        return sizeof(float) * 16;
    case (GL_INT):
        return sizeof(int);
    case (GL_BOOL):
        return sizeof(bool);
    case (GL_DOUBLE):
        return sizeof(double);
    case (GL_DOUBLE_VEC2):
        return sizeof(double) * 2;
    case (GL_DOUBLE_VEC3):
        return sizeof(double) * 3;
    case (GL_DOUBLE_VEC4):
        return sizeof(double) * 4;
    case (GL_DOUBLE_MAT2):
        return sizeof(double) * 4;
    case (GL_DOUBLE_MAT3):
        return sizeof(double) * 9;
    case (GL_INT_VEC2):
        return sizeof(int) * 2;
    case (GL_INT_VEC3):
        return sizeof(int) * 3;
    case (GL_INT_VEC4):
        return sizeof(int) * 4;
    case (GL_UNSIGNED_INT):
        return sizeof(unsigned int);
    case (GL_UNSIGNED_INT_VEC2):
        return sizeof(unsigned int) * 2;
    case (GL_UNSIGNED_INT_VEC3):
        return sizeof(unsigned int) * 3;
    case (GL_UNSIGNED_INT_VEC4):
        return sizeof(unsigned int) * 4;
    case (GL_FLOAT_MAT3x2):
    case (GL_FLOAT_MAT2x3):
        return sizeof(float) * 6;
    case (GL_FLOAT_MAT4x2):
    case (GL_FLOAT_MAT2x4):
        return sizeof(float) * 8;
    case (GL_FLOAT_MAT4x3):
    case (GL_FLOAT_MAT3x4):
        return sizeof(float) * 12;
    case (GL_DOUBLE_MAT4):
        return sizeof(double) * 16;
    case (GL_DOUBLE_MAT3x2):
    case (GL_DOUBLE_MAT2x3):
        return sizeof(double) * 6;
    case (GL_DOUBLE_MAT4x2):
    case (GL_DOUBLE_MAT2x4):
        return sizeof(double) * 8;
    case (GL_DOUBLE_MAT4x3):
    case (GL_DOUBLE_MAT3x4):
        return sizeof(double) * 12;
    case (GL_BOOL_VEC2):
        return sizeof(bool) * 2;
    case (GL_BOOL_VEC3):
        return sizeof(bool) * 3;
    case (GL_BOOL_VEC4):
        return sizeof(bool) * 4;
    default:
        FUNCT_WARNING(stringFormat("Enum \"%d\" not implemented. Size 0 returned", type));
        return 0;
    }
}

size_t getTypeBaseCount(GLenum type)
{
    switch (type)
    {
    case (GL_FLOAT):
        return 1;
    case (GL_FLOAT_VEC2):
        return 2;
    case (GL_FLOAT_VEC3):
        return 3;
    case (GL_FLOAT_VEC4):
        return 4;
    case (GL_FLOAT_MAT2):
        return 4;
    case (GL_FLOAT_MAT3):
        return 9;
    case (GL_FLOAT_MAT4):
        return 16;
    case (GL_INT):
        return 1;
    case (GL_BOOL):
        return 1;
    case (GL_DOUBLE):
        return 1;
    case (GL_DOUBLE_VEC2):
        return 2;
    case (GL_DOUBLE_VEC3):
        return 3;
    case (GL_DOUBLE_VEC4):
        return 4;
    case (GL_DOUBLE_MAT2):
        return 4;
    case (GL_DOUBLE_MAT3):
        return 9;
    case (GL_INT_VEC2):
        return 2;
    case (GL_INT_VEC3):
        return 3;
    case (GL_INT_VEC4):
        return 4;
    case (GL_UNSIGNED_INT):
        return 1;
    case (GL_UNSIGNED_INT_VEC2):
        return 2;
    case (GL_UNSIGNED_INT_VEC3):
        return 3;
    case (GL_UNSIGNED_INT_VEC4):
        return 4;
    case (GL_FLOAT_MAT3x2):
    case (GL_FLOAT_MAT2x3):
        return 6;
    case (GL_FLOAT_MAT4x2):
    case (GL_FLOAT_MAT2x4):
        return 8;
    case (GL_FLOAT_MAT4x3):
    case (GL_FLOAT_MAT3x4):
        return 12;
    case (GL_DOUBLE_MAT4):
        return 16;
    case (GL_DOUBLE_MAT3x2):
    case (GL_DOUBLE_MAT2x3):
        return 6;
    case (GL_DOUBLE_MAT4x2):
    case (GL_DOUBLE_MAT2x4):
        return 8;
    case (GL_DOUBLE_MAT4x3):
    case (GL_DOUBLE_MAT3x4):
        return 12;
    case (GL_BOOL_VEC2):
        return 2;
    case (GL_BOOL_VEC3):
        return 3;
    case (GL_BOOL_VEC4):
        return 4;
    default:
        FUNCT_WARNING(stringFormat("Enum \"%d\" not implemented. 0 returned", type));
        return 0;
    }
}

GLenum getTypeBase(GLenum type)
{
    switch (type)
    {
    case (GL_FLOAT):
        return GL_FLOAT;
    case (GL_FLOAT_VEC2):
        return GL_FLOAT;
    case (GL_FLOAT_VEC3):
        return GL_FLOAT;
    case (GL_FLOAT_VEC4):
        return GL_FLOAT;
    case (GL_FLOAT_MAT2):
        return GL_FLOAT;
    case (GL_FLOAT_MAT3):
        return GL_FLOAT;
    case (GL_FLOAT_MAT4):
        return GL_FLOAT;
    case (GL_INT):
        return GL_INT;
    case (GL_BOOL):
        return GL_BOOL;
    case (GL_DOUBLE):
        return GL_DOUBLE;
    case (GL_DOUBLE_VEC2):
        return GL_DOUBLE;
    case (GL_DOUBLE_VEC3):
        return GL_DOUBLE;
    case (GL_DOUBLE_VEC4):
        return GL_DOUBLE;
    case (GL_DOUBLE_MAT2):
        return GL_DOUBLE;
    case (GL_DOUBLE_MAT3):
        return GL_DOUBLE;
    case (GL_INT_VEC2):
        return GL_INT;
    case (GL_INT_VEC3):
        return GL_INT;
    case (GL_INT_VEC4):
        return GL_INT;
    case (GL_UNSIGNED_INT):
        return GL_UNSIGNED_INT;
    case (GL_UNSIGNED_INT_VEC2):
        return GL_UNSIGNED_INT;
    case (GL_UNSIGNED_INT_VEC3):
        return GL_UNSIGNED_INT;
    case (GL_UNSIGNED_INT_VEC4):
        return GL_UNSIGNED_INT;
    case (GL_FLOAT_MAT3x2):
    case (GL_FLOAT_MAT2x3):
        return GL_FLOAT;
    case (GL_FLOAT_MAT4x2):
    case (GL_FLOAT_MAT2x4):
        return GL_FLOAT;
    case (GL_FLOAT_MAT4x3):
    case (GL_FLOAT_MAT3x4):
        return GL_FLOAT;
    case (GL_DOUBLE_MAT4):
        return GL_DOUBLE;
    case (GL_DOUBLE_MAT3x2):
    case (GL_DOUBLE_MAT2x3):
        return GL_DOUBLE;
    case (GL_DOUBLE_MAT4x2):
    case (GL_DOUBLE_MAT2x4):
        return GL_DOUBLE;
    case (GL_DOUBLE_MAT4x3):
    case (GL_DOUBLE_MAT3x4):
        return GL_DOUBLE;
    case (GL_BOOL_VEC2):
        return GL_BOOL;
    case (GL_BOOL_VEC3):
        return GL_BOOL;
    case (GL_BOOL_VEC4):
        return GL_BOOL;
    default:
        FUNCT_WARNING(stringFormat("Enum \"%d\" not implemented. 0 returned", type));
        return 0;
    }
}

} // namespace GPE