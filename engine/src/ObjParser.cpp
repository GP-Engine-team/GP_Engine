﻿#include "Engine/Core/Parsers/ObjParser.hpp"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure
#include <memory>               // std::make_unique

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Resources/Texture.hpp"
#include "GPM/Shape3D/AABB.hpp"

using namespace GPE;
using namespace GPM;

Model::CreateArg GPE::importeSingleModel(const char* assetPath, ResourceManagerType& resourceManager) noexcept
{
    GPE_ASSERT(assetPath != nullptr, "Void path");

    Log::logInitializationStart("Obj parsing");

    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(assetPath, aiProcess_Triangulate /*| aiProcess_JoinIdenticalVertices*/ |
                                                            aiProcess_SortByPType | aiProcess_GenNormals |
                                                            aiProcess_GenUVCoords | aiProcess_GenBoundingBoxes);
    if (!scene)
        FUNCT_ERROR(importer.GetErrorString());

    // SubModule initialization
    GPE_ASSERT(scene->HasMeshes(), "File without mesh");
    Model::CreateArg modelArg;
    modelArg.subModels.reserve(scene->mNumMeshes);

    // Material and texture
    GPE_ASSERT(scene->HasMaterials(), "Mesh without material not supported");

    std::vector<Material*> pMaterials;
    pMaterials.reserve(scene->mNumMaterials - 1);

    for (size_t i = 1; i < scene->mNumMaterials; ++i)
    {
        GPE_ASSERT(scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) != 0,
                   "No diffuse texture not supported");
        GPE_ASSERT(scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) < 2,
                   "Multiple diffuse trexture not supported");

        aiString diffuseTextureName;
        scene->mMaterials[i]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &diffuseTextureName);

        Texture::LoadArg textureArg;
        textureArg.path = PATH_TEXTURE_RESOURCE;
        textureArg.path += diffuseTextureName.C_Str();

        Material::CreateArg materialArg;

        materialArg.name = scene->mMaterials[i]->GetName().C_Str();

        aiColor3D color;
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color);
        materialArg.comp.ambient.rgbi = GPM::Vec4{color.r, color.g, color.b, 1.f};

        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        materialArg.comp.diffuse.rgbi = GPM::Vec4{color.r, color.g, color.b, 1.f};

        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color);
        materialArg.comp.specular.rgbi = GPM::Vec4{color.r, color.g, color.b, 1.f};

        scene->mMaterials[i]->Get(AI_MATKEY_SHININESS, materialArg.comp.shininess);
        scene->mMaterials[i]->Get(AI_MATKEY_OPACITY, materialArg.comp.opacity);

        materialArg.pTexture = &resourceManager.add<Texture>(diffuseTextureName.C_Str(), textureArg);

        pMaterials.emplace_back(&resourceManager.add<Material>(materialArg.name, materialArg));
    }

    // Mesh
    for (size_t i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh* pMesh = scene->mMeshes[i];

        Mesh::CreateArg arg;
        arg.vBuffer.reserve(pMesh->mNumVertices);
        arg.vtBuffer.reserve(pMesh->mNumVertices);
        arg.vnBuffer.reserve(pMesh->mNumVertices);

        arg.objName = pMesh->mName.C_Str();

        for (size_t verticeId = 0; verticeId < pMesh->mNumVertices; ++verticeId)
        {

            GPE_ASSERT(pMesh->mVertices != nullptr, "Mesh without vertices");
            GPE_ASSERT(pMesh->HasNormals(), "Mesh without Normal");
            GPE_ASSERT(pMesh->mTextureCoords != nullptr, "Mesh without UV");
            GPE_ASSERT(pMesh->mTextureCoords[0] != nullptr, "Invalid UV");

            const aiVector3D& vertice   = pMesh->mVertices[verticeId];
            const aiVector3D& textCoord = pMesh->mTextureCoords[0][verticeId];
            const aiVector3D& normal    = pMesh->mNormals[verticeId];

            arg.vBuffer.emplace_back(vertice.x, vertice.y, vertice.z);
            arg.vnBuffer.emplace_back(normal.x, normal.y, normal.z);
            arg.vtBuffer.emplace_back(textCoord.x, textCoord.y);
        }

        arg.boundingVolume =
            std::make_unique<AABB>(Vector3{pMesh->mAABB.mMin.x, pMesh->mAABB.mMin.y, pMesh->mAABB.mMin.z},
                                   Vector3{pMesh->mAABB.mMax.x, pMesh->mAABB.mMax.y, pMesh->mAABB.mMax.z});

        arg.boundingVolumeType = Mesh::BoundingVolume::AABB;

        modelArg.subModels.emplace_back(SubModel{nullptr, resourceManager.get<Shader>("TextureWithLihghts"),
                                                 pMaterials.empty() ? nullptr : pMaterials[pMesh->mMaterialIndex - 1],
                                                 &resourceManager.add<Mesh>(arg.objName, arg), true});
    }

    Log::logInitializationEnd("Obj parsing");

    return modelArg;
}
