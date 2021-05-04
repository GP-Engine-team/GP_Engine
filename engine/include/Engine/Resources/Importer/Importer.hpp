﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/Model.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Serialization/SavedScene.hpp"

#define ENGINE_MESH_EXTENSION ".GPMesh"
#define ENGINE_MATERIAL_EXTENSION ".GPMaterial"
#define ENGINE_TEXTURE_EXTENSION ".GPTexture"
#define ENGINE_SHADER_EXTENSION ".GPShader"
#define ENGINE_SCENE_EXTENSION ".GPScene"

namespace GPE
{
enum class EFileType
{
    MESH     = 0,
    MATERIAL = 1,
    TEXTURE  = 2,
    SHADER   = 3,
    SCENE    = 4
};

struct TextureImportConfig
{
    enum class EFormatType
    {
        PNG,
        COUNT
    } format = EFormatType::PNG;

    bool verticalFlip = true;

    std::string formatToString()
    {
        switch (format)
        {
        case GPE::TextureImportConfig::EFormatType::PNG:
            return "png";
            break;
        default:
            break;
        }

        return "unknow";
    }

    std::vector<const char*> allFormatToString()
    {
        std::vector<const char*> rst;
        rst.emplace_back("png");
        return rst;
    }
};

struct ShaderCreateConfig
{
    std::string vertexShaderPath   = "";
    std::string fragmentShaderPath = "";
    uint16_t    featureMask        = 0;
};

void importeModel(const char* srcPath, const char* dstPath) noexcept;

void               importeTextureFile(const char* srcPath, const char* dstPath, const TextureImportConfig& config = {});
void               writeTextureFile(const char* dst, const Texture::ImportArg& arg);
Texture::ImportArg readTextureFile(const char* src);
Texture*           loadTextureFile(const char* src);

void                 writeMaterialFile(const char* dst, const Material::ImporteArg& arg = Material::ImporteArg{});
Material::ImporteArg readMaterialFile(const char* src);
Material*            loadMaterialFile(const char* src);

void                        writeMeshFile(const char* dst, const Mesh::CreateIndiceBufferArg& arg);
Mesh::CreateIndiceBufferArg readMeshFile(const char* src);
Mesh*                       loadMeshFile(const char* src);

void               writeShaderFile(const char* dst, const ShaderCreateConfig& arg = ShaderCreateConfig{});
ShaderCreateConfig readShaderFile(const char* src);
Shader*            loadShaderFile(const char* src);

void                  writeSceneFile(const char* dst, const SavedScene::CreateArg& arg);
SavedScene::CreateArg readSceneFile(const char* src);
SavedScene::CreateArg loadSceneFile(const char* src);

} // namespace GPE
