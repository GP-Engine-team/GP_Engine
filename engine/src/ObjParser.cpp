#include "Engine/Core/Parsers/ObjParser.hpp"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure
#include <memory>               // std::make_unique

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"
#include "Engine/Resources/Texture.hpp"
#include "GPM/Shape3D/AABB.hpp"
#include "GPM/Shape3D/Sphere.hpp"

using namespace GPE;
using namespace GPM;

Model::CreateArg GPE::importeSingleModel(const char* assetPath, Mesh::EBoundingVolume boundingVolumeType) noexcept
{
    GPE_ASSERT(assetPath != nullptr, "Void path");

    Log::getInstance()->logInitializationStart("Obj parsing");

    ResourceManagerType& resourceManager = Engine::getInstance()->resourceManager;

    unsigned int postProcessflags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType |
                                    aiProcess_GenNormals | aiProcess_GenUVCoords;

    if (boundingVolumeType != Mesh::EBoundingVolume::NONE)
        postProcessflags |= aiProcess_GenBoundingBoxes;

    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(assetPath, postProcessflags);
    if (!scene)
        FUNCT_ERROR(importer.GetErrorString());

    // SubModule initialization
    GPE_ASSERT(scene->HasMeshes(), "File without mesh");
    Model::CreateArg modelArg;
    // modelArg.subModels.reserve(scene->mNumMeshes); //Only if std::vector

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

        Mesh::CreateIndiceBufferArg arg;
        arg.vertices.reserve(pMesh->mNumVertices);
        arg.indices.reserve(pMesh->mNumFaces * 3u);

        arg.objName = pMesh->mName.C_Str();

        // Vertices
        for (size_t verticeId = 0; verticeId < pMesh->mNumVertices; ++verticeId)
        {
            GPE_ASSERT(pMesh->mVertices != nullptr, "Mesh without vertices");
            GPE_ASSERT(pMesh->HasNormals(), "Mesh without Normal");
            GPE_ASSERT(pMesh->mTextureCoords != nullptr, "Mesh without UV");
            GPE_ASSERT(pMesh->mTextureCoords[0] != nullptr, "Invalid UV");

            const aiVector3D& vertice   = pMesh->mVertices[verticeId];
            const aiVector3D& textCoord = pMesh->mTextureCoords[0][verticeId];
            const aiVector3D& normal    = pMesh->mNormals[verticeId];

            arg.vertices.emplace_back(Mesh::Vertex{Vec3{vertice.x, vertice.y, vertice.z},
                                                   Vec3{normal.x, normal.y, normal.z}, Vec2{textCoord.x, textCoord.y}});
        }

        // Indices
        for (size_t i = 0; i < pMesh->mNumFaces; ++i)
        {
            arg.indices.emplace_back(pMesh->mFaces[i].mIndices[0]);
            arg.indices.emplace_back(pMesh->mFaces[i].mIndices[1]);
            arg.indices.emplace_back(pMesh->mFaces[i].mIndices[2]);
        }

        arg.boundingVolumeType = boundingVolumeType;

        // Generate bounding volume
        switch (boundingVolumeType)
        {
        case Mesh::EBoundingVolume::SPHERE: {

            const Vector3 min{pMesh->mAABB.mMin.x, pMesh->mAABB.mMin.y, pMesh->mAABB.mMin.z};
            const Vector3 max{pMesh->mAABB.mMax.x, pMesh->mAABB.mMax.y, pMesh->mAABB.mMax.z};

            arg.boundingVolume = std::make_unique<Sphere>(std::max(min.length(), max.length()), (max + min) * 0.5);

            break;
        }

        case Mesh::EBoundingVolume::AABB: {
            arg.boundingVolume =
                std::make_unique<AABB>(Vector3{pMesh->mAABB.mMin.x, pMesh->mAABB.mMin.y, pMesh->mAABB.mMin.z},
                                       Vector3{pMesh->mAABB.mMax.x, pMesh->mAABB.mMax.y, pMesh->mAABB.mMax.z});

            break;
        }
        default:
            break;
        }

        modelArg.subModels.emplace_back(SubModel{nullptr, resourceManager.get<Shader>("TextureWithLihghts"),
                                                 pMaterials.empty() ? nullptr : pMaterials[pMesh->mMaterialIndex - 1],
                                                 &resourceManager.add<Mesh>(arg.objName, arg), true});
    }

    Log::getInstance()->logInitializationEnd("Obj parsing");

    return modelArg;
}
