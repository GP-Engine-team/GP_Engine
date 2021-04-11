/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/Model.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"

#define ENGINE_MESH_EXTENSION ".GPMesh"
#define ENGINE_MATERIAL_EXTENSION ".GPMaterial"
#define ENGINE_TEXTURE_EXTENSION ".GPTexture"
#define ENGINE_SHADER_EXTENSION ".GPShader"

namespace GPE
{
enum class EFileType
{
    MESH     = 0,
    MATERIAL = 1,
    TEXTURE  = 2,
    SHADER   = 3
};

struct TextureImportDataConfig
{
    std::string srcPath;
};

struct ShaderCreateonfig
{
    std::string   vertexShaderPath;
    std::string   fragmentShaderPath;
    unsigned char featureMask;
};

void importeModel(const char* srcPath, const char* dstPath) noexcept;

void               importeTextureFile(const char* srcPath, const char* dstPath);
void               writeTextureFile(const char* dst, const TextureImportDataConfig& arg);
Texture::ImportArg readTextureFile(const char* src);
Texture*           loadTextureFile(const char* src);

void                 writeMaterialFile(const char* dst, const Material::ImporteArg& arg = Material::ImporteArg{});
Material::ImporteArg readMaterialFile(const char* src);
Material*            loadMaterialFile(const char* src);

void                        writeMeshFile(const char* dst, const Mesh::CreateIndiceBufferArg& arg);
Mesh::CreateIndiceBufferArg readMeshFile(const char* src);
Mesh*                       loadMeshFile(const char* src);

void              writeShaderFile(const char* dst, const ShaderCreateonfig& arg = ShaderCreateonfig{});
ShaderCreateonfig readShaderFile(const char* src);
Shader*           loadShaderFile(const char* src);

} // namespace GPE
