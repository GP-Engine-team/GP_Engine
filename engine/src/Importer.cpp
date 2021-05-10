#include <Engine/Resources/Importer/Importer.hpp>

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

#include <Engine/Core/Debug/Assert.hpp>
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Resources/ResourcesManagerType.hpp>
#include <Engine/Resources/Texture.hpp>
#include <GPM/Shape3D/AABB.hpp>
#include <GPM/Shape3D/Sphere.hpp>

#include <Engine/Core/Tools/Hash.hpp>

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
    {
        FUNCT_ERROR(importer.GetErrorString());
        return;
    }

    std::filesystem::path srcDirPath(srcPath);
    srcDirPath = srcDirPath.parent_path();

    std::filesystem::path dstDirPath(dstPath);

    for (size_t i = 0; i < scene->mNumMaterials; ++i)
    {
        Material::ImporteArg materialArg;

        aiColor3D color;
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color);
        materialArg.comp.ambient.rgbi = GPM::Vec4{color.r, color.g, color.b, 1.f};

        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        materialArg.comp.diffuse.rgbi = GPM::Vec4{color.r, color.g, color.b, 1.f};

        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color);
        materialArg.comp.specular.rgbi = GPM::Vec4{color.r, color.g, color.b, 1.f};

        scene->mMaterials[i]->Get(AI_MATKEY_SHININESS, materialArg.comp.shininess);
        scene->mMaterials[i]->Get(AI_MATKEY_OPACITY, materialArg.comp.opacity);

        TextureImportConfig   textureArg;
        aiString              path;
        std::filesystem::path fsPath;
        std::filesystem::path fsDstPath;
        std::filesystem::path fsSrcPath;

        for (unsigned int iText = 0;
             iText < scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_AMBIENT); ++iText)
        {
            scene->mMaterials[i]->GetTexture(aiTextureType::aiTextureType_AMBIENT, iText, &path);

            if (const aiTexture* texture = scene->GetEmbeddedTexture(path.C_Str()))
            {
                Texture::ImportArg arg;

                if (texture->mHeight)
                {
                    arg.len = texture->mHeight * texture->mWidth * sizeof(texture->pcData);
                }
                else
                {
                    arg.len = texture->mWidth;
                }
                arg.pixels.reset((unsigned char*)texture->pcData);
                arg.flipTexture = true;

                fsDstPath = dstDirPath / std::filesystem::path(texture->mFilename.C_Str()).stem();
                fsDstPath.replace_extension(ENGINE_TEXTURE_EXTENSION);
                materialArg.ambianteTexturePath = std::filesystem::relative(fsDstPath).string().c_str();

                writeTextureFile(fsDstPath.string().c_str(), arg);
                arg.pixels.release(); // Assimp manage it's mamory
            }
            else
            {
                fsPath    = path.C_Str();
                fsSrcPath = srcDirPath / fsPath;

                fsPath.replace_extension(ENGINE_TEXTURE_EXTENSION);
                fsDstPath = dstDirPath / fsPath;

                materialArg.ambianteTexturePath = std::filesystem::relative(fsDstPath).string().c_str();

                importeTextureFile(fsSrcPath.string().c_str(), fsDstPath.string().c_str(), textureArg);
            }
        }

        for (unsigned int iText = 0;
             iText < scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE); ++iText)
        {
            scene->mMaterials[i]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, iText, &path);

            if (const aiTexture* texture = scene->GetEmbeddedTexture(path.C_Str()))
            {
                Texture::ImportArg arg;

                if (texture->mHeight)
                {
                    arg.len = texture->mHeight * texture->mWidth * sizeof(texture->pcData);
                }
                else
                {
                    arg.len = texture->mWidth;
                }
                arg.pixels.reset((unsigned char*)texture->pcData);
                arg.flipTexture = true;

                fsDstPath = dstDirPath / std::filesystem::path(texture->mFilename.C_Str()).stem();
                fsDstPath.replace_extension(ENGINE_TEXTURE_EXTENSION);
                materialArg.diffuseTexturePath = std::filesystem::relative(fsDstPath).string().c_str();

                writeTextureFile(fsDstPath.string().c_str(), arg);
                arg.pixels.release(); // Assimp manage it's mamory
            }
            else
            {
                fsPath    = path.C_Str();
                fsSrcPath = srcDirPath / fsPath;

                fsPath.replace_extension(ENGINE_TEXTURE_EXTENSION);
                fsDstPath = dstDirPath / fsPath;

                materialArg.diffuseTexturePath = std::filesystem::relative(fsDstPath).string().c_str();

                importeTextureFile(fsSrcPath.string().c_str(), fsDstPath.string().c_str(), textureArg);
            }
        }

        for (unsigned int iText = 0;
             iText < scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_BASE_COLOR); ++iText)
        {
            scene->mMaterials[i]->GetTexture(aiTextureType::aiTextureType_BASE_COLOR, iText, &path);

            if (const aiTexture* texture = scene->GetEmbeddedTexture(path.C_Str()))
            {
                Texture::ImportArg arg;

                if (texture->mHeight)
                {
                    arg.len = texture->mHeight * texture->mWidth * sizeof(texture->pcData);
                }
                else
                {
                    arg.len = texture->mWidth;
                }

                arg.pixels.reset((unsigned char*)texture->pcData);
                arg.flipTexture = true;

                fsDstPath = dstDirPath / std::filesystem::path(texture->mFilename.C_Str()).stem();
                fsDstPath.replace_extension(ENGINE_TEXTURE_EXTENSION);
                materialArg.baseColorTexturePath = std::filesystem::relative(fsDstPath).string().c_str();

                writeTextureFile(fsDstPath.string().c_str(), arg);
                arg.pixels.release(); // Assimp manage it's mamory
            }
            else
            {
                fsPath    = path.C_Str();
                fsSrcPath = srcDirPath / fsPath;

                fsPath.replace_extension(ENGINE_TEXTURE_EXTENSION);
                fsDstPath = dstDirPath / fsPath;

                materialArg.baseColorTexturePath = std::filesystem::relative(fsDstPath).string().c_str();

                importeTextureFile(fsSrcPath.string().c_str(), fsDstPath.string().c_str(), textureArg);
            }
        }

        std::filesystem::path dstMaterialPath = dstDirPath / scene->mMaterials[i]->GetName().C_Str();
        dstMaterialPath += ENGINE_MATERIAL_EXTENSION;

        writeMaterialFile(dstMaterialPath.string().c_str(), materialArg);
    }

    // Mesh
    for (size_t i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh* pMesh = scene->mMeshes[i];

        Mesh::CreateIndiceBufferArg arg;
        arg.vertices.reserve(pMesh->mNumVertices);
        arg.indices.reserve(pMesh->mNumFaces * 3u);

        if (pMesh->mVertices == nullptr)
        {
            Log::getInstance()->logError(stringFormat("Mesh \"%s\" without vertices", pMesh->mName));
            continue;
        }

        if (!pMesh->HasNormals())
        {
            Log::getInstance()->logError(stringFormat("Mesh \"%s\" without Normal", pMesh->mName));
            continue;
        }

        if (pMesh->mTextureCoords == nullptr)
        {
            Log::getInstance()->logError(stringFormat("Mesh \"%s\" without UV", pMesh->mName));
            continue;
        }

        if (pMesh->mTextureCoords[0] == nullptr)
        {
            Log::getInstance()->logError(stringFormat("Mesh \"%s\" with invalid UV", pMesh->mName));
            continue;
        }

        // Vertices
        for (size_t verticeId = 0; verticeId < pMesh->mNumVertices; ++verticeId)
        {
            const aiVector3D& vertice   = pMesh->mVertices[verticeId];
            const aiVector3D& textCoord = pMesh->mTextureCoords[0][verticeId];
            const aiVector3D& normal    = pMesh->mNormals[verticeId];

            arg.vertices.emplace_back(Mesh::Vertex{Vec3{vertice.x, vertice.y, vertice.z},
                                                   Vec3{normal.x, normal.y, normal.z}, Vec2{textCoord.x, textCoord.y}});
        }

        // Indices
        for (size_t iFace = 0; iFace < pMesh->mNumFaces; ++iFace)
        {
            arg.indices.emplace_back(pMesh->mFaces[iFace].mIndices[0]);
            arg.indices.emplace_back(pMesh->mFaces[iFace].mIndices[1]);
            arg.indices.emplace_back(pMesh->mFaces[iFace].mIndices[2]);
        }

        std::filesystem::path dstMeshPath = dstDirPath / pMesh->mName.C_Str();
        if (i != 0 &&
            pMesh->mName == scene->mMeshes[i - 1]->mName) // Add differente name if the FBX containe mesh with same name
            dstMeshPath += "0";

        dstMeshPath += ENGINE_MESH_EXTENSION;

        writeMeshFile(dstMeshPath.string().c_str(), arg);
    }

    Log::getInstance()->logInitializationEnd("Model importion");
}

