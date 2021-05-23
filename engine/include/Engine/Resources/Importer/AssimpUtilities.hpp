/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include <GPM/Matrix4.hpp>

namespace GPE
{

GPM::Mat4 mat4aiToGPM(const aiMatrix4x4& aiMat)
{
    GPM::Mat4 m;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m.e[i * 4 + j] = aiMat[j][i];
        }
    }
    return m;
}

}