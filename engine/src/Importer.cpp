#include "Engine/Resources/Importer/Importer.hpp"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure
#include <cstdio>
#include <filesystem>
#include <memory> // std::make_unique

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"
#include "Engine/Resources/Texture.hpp"
#include "GPM/Shape3D/AABB.hpp"
#include "GPM/Shape3D/Sphere.hpp"

using namespace GPE;
using namespace GPM;

void GPE::importeModel(const char* srcPath, const char* dstPath) noexcept
{
    GPE_ASSERT(srcPath != nullptr, "Void path");

    Log::getInstance()->logInitializationStart("Model importation");

    ResourceManagerType& resourceManager = Engine::getInstance()->resourceManager;

    unsigned int postProcessflags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType |
                                    aiProcess_GenNormals | aiProcess_GenUVCoords;

    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(srcPath, postProcessflags);
    if (!scene)
        FUNCT_ERROR(importer.GetErrorString());

    // SubModule initialization
    GPE_ASSERT(scene->HasMeshes(), "File without mesh");

    // Material and texture
    GPE_ASSERT(scene->HasMaterials(), "Mesh without material not supported");

    std::filesystem::path srcDirPath(srcPath);
    srcDirPath = srcDirPath.parent_path();

    std::filesystem::path dstDirPath(dstPath);

    for (size_t i = 1; i < scene->mNumMaterials; ++i)
    {
        GPE_ASSERT(scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) != 0,
                   "No diffuse texture not supported");
        GPE_ASSERT(scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) < 2,
                   "Multiple diffuse trexture not supported");

        Material::ImporteArg materialArg;

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

        aiString diffuseTextureName;
        scene->mMaterials[i]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &diffuseTextureName);
        materialArg.diffuseTextureName = diffuseTextureName.C_Str();

        TextureImportDataConfig textureArg;

        std::filesystem::path dstTexturePath = dstDirPath;
        std::filesystem::path srcTexturePath = srcDirPath;
        std::filesystem::path diffuseTextureFile(materialArg.diffuseTextureName);
        dstTexturePath /= diffuseTextureFile.stem();
        dstTexturePath += ENGINE_TEXTURE_EXTENSION;
        srcTexturePath /= diffuseTextureFile;

        textureArg.srcPath = srcTexturePath.string();

        materialArg.diffuseTextureName = dstTexturePath.string().c_str();

        std::filesystem::path dstMaterialPath = dstDirPath;
        dstMaterialPath /= materialArg.name;
        dstMaterialPath += ENGINE_MATERIAL_EXTENSION;

        writeTextureFile(dstTexturePath.string().c_str(), textureArg);
        writeMaterialFile(dstMaterialPath.string().c_str(), materialArg);
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

        std::filesystem::path dstMeshPath = dstDirPath;
        dstMeshPath /= arg.objName;
        dstMeshPath += ENGINE_MESH_EXTENSION;

        writeMeshFile(dstMeshPath.string().c_str(), arg);
    }

    Log::getInstance()->logInitializationEnd("Model importion");
}

struct TextureHeader
{
    char assetID       = (char)EFileType::TEXTURE;
    int  textureLenght = 0;
};

void GPE::writeTextureFile(const char* dst, const TextureImportDataConfig& arg)
{
    stbi_set_flip_vertically_on_load(true);

    int            w, h, comp;
    unsigned char* pixels = stbi_load(arg.srcPath.c_str(), &w, &h, &comp, 0);

    if (pixels == nullptr)
    {
        FUNCT_ERROR((std::string("STBI cannot load image: ") + arg.srcPath).c_str());
        FUNCT_ERROR(std::string("Reason: ") + stbi_failure_reason());
        return;
    }

    int            len;
    unsigned char* png = stbi_write_png_to_mem((const unsigned char*)pixels, w * comp, w, h, comp, &len);

    if (png == NULL)
    {
        FUNCT_ERROR((std::string("STBI cannot write image with png format with this path : ") + arg.srcPath).c_str());
    }

    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    TextureHeader header{(char)EFileType::TEXTURE, len};
    fwrite(&header, sizeof(header), 1, pFile); // header

    fwrite(png, len, 1, pFile);
    fclose(pFile);
    stbi_image_free(pixels);
    STBIW_FREE(png);
}

Texture::ImportArg GPE::readTextureFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);
    Texture::ImportArg    arg;

    if (fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    if (srcPath.extension() == ENGINE_TEXTURE_EXTENSION)
    {
        TextureHeader header;

        // copy the file into the buffer:
        fread(&header, sizeof(header), 1, pFile);

        std::vector<stbi_uc> texBuffer;
        texBuffer.assign(header.textureLenght, 0);

        fread(&texBuffer[0], sizeof(stbi_uc), header.textureLenght, pFile); // Texture buffer

        arg.pixels = stbi_load_from_memory(texBuffer.data(), header.textureLenght, &arg.w, &arg.h, &arg.comp, 0);
    }
    else
    {
        arg.pixels = stbi_load_from_file(pFile, &arg.w, &arg.h, &arg.comp, 0);
    }

    if (!arg.pixels)
    {
        Log::getInstance()->logError(stringFormat("Fail to load file \"%s\"", src));
    }

    return arg;
}

Texture* GPE::loadTextureFile(const char* src)
{
    std::filesystem::path srcPath(src);
    return &Engine::getInstance()->resourceManager.add<Texture>(srcPath.filename().string(), readTextureFile(src));
}

struct MaterialHeader
{
    char              assetID                  = (char)EFileType::MATERIAL;
    MaterialComponent component                = {};
    int               nameLenght               = 0;
    int               nameDiffuseTextureLenght = 0;
};

void GPE::writeMaterialFile(const char* dst, const Material::ImporteArg& arg)
{
    FILE* pFile = nullptr;
    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    MaterialHeader header{(char)EFileType::MATERIAL, arg.comp, arg.name.size(), arg.diffuseTextureName.size()};
    fwrite(&header, sizeof(header), 1, pFile);                                                   // header
    fwrite(arg.name.data(), sizeof(char), header.nameLenght, pFile);                             // string buffer
    fwrite(arg.diffuseTextureName.data(), sizeof(char), header.nameDiffuseTextureLenght, pFile); // string buffer

    fclose(pFile);
}

Material* GPE::loadMaterialFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);

    if (srcPath.extension() != ENGINE_MATERIAL_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return nullptr;
    }

    MaterialHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);

    Material::CreateArg arg;
    arg.name.assign(header.nameLenght, '\0');
    std::string diffusePath(header.nameDiffuseTextureLenght, '\0');

    fread(&arg.name[0], sizeof(char), header.nameLenght, pFile); // string buffer

    if (header.nameDiffuseTextureLenght) // If diffuse texture existe
    {
        fread(diffusePath.data(), sizeof(char), header.nameDiffuseTextureLenght, pFile); // string buffer
        arg.pTexture = loadTextureFile(diffusePath.c_str());
    }

    return &Engine::getInstance()->resourceManager.add<Material>(srcPath.filename().string(), arg);
}

struct MeshHeader
{
    char assetID       = (char)EFileType::MESH;
    int  nameLenght    = 0;
    int  verticeLenght = 0;
    int  indiceLenght  = 0;
};

void GPE::writeMeshFile(const char* dst, const Mesh::CreateIndiceBufferArg& arg)
{
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    MeshHeader header{(char)EFileType::MESH, arg.objName.size(), arg.vertices.size(), arg.indices.size()};
    fwrite(&header, sizeof(header), 1, pFile);                                         // header
    fwrite(arg.objName.data(), sizeof(char), header.nameLenght, pFile);                // string buffer
    fwrite(arg.vertices.data(), sizeof(arg.vertices[0]), header.verticeLenght, pFile); // vertice buffer
    fwrite(arg.indices.data(), sizeof(arg.indices[0]), header.indiceLenght, pFile);    // indice buffer

    fclose(pFile);
}

Mesh* GPE::loadMeshFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);

    if (srcPath.extension() != ENGINE_MESH_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return nullptr;
    }

    MeshHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);

    Mesh::CreateIndiceBufferArg arg;
    arg.objName.assign(header.nameLenght, '\0');
    arg.vertices.assign(header.verticeLenght, Mesh::Vertex{});
    arg.indices.assign(header.indiceLenght, 0);

    fread(&arg.objName[0], sizeof(char), header.nameLenght, pFile);                // string buffer
    fread(&arg.vertices[0], sizeof(arg.vertices[0]), header.verticeLenght, pFile); // vertice buffer
    fread(&arg.indices[0], sizeof(arg.indices[0]), header.indiceLenght, pFile);    // indice buffer

    return &Engine::getInstance()->resourceManager.add<Mesh>(srcPath.filename().string(), arg);
}

struct ShadeHeader
{
    char assetID            = (char)EFileType::SHADER;
    int  vertexPathLenght   = 0;
    int  fragmentPathLenght = 0;
};

void GPE::writeShaderFile(const char* dst, const ShaderCreateonfig& arg)
{
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    ShadeHeader header{(char)EFileType::SHADER, arg.vertexShaderPath.size(), arg.fragmentShaderPath.size()};
    fwrite(&header, sizeof(header), 1, pFile);                                                 // header
    fwrite(arg.vertexShaderPath.data(), sizeof(char), arg.vertexShaderPath.size(), pFile);     // string buffer
    fwrite(arg.fragmentShaderPath.data(), sizeof(char), arg.fragmentShaderPath.size(), pFile); // string buffer

    fclose(pFile);
}

Shader* GPE::loadShaderFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);

    if (srcPath.extension() != ENGINE_SHADER_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return nullptr;
    }

    ShadeHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);

    std::string vertexShaderPath(header.vertexPathLenght, '\0');
    fread(vertexShaderPath.data(), sizeof(char), header.vertexPathLenght, pFile); // string buffer

    std::string fragmentShaderPath(header.fragmentPathLenght, '\0');
    fread(fragmentShaderPath.data(), sizeof(char), header.fragmentPathLenght, pFile); // string buffer

    return &Engine::getInstance()->resourceManager.add<Shader>(srcPath.filename().string(), vertexShaderPath.c_str(),
                                                               fragmentShaderPath.c_str());
}