struct TextureHeader
{
    char assetID            = (char)EFileType::TEXTURE;
    int  textureLenght      = 0;
    bool vflipTextureOnLoad = false;
};

void GPE::importeTextureFile(const char* srcPath, const char* dstPath, const TextureImportConfig& config)
{
    stbi_set_flip_vertically_on_load(config.verticalFlip);

    Texture::ImportArg arg;
    arg.pixels.reset(stbi_load(srcPath, &arg.w, &arg.h, &arg.comp, 0));

    if (arg.pixels == nullptr)
    {
        FUNCT_ERROR((std::string("STBI cannot load image: ") + srcPath).c_str());
        FUNCT_ERROR(std::string("Reason: ") + stbi_failure_reason());
        return;
    }

    switch (config.format)
    {
    case TextureImportConfig::EFormatType::PNG:
    default: {
        arg.pixels.reset(stbi_write_png_to_mem((const unsigned char*)arg.pixels.get(), arg.w * arg.comp, arg.w, arg.h,
                                               arg.comp, &arg.len));

        if (arg.pixels == NULL)
        {
            FUNCT_ERROR((std::string("STBI cannot write image with png format with this path : ") + srcPath).c_str());
        }

        break;
    }
    }

    writeTextureFile(dstPath, arg);
    stbi_set_flip_vertically_on_load(false);
}

void GPE::writeTextureFile(const char* dst, const Texture::ImportArg& data)
{
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    TextureHeader header{(char)EFileType::TEXTURE, data.len, data.flipTexture};
    fwrite(&header, sizeof(header), 1, pFile); // header

    fwrite(data.pixels.get(), data.len, 1, pFile);
    fclose(pFile);

    Log::getInstance()->log(stringFormat("Texture write to \"%s\"", dst));
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

        stbi_set_flip_vertically_on_load(header.vflipTextureOnLoad);

        std::vector<stbi_uc> texBuffer;
        texBuffer.assign(header.textureLenght, 0);

        fread(&texBuffer[0], sizeof(stbi_uc), header.textureLenght, pFile); // Texture buffer

        arg.pixels.reset(stbi_load_from_memory(texBuffer.data(), header.textureLenght, &arg.w, &arg.h, &arg.comp, 0));

        stbi_set_flip_vertically_on_load(false);
    }
    else
    {
        arg.pixels.reset(stbi_load_from_file(pFile, &arg.w, &arg.h, &arg.comp, 0));
    }

    if (!arg.pixels)
    {
        Log::getInstance()->logError(stringFormat("Fail to load file \"%s\"", src));
    }

    fclose(pFile);

    Log::getInstance()->log(stringFormat("Texture read from \"%s\"", src));
    return arg;
}

Texture* GPE::loadTextureFile(const char* src)
{
    std::filesystem::path srcPath(src);
    if (Texture* const pTexture = Engine::getInstance()->resourceManager.get<Texture>(srcPath.filename().string()))
        return pTexture;
    return &Engine::getInstance()->resourceManager.add<Texture>(srcPath.filename().string(), readTextureFile(src));
}

struct MaterialHeader
{
    char              assetID                    = (char)EFileType::MATERIAL;
    MaterialComponent component                  = {};
    int               pathAmbianteTextureLenght  = 0;
    int               pathDiffuseTextureLenght   = 0;
    int               pathBaseColorTextureLenght = 0;
};

void GPE::writeMaterialFile(const char* dst, const Material::ImporteArg& arg)
{
    FILE* pFile = nullptr;
    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    MaterialHeader header{(char)EFileType::MATERIAL, arg.comp, static_cast<int>(arg.ambianteTexturePath.size()),
                          static_cast<int>(arg.diffuseTexturePath.size()),
                          static_cast<int>(arg.baseColorTexturePath.size())};
    fwrite(&header, sizeof(header), 1, pFile); // header

    fwrite(arg.ambianteTexturePath.data(), sizeof(char), header.pathAmbianteTextureLenght, pFile); // string buffer
    fwrite(arg.diffuseTexturePath.data(), sizeof(char), header.pathDiffuseTextureLenght, pFile);   // string buffer
    fwrite(arg.baseColorTexturePath.data(), sizeof(char), header.pathBaseColorTextureLenght,
           pFile); // string buffer

    fclose(pFile);

    Log::getInstance()->log(stringFormat("Material write to \"%s\"", dst));
}

Material::ImporteArg GPE::readMaterialFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);
    Material::ImporteArg  arg = {};

    if (srcPath.extension() != ENGINE_MATERIAL_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    MaterialHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);
    arg.comp = header.component;

    if (header.pathAmbianteTextureLenght) // If ambiante texture existe
    {
        arg.ambianteTexturePath.assign(header.pathAmbianteTextureLenght, '\0');
        fread(arg.ambianteTexturePath.data(), sizeof(char), header.pathAmbianteTextureLenght,
              pFile); // string buffer
    }

    if (header.pathDiffuseTextureLenght) // If diffuse texture existe
    {
        arg.diffuseTexturePath.assign(header.pathDiffuseTextureLenght, '\0');
        fread(arg.diffuseTexturePath.data(), sizeof(char), header.pathDiffuseTextureLenght, pFile); // string buffer
    }

    if (header.pathBaseColorTextureLenght) // If base color texture existe
    {
        arg.baseColorTexturePath.assign(header.pathBaseColorTextureLenght, '\0');
        fread(arg.baseColorTexturePath.data(), sizeof(char), header.pathBaseColorTextureLenght,
              pFile); // string buffer
    }

    fclose(pFile);
    Log::getInstance()->log(stringFormat("Material read from \"%s\"", src));
    return arg;
}

Material* GPE::loadMaterialFile(const char* src)
{
    std::filesystem::path srcPath(src);
    Material::ImporteArg  importeArg = readMaterialFile(src);
    Material::CreateArg   arg;
    arg.comp = importeArg.comp;

    if (!importeArg.ambianteTexturePath.empty())
        arg.pAmbianteTexture =
            loadTextureFile((std::filesystem::current_path() / importeArg.ambianteTexturePath).string().c_str());

    if (!importeArg.diffuseTexturePath.empty())
        arg.pDiffuseTexture =
            loadTextureFile((std::filesystem::current_path() / importeArg.diffuseTexturePath).string().c_str());

    if (!importeArg.baseColorTexturePath.empty())
        arg.pBaseColorTexture =
            loadTextureFile((std::filesystem::current_path() / importeArg.baseColorTexturePath).string().c_str());

    if (Material* const pMat = Engine::getInstance()->resourceManager.get<Material>(srcPath.filename().string()))
        return pMat;
    return &Engine::getInstance()->resourceManager.add<Material>(srcPath.filename().string(), arg);
}

struct MeshHeader
{
    char assetID       = (char)EFileType::MESH;
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

    MeshHeader header{(char)EFileType::MESH, static_cast<int>(arg.vertices.size()),
                      static_cast<int>(arg.indices.size())};
    fwrite(&header, sizeof(header), 1, pFile);                                         // header
    fwrite(arg.vertices.data(), sizeof(arg.vertices[0]), header.verticeLenght, pFile); // vertice buffer
    fwrite(arg.indices.data(), sizeof(arg.indices[0]), header.indiceLenght, pFile);    // indice buffer

    fclose(pFile);
    Log::getInstance()->log(stringFormat("Mesh write to \"%s\"", dst));
}

Mesh::CreateIndiceBufferArg GPE::readMeshFile(const char* src)
{
    FILE*                       pFile = nullptr;
    std::filesystem::path       srcPath(src);
    Mesh::CreateIndiceBufferArg arg;

    if (srcPath.extension() != ENGINE_MESH_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    MeshHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);

    if (header.verticeLenght)
    {
        arg.vertices.assign(header.verticeLenght, Mesh::Vertex{});
        fread(&arg.vertices[0], sizeof(arg.vertices[0]), header.verticeLenght, pFile); // vertice buffer
    }

    if (header.indiceLenght)
    {
        arg.indices.assign(header.indiceLenght, 0);
        fread(&arg.indices[0], sizeof(arg.indices[0]), header.indiceLenght, pFile); // indice buffer
    }

    fclose(pFile);
    Log::getInstance()->log(stringFormat("Mesh read from \"%s\"", src));

    return arg;
}

Mesh* GPE::loadMeshFile(const char* src)
{
    std::filesystem::path srcPath(src);

    if (Mesh* const pMesh = Engine::getInstance()->resourceManager.get<Mesh>(srcPath.filename().string()))
        return pMesh;
    return &Engine::getInstance()->resourceManager.add<Mesh>(srcPath.filename().string(), readMeshFile(src));
}

struct ShaderHeader
{
    char     assetID            = (char)EFileType::SHADER;
    int      vertexPathLenght   = 0;
    int      fragmentPathLenght = 0;
    uint8_t  featureMask        = 0;
};

void GPE::writeShaderFile(const char* dst, const ShaderCreateConfig& arg)
{
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    ShaderHeader header{(char)EFileType::SHADER, static_cast<int>(arg.vertexShaderPath.size()),
                        static_cast<int>(arg.fragmentShaderPath.size()), arg.featureMask};
    fwrite(&header, sizeof(header), 1, pFile); // header

    fwrite(arg.vertexShaderPath.data(), sizeof(char), arg.vertexShaderPath.size(), pFile);     // string buffer
    fwrite(arg.fragmentShaderPath.data(), sizeof(char), arg.fragmentShaderPath.size(), pFile); // string buffer

    fclose(pFile);

    Log::getInstance()->log(stringFormat("Shader write to \"%s\"", dst));
}

ShaderCreateConfig GPE::readShaderFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);
    ShaderCreateConfig    arg;

    if (srcPath.extension() != ENGINE_SHADER_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    ShaderHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);
    arg.featureMask = header.featureMask;

    if (header.vertexPathLenght)
    {
        arg.vertexShaderPath.assign(header.vertexPathLenght, '\0');
        fread(arg.vertexShaderPath.data(), sizeof(char), header.vertexPathLenght, pFile); // string buffer
    }

    if (header.fragmentPathLenght)
    {
        arg.fragmentShaderPath.assign(header.fragmentPathLenght, '\0');
        fread(arg.fragmentShaderPath.data(), sizeof(char), header.fragmentPathLenght, pFile); // string buffer
    }

    fclose(pFile);

    Log::getInstance()->log(stringFormat("Shader read from \"%s\"", src));
    return arg;
}

Shader* GPE::loadShaderFile(const char* src)
{
    std::filesystem::path srcPath(src);
    ShaderCreateConfig    arg = readShaderFile(src);

    if (Shader* const pShader = Engine::getInstance()->resourceManager.get<Shader>(srcPath.filename().string()))
        return pShader;

    return &Engine::getInstance()->resourceManager.add<Shader>(
        srcPath.filename().string(), (std::filesystem::current_path() / arg.vertexShaderPath).string().c_str(),
        (std::filesystem::current_path() / arg.fragmentShaderPath).string().c_str(), arg.featureMask);
}

struct PrefabHeader
{
    char     assetID  = (char)EFileType::PREFAB;
    uint16_t type     = 0;
    size_t   dataSize = 0;
};

void GPE::writePrefabFile(const char* dst, const SavedScene::CreateArg& arg)
{
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    PrefabHeader header{(char)EFileType::PREFAB, (uint16_t)arg.type, arg.data.size()};
    fwrite(&header, sizeof(header), 1, pFile); // header

    fwrite(arg.data.data(), sizeof(char), arg.data.size(), pFile); // string buffer
    fclose(pFile);

    Log::getInstance()->log(stringFormat("Prefab write to \"%s\"", dst));
}

SavedScene::CreateArg GPE::readPrefabFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);
    SavedScene::CreateArg arg;

    if (srcPath.extension() != ENGINE_PREFAB_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    PrefabHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);

    arg.type = (SavedScene::EType)header.type;
    if (header.dataSize)
    {
        arg.data.assign(header.dataSize, '\0');
        fread(arg.data.data(), sizeof(char), header.dataSize, pFile); // string buffer
    }

    fclose(pFile);

    Log::getInstance()->log(stringFormat("Prefab read from \"%s\"", src));
    return arg;
}

SavedScene::CreateArg GPE::loadPrefabFile(const char* src)
{
    std::filesystem::path srcPath(src);
    return readPrefabFile(src);
}

struct SceneHeader
{
    char     assetID  = (char)EFileType::SCENE;
    uint16_t type     = 0;
    size_t   dataSize = 0;
};

void GPE::writeSceneFile(const char* dst, const SavedScene::CreateArg& arg)
{
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    SceneHeader header{(char)EFileType::SCENE, (uint16_t)arg.type, arg.data.size()};
    fwrite(&header, sizeof(header), 1, pFile); // header

    fwrite(arg.data.data(), sizeof(char), arg.data.size(), pFile); // string buffer
    fclose(pFile);

    Log::getInstance()->log(stringFormat("Scene write to \"%s\"", dst));
}

SavedScene::CreateArg GPE::readSceneFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);
    SavedScene::CreateArg arg;

    if ((srcPath.extension() == ENGINE_SCENE_EXTENSION || srcPath.extension() == ENGINE_PREFAB_EXTENSION) &&
        fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    SceneHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);

    arg.type = (SavedScene::EType)header.type;
    if (header.dataSize)
    {
        arg.data.assign(header.dataSize, '\0');
        fread(arg.data.data(), sizeof(char), header.dataSize, pFile); // string buffer
    }

    fclose(pFile);

    Log::getInstance()->log(stringFormat("Scene read from \"%s\"", src));
    return arg;
}

SavedScene::CreateArg GPE::loadSceneFile(const char* src)
{
    return readSceneFile(src);
}