/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "gpm/Vector3.hpp"
#include "Engine/Resources/Type.hpp"

namespace Engine::Resources
{
    struct alignas(16) MaterialComponent
    {
        AmbiantComponent    ambient;
        DiffuseComponent    diffuse;
        SpecularComponent   specular;
        float               shininess;
    };

    // Same memory layout than 'struct light' in glsl shader
    // alignas to Vec4 at 16o. Pading must be add if alignas is not respected (i.e : Vec3 + pad)
    struct alignas(16) light
    {
        AmbiantComponent Ambient;
        DiffuseComponent Diffuse;
        SpecularComponent Specular;

        GPM::Vec3 lightPosition; float lightype;

        //Point arg
        float constant, linear, quadratic, cutOffExponent; //Pad1 for alignas at 16o

        //Spot arg
        GPM::Vec3 direction;
        float cutOff; // specifies the spotlight's radius.
    };
} /*namespace Engine::Resources*/