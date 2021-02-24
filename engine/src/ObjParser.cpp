#include "Engine/Core/Parsers/ObjParser.hpp"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"

using namespace GPE;
using namespace GPM;

void GPE::importeAsset(const char* assetPath, ResourceManagerType& resourceManager) noexcept
{
    GPE_ASSERT(assetPath != nullptr, "Void path");

    Log::logInitializationStart("Obj parsing");

    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(assetPath, aiProcess_Triangulate /*| aiProcess_JoinIdenticalVertices*/| aiProcess_SortByPType |
                                         aiProcess_GenNormals | aiProcess_GenUVCoords);
    if (!scene)
        FUNCT_ERROR(importer.GetErrorString());

    //Material and texture
    GPE_ASSERT(scene->HasMaterials(), "Mesh without material not supported");

    for (size_t i = 1; i < scene->mNumMaterials; ++i)
    {
        std::cout << scene->mMaterials[i]->GetName().C_Str() << std::endl;
        std::cout << scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_NONE) << std::endl;
        std::cout << scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_UNKNOWN) << std::endl;
        std::cout << scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_AMBIENT) << std::endl;
        std::cout << scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_AMBIENT_OCCLUSION) << std::endl;
        std::cout << scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) << std::endl;
        std::cout << scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE_ROUGHNESS) << std::endl;

        GPE_ASSERT(scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) != 0, "No diffuse texture not supported");
        GPE_ASSERT(scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) < 2, "Multiple diffuse trexture not supported");

        aiString str;
        scene->mMaterials[i]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &str);

        std::cout << str.C_Str() << std::endl;

        
        //resourceManager.add<Texture>();


        Material::CreateArg arg;
        

        //resourceManager.add<Material>()
    }
    exit(10);

    //Mesh
    if (scene->HasMeshes())
    {
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

            resourceManager.add<Mesh>(arg.objName, arg);            
        }
    }

    Log::logInitializationEnd("Obj parsing");
}