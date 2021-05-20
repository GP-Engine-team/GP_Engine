﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/Model.hpp"
#include "Engine/Resources/Animation.hpp"
#include "Engine/Resources/Skeleton.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Serialization/SavedScene.hpp"

#define ENGINE_MESH_EXTENSION ".GPMesh"
#define ENGINE_MATERIAL_EXTENSION ".GPMaterial"
#define ENGINE_TEXTURE_EXTENSION ".GPTexture"
#define ENGINE_SHADER_EXTENSION ".GPShader"
#define ENGINE_PREFAB_EXTENSION ".GPPrefab"
#define ENGINE_SCENE_EXTENSION ".GPScene"
#define ENGINE_SKELETON_EXTENSION ".GPSkeleton"
#define ENGINE_ANIMATION_EXTENSION ".GPAnimation"

namespace GPE
{
enum class EFileType
{
    MESH      = 0,
    MATERIAL  = 1,
    TEXTURE   = 2,
    SHADER    = 3,
    SCENE     = 4,
    PREFAB    = 6,
    SKELETON  = 7,
    ANIMATION = 8
};

struct TextureImportConfig
{
    enum class EFormatType
    {
        PNG,
        COUNT
    } format = EFormatType::PNG;

    bool verticalFlip = false;

    std::string formatToString()
    {
        switch (format)
        {
        case EFormatType::PNG:
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
    uint16_t    featureMask        = 0u;
};

void saveSceneToPathImp(GPE::Scene* scene, const char* path, GPE::SavedScene::EType saveMode);
void loadSceneFromPathImp(GPE::Scene* scene, const char* path);

void importeModel(const char* srcPath, const char* dstPath,
                  Mesh::EBoundingVolume boundingVolumeType = Mesh::EBoundingVolume::SPHERE) noexcept;

void importeTextureFile(const char* srcPath, const char* dstPath, const TextureImportConfig& config = {});
void writeTextureFile(const char* dst, const Texture::ImportArg& arg, const TextureImportConfig& config = {});
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

void                  writePrefabFile(const char* dst, const SavedScene::CreateArg& arg = SavedScene::CreateArg{});
SavedScene::CreateArg readPrefabFile(const char* src);
SavedScene::CreateArg loadPrefabFile(const char* src);

void                  writeSceneFile(const char* dst, const SavedScene::CreateArg& arg);
SavedScene::CreateArg readSceneFile(const char* src);
SavedScene::CreateArg loadSceneFile(const char* src);

void                        writeSkeletonFile(const char* dst, const Skeleton::CreateArgs& arg);
Mesh::CreateIndiceBufferArg readSkeletonFile(const char* src);
Skeleton*                   loadSkeletonFile(const char* src);

void                        writeAnimationFile(const char* dst, const Animation::CreateArgs& arg);
Mesh::CreateIndiceBufferArg readAnimationFile(const char* src);
Animation*                  loadAnimationFile(const char* src);

} // namespace GPE
