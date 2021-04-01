/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/Model.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"

#define ENGINE_ASSET_EXTENSION ".GPAsset"

namespace GPE
{
enum class EFileType
{
    MESH     = 0,
    MATERIAL = 1,
    TEXTURE  = 2
};

struct TextureImportDataConfig
{
    std::string srcPath;
};

void importeModel(const char* srcPath, const char* dstPath) noexcept;

void writeTextureFile(const char* dst, const TextureImportDataConfig& arg);
void readTextureFile(const char* src, Texture::ImportArg& arg);

void writeMaterialFile(const char* dst, const Material::ImporteArg& arg);
void readMaterialFile(const char* src, Material::ImporteArg& arg);

void writeMeshFile(const char* dst, const Mesh::CreateIndiceBufferArg& arg);
void readMeshFile(const char* src, Mesh::CreateIndiceBufferArg& arg);

} // namespace GPE
