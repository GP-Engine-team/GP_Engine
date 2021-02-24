#include "Engine/Core/Parsers/ObjParser.hpp"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Resources/Texture.hpp"

using namespace GPE;
using namespace GPM;

std::vector<SubModel> GPE::importeSingleModel(const char* assetPath, ResourceManagerType& resourceManager) noexcept
{
    GPE_ASSERT(assetPath != nullptr, "Void path");

    Log::logInitializationStart("Obj parsing");

    Assimp::Importer importer;
    const aiScene*   scene =
        importer.ReadFile(assetPath, aiProcess_Triangulate /*| aiProcess_JoinIdenticalVertices*/ |
                                         aiProcess_SortByPType | aiProcess_GenNormals | aiProcess_GenUVCoords);
    if (!scene)
        FUNCT_ERROR(importer.GetErrorString());

    // SubModule initialization
    GPE_ASSERT(scene->HasMeshes(), "File without mesh");
    std::vector<SubModel> subModuleBuffer;
    subModuleBuffer.reserve(scene->mNumMeshes);

    // Material and texture
    GPE_ASSERT(scene->HasMaterials(), "Mesh without material not supported");

    std::vector<Material::CreateArg> matArgs;
    matArgs.reserve(scene->mNumMaterials - 1);

    for (size_t i = 1; i < scene->mNumMaterials; ++i)
    {
        GPE_ASSERT(scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) != 0,
                   "No diffuse texture not supported");
        GPE_ASSERT(scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) < 2,
                   "Multiple diffuse trexture not supported");

        aiString str;
        scene->mMaterials[i]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &str);

        Texture::LoadArg textureArg;
        textureArg.path = PATH_TEXTURE_RESOURCE;
        textureArg.path += str.C_Str();

        matArgs.emplace_back();
        Material::CreateArg& materialArg = matArgs.back();

        aiColor3D color;
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color);
        materialArg.comp.ambient.rgbi = GPM::Vec4{color.r, color.g, color.b, 1.f};

        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        materialArg.comp.diffuse.rgbi = GPM::Vec4{color.r, color.g, color.b, 1.f};

        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color);
        materialArg.comp.specular.rgbi = GPM::Vec4{color.r, color.g, color.b, 1.f};

        float scale;
        scene->mMaterials[i]->Get(AI_MATKEY_SHININESS_STRENGTH, scale);
        materialArg.comp.shininess = scale;

        materialArg.pTexture = &resourceManager.add<Texture>(str.C_Str(), textureArg);
    }

    std::vector<Material>& materials =
        resourceManager.add<std::vector<Material>>("Mat", matArgs.begin(), matArgs.end());

    // Mesh
    for (size_t i = 0; i < scene->mNumMeshes; ++i)
    {
        Mesh::CreateArg arg;
        arg.vBuffer.reserve(scene->mMeshes[i]->mNumVertices);
        arg.vtBuffer.reserve(scene->mMeshes[i]->mNumVertices);
        arg.vnBuffer.reserve(scene->mMeshes[i]->mNumVertices);

        arg.objName = scene->mMeshes[i]->mName.C_Str();

        for (size_t verticeId = 0; verticeId < scene->mMeshes[i]->mNumVertices; ++verticeId)
        {
            GPE_ASSERT(scene->mMeshes[i]->mVertices != nullptr, "Mesh without vertices");
            GPE_ASSERT(scene->mMeshes[i]->HasNormals(), "Mesh without Normal");
            GPE_ASSERT(scene->mMeshes[i]->mTextureCoords != nullptr, "Mesh without UV");
            GPE_ASSERT(scene->mMeshes[i]->mTextureCoords[0] != nullptr, "Invalid UV");

            const aiVector3D& vertice   = scene->mMeshes[i]->mVertices[verticeId];
            const aiVector3D& textCoord = scene->mMeshes[i]->mTextureCoords[0][verticeId];
            const aiVector3D& normal    = scene->mMeshes[i]->mNormals[verticeId];

            arg.vBuffer.emplace_back(vertice.x, vertice.y, vertice.z);
            arg.vnBuffer.emplace_back(normal.x, normal.y, normal.z);
            arg.vtBuffer.emplace_back(textCoord.x, textCoord.y);
        }

        bool enableBackFaceCulling = true;

        subModuleBuffer.emplace_back(SubModel{nullptr, resourceManager.get<Shader>("TextureWithLihghts"),
                                              &materials[scene->mMeshes[i]->mMaterialIndex + 1],
                                              &resourceManager.add<Mesh>(arg.objName, arg), true});
    }

    Log::logInitializationEnd("Obj parsing");

    return subModuleBuffer;
}