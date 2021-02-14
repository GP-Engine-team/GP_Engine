/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Color.hpp" //ColorRGBA
#include "GPM/Vector2.hpp"            //GPM::Vec2
#include "GPM/Vector3.hpp"            //GPM::Vec3
#include "GPM/Vector4.hpp"            //GPM::Vec4
#include <string>                     //std::string
#include <vector>                     //std::vector

namespace Engine::Resources
{
struct Indice
{
    unsigned int iv, ivt, ivn;
};

typedef union {
    struct
    {
        float kr, kg, kb, ki;
    };
    float     e[4];
    GPM::Vec4 rgbi;
    ColorRGBA rgba;

} AmbiantComponent;

typedef AmbiantComponent DiffuseComponent;
typedef AmbiantComponent SpecularComponent;
typedef AmbiantComponent Emission;

struct MaterialAttrib
{
    AmbiantComponent  ambient;
    DiffuseComponent  diffuse;
    SpecularComponent specular;
    Emission          emission;
    float             shininess;

    std::string nameAmbiantTexture;
    std::string nameDiffuseTexture;
    std::string nameSpecularTexture;

    std::string id;
};

struct Shape
{
    std::string               name;
    std::vector<unsigned int> iv;
    std::vector<unsigned int> ivn;
    std::vector<unsigned int> ivt;
    std::string               material_ids;
};

struct Attrib
{
    std::string            objName;
    std::vector<GPM::Vec3> vBuffer;
    std::vector<GPM::Vec2> vtBuffer;
    std::vector<GPM::Vec3> vnBuffer;

    void clear()
    {
        objName.clear();
        vBuffer.clear();
        vtBuffer.clear();
        vnBuffer.clear();
    }
};

struct Size
{
    int width, height;
};
} /*namespace Engine::Resources*/