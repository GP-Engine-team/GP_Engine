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

#include <ENgine/Resources/Skeleton.hpp>
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

    unsigned int postProcessflags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType |
                                    aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_FlipUVs |
                                    aiProcess_CalcTangentSpace;

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
             iText < scene->mMaterials[i]->GetTextureCount(aiTextureType::aiTextureType_NORMALS); ++iText)
        {
            scene->mMaterials[i]->GetTexture(aiTextureType::aiTextureType_NORMALS, iText, &path);

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
                materialArg.normalMapTexturePath = std::filesystem::relative(fsDstPath).string().c_str();

                writeTextureFile(fsDstPath.string().c_str(), arg);
                arg.pixels.release(); // Assimp manage it's mamory
            }
            else
            {
                fsPath    = path.C_Str();
                fsSrcPath = srcDirPath / fsPath;

                fsPath.replace_extension(ENGINE_TEXTURE_EXTENSION);
                fsDstPath = dstDirPath / fsPath;

                materialArg.normalMapTexturePath = std::filesystem::relative(fsDstPath).string().c_str();

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
            const aiVector3D& tangeante = pMesh->mTangents[verticeId];

            arg.vertices.emplace_back(Mesh::Vertex{Vec3{vertice.x, vertice.y, vertice.z},
                                                   Vec3{normal.x, normal.y, normal.z}, Vec2{textCoord.x, textCoord.y},
                                                   Vec3{tangeante.x, tangeante.y, tangeante.z}});
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

    //// Skeleton
    // for (size_t i = 0; i < scene->mNumMeshes; ++i)
    //{
    //    aiMesh* pMesh = scene->mMeshes[i];

    //    Skeleton::CreateArgs arg;

    //    // Save names
    //    arg.boneNames.reserve(pMesh->mNumBones);
    //    for (size_t boneIndex = 0; boneIndex < pMesh->mNumBones; boneIndex++)
    //    {
    //        aiBone* bone = pMesh->mBones[boneIndex];
    //        arg.boneNames.push_back(bone->mName.C_Str());
    //    }

    //    // Save offset matrices
    //    arg.offsetMatrices.reserve(pMesh->mNumBones);
    //    for (size_t boneIndex = 0; boneIndex < pMesh->mNumBones; boneIndex++)
    //    {
    //        aiBone* bone = pMesh->mBones[boneIndex];
    //        auto    aiMatrix4ToMat4 = [](const aiMatrix4x4& mat) -> GPM::Mat4
    //        {
    //            GPM::mat4 m;
    //            for (int matY = 0; matY < 4; matY++)
    //            {
    //                for (int matX = 0; matX < 4; matX++)
    //                {
    //                    m.e[matY*4+matX] = mat[matY][matX];
    //                }
    //            }
    //            return m;
    //        };
    //        arg.offsetMatrices.push_back(aiMatrix4ToMat4(bone->mOffsetMatrix));
    //    }

    //    for (size_t boneIndex = 0; boneIndex < pMesh->mNumBones; boneIndex++)
    //    {
    //        aiBone* bone = pMesh->mBones[boneIndex];
    //        std::vector<Skeleton::VertexWeight> weights;
    //        weights.reserve(bone->mNumWeights);
    //        for (size_t weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++)
    //        {
    //            Skeleton::VertexWeight weightData;
    //            weightData.vertexID = bone->mWeights[weightIndex].mVertexId;
    //            weightData.weight = bone->mWeights[weightIndex].mWeight;
    //            weights.emplace_back(std::move(weightData));
    //        }
    //        arg.weights.emplace_back(std::move(weights));
    //    }

    //    std::filesystem::path dstSkeletonPath = dstDirPath / pMesh->mName.C_Str();
    //    if (i != 0 &&
    //        pMesh->mName == scene->mMeshes[i - 1]->mName) // Add differente name if the FBX containe mesh with same
    //        name dstSkeletonPath += "0";

    //    dstSkeletonPath += ENGINE_SKELETON_EXTENSION;

    //    writeSkeletonFile(dstSkeletonPath.string().c_str(), arg);
    //}

    // Animations
    for (size_t i = 0; i < scene->mNumAnimations; ++i)
    {
        aiAnimation* pAnim = scene->mAnimations[i];

        Animation::CreateArgs arg;

        // Save name
        arg.animName = pAnim->mName.data;

        // Save duration
        arg.duration = pAnim->mDuration;

        // Save names and delta
        arg.boneNames.reserve(pAnim->mNumChannels);
        arg.boneDeltas.reserve(pAnim->mNumChannels);
        for (size_t boneIndex = 0; boneIndex < pAnim->mNumChannels; boneIndex++)
        {
            aiNodeAnim* bone = pAnim->mChannels[boneIndex];

            // Save Name
            arg.boneNames.push_back(bone->mNodeName.data);

            DeltaBone deltaBone;
            // Save Pos
            for (size_t deltaPosIndex = 0; deltaPosIndex < bone->mNumPositionKeys; deltaPosIndex++)
            {
                const aiVector3D& p                        = bone->mPositionKeys[deltaPosIndex].mValue;
                deltaBone.deltaPositions[deltaPosIndex]    = {p.x, p.y, p.z};
                deltaBone.positionsTimeKeys[deltaPosIndex] = bone->mPositionKeys[deltaPosIndex].mTime;
            }
            // Save Rot
            for (size_t deltaRotIndex = 0; deltaRotIndex < bone->mNumRotationKeys; deltaRotIndex++)
            {
                const aiQuaternion& r                      = bone->mRotationKeys[deltaRotIndex].mValue;
                Quat&               q                      = deltaBone.deltaRotations[deltaRotIndex];
                q.x                                        = r.x;
                q.y                                        = r.y;
                q.z                                        = r.z;
                q.w                                        = r.w;
                deltaBone.rotationsTimeKeys[deltaRotIndex] = bone->mRotationKeys[deltaRotIndex].mTime;
            }
            // Save Scale
            for (size_t deltaScaleIndex = 0; deltaScaleIndex < bone->mNumScalingKeys; deltaScaleIndex++)
            {
                const aiVector3D& s                          = bone->mScalingKeys[deltaScaleIndex].mValue;
                deltaBone.deltaScales[deltaScaleIndex]    = {s.x, s.y, s.z};
                deltaBone.scalesTimeKeys[deltaScaleIndex] = bone->mScalingKeys[deltaScaleIndex].mTime;
            }
            arg.boneDeltas.push_back(deltaBone);
        }

        std::filesystem::path dstAnimationPath = dstDirPath / pAnim->mName.C_Str();
        if (i != 0 &&
            pAnim->mName == scene->mMeshes[i - 1]->mName) // Add differente name if the FBX containe mesh with same name
            dstAnimationPath += "0";

        dstAnimationPath += ENGINE_ANIMATION_EXTENSION;

        writeAnimationFile(dstAnimationPath.string().c_str(), arg);
    }

    Log::getInstance()->logInitializationEnd("Model importation");
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

    Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
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

    Log::getInstance()->log(stringFormat("File read from \"%s\"", src));
    return arg;
}

Texture* GPE::loadTextureFile(const char* src)
{
    if (Texture* const pTexture = Engine::getInstance()->resourceManager.get<Texture>(src))
        return pTexture;

    return &Engine::getInstance()->resourceManager.add<Texture>(src, readTextureFile(src));
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
                          static_cast<int>(arg.normalMapTexturePath.size())};
    fwrite(&header, sizeof(header), 1, pFile); // header

    fwrite(arg.ambianteTexturePath.data(), sizeof(char), header.pathAmbianteTextureLenght, pFile); // string buffer
    fwrite(arg.diffuseTexturePath.data(), sizeof(char), header.pathDiffuseTextureLenght, pFile);   // string buffer
    fwrite(arg.normalMapTexturePath.data(), sizeof(char), header.pathBaseColorTextureLenght,
           pFile); // string buffer

    fclose(pFile);

    Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
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
        arg.normalMapTexturePath.assign(header.pathBaseColorTextureLenght, '\0');
        fread(arg.normalMapTexturePath.data(), sizeof(char), header.pathBaseColorTextureLenght,
              pFile); // string buffer
    }

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File read from \"%s\"", src));
    return arg;
}

Material* GPE::loadMaterialFile(const char* src)
{
    Material::ImporteArg importeArg = readMaterialFile(src);
    Material::CreateArg  arg;
    arg.comp = importeArg.comp;

    if (!importeArg.ambianteTexturePath.empty())
        arg.pAmbianteTexture =
            loadTextureFile((std::filesystem::current_path() / importeArg.ambianteTexturePath).string().c_str());

    if (!importeArg.diffuseTexturePath.empty())
        arg.pDiffuseTexture =
            loadTextureFile((std::filesystem::current_path() / importeArg.diffuseTexturePath).string().c_str());

    if (!importeArg.normalMapTexturePath.empty())
        arg.pNormalMapTexture =
            loadTextureFile((std::filesystem::current_path() / importeArg.normalMapTexturePath).string().c_str());

    if (Material* const pMat = Engine::getInstance()->resourceManager.get<Material>(src))
        return pMat;
    return &Engine::getInstance()->resourceManager.add<Material>(src, arg);
}

struct MeshHeader
{
    char assetID       = (char)EFileType::MESH;
    int  verticeLength = 0;
    int  indiceLength  = 0;
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
    fwrite(arg.vertices.data(), sizeof(arg.vertices[0]), header.verticeLength, pFile); // vertice buffer
    fwrite(arg.indices.data(), sizeof(arg.indices[0]), header.indiceLength, pFile);    // indice buffer

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
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

    if (header.verticeLength)
    {
        arg.vertices.assign(header.verticeLength, Mesh::Vertex{});
        fread(&arg.vertices[0], sizeof(arg.vertices[0]), header.verticeLength, pFile); // vertice buffer
    }

    if (header.indiceLength)
    {
        arg.indices.assign(header.indiceLength, 0);
        fread(&arg.indices[0], sizeof(arg.indices[0]), header.indiceLength, pFile); // indice buffer
    }

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File read from \"%s\"", src));

    return arg;
}

Mesh* GPE::loadMeshFile(const char* src)
{
    if (Mesh* const pMesh = Engine::getInstance()->resourceManager.get<Mesh>(src))
        return pMesh;
    return &Engine::getInstance()->resourceManager.add<Mesh>(src, readMeshFile(src));
}

struct ShaderHeader
{
    char    assetID            = (char)EFileType::SHADER;
    int     vertexPathLenght   = 0;
    int     fragmentPathLenght = 0;
    uint8_t featureMask        = 0;
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

    Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
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

    Log::getInstance()->log(stringFormat("File read from \"%s\"", src));
    return arg;
}

Shader* GPE::loadShaderFile(const char* src)
{
    if (Shader* const pShader = Engine::getInstance()->resourceManager.get<Shader>(src))
        return pShader;

    ShaderCreateConfig arg = readShaderFile(src);

    return &Engine::getInstance()->resourceManager.add<Shader>(
        src, (std::filesystem::current_path() / arg.vertexShaderPath).string().c_str(),
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

    Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
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

    Log::getInstance()->log(stringFormat("File read from \"%s\"", src));
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

    Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
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

    Log::getInstance()->log(stringFormat("File read from \"%s\"", src));
    return arg;
}

SavedScene::CreateArg GPE::loadSceneFile(const char* src)
{
    return readSceneFile(src);
}

struct SkeletonHeader
{
    char assetID       = (char)EFileType::SKELETON;
    int  verticeLength = 0;
    int  indiceLength  = 0;
};

void writeSkeletonFile(const char* dst, const Skeleton::CreateArgs& arg)
{
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    //SkeletonHeader header{(char)EFileType::MESH, static_cast<int>(arg.vertices.size()),
    //                      static_cast<int>(arg.indices.size())};

    //fwrite(&header, sizeof(header), 1, pFile);                                         // header
    //fwrite(arg.vertices.data(), sizeof(arg.vertices[0]), header.verticeLength, pFile); // vertice buffer
    //fwrite(arg.indices.data(), sizeof(arg.indices[0]), header.indiceLength, pFile);    // indice buffer

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
}

Mesh::CreateIndiceBufferArg GPE::readSkeletonFile(const char* src)
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

    //if (header.verticeLength)
    //{
    //    arg.vertices.assign(header.verticeLength, Mesh::Vertex{});
    //    fread(&arg.vertices[0], sizeof(arg.vertices[0]), header.verticeLength, pFile); // vertice buffer
    //}

    //if (header.indiceLength)
    //{
    //    arg.indices.assign(header.indiceLength, 0);
    //    fread(&arg.indices[0], sizeof(arg.indices[0]), header.indiceLength, pFile); // indice buffer
    //}

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File read from \"%s\"", src));

    return arg;
}

Skeleton* GPE::loadSkeletonFile(const char* src)
{
    std::filesystem::path srcPath(src);

    //if (Skeleton* const pSkeleton = Engine::getInstance()->resourceManager.get<Skeleton>(srcPath.filename().string()))
    //    return pSkeleton;
    //return &Engine::getInstance()->resourceManager.add<Skeleton>(srcPath.filename().string(), readMeshFile(src));
    return nullptr;
}

struct AnimationHeader
{
    char assetID       = (char)EFileType::ANIMATION;
    int  verticeLength = 0;
    int  indiceLength  = 0;
};

void GPE::writeAnimationFile(const char* dst, const Animation::CreateArgs& arg)
{
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    //MeshHeader header{(char)EFileType::MESH, static_cast<int>(arg.vertices.size()),
    //                  static_cast<int>(arg.indices.size())};
    //fwrite(&header, sizeof(header), 1, pFile);                                         // header
    //fwrite(arg.vertices.data(), sizeof(arg.vertices[0]), header.verticeLength, pFile); // vertice buffer
    //fwrite(arg.indices.data(), sizeof(arg.indices[0]), header.indiceLength, pFile);    // indice buffer

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
}

Mesh::CreateIndiceBufferArg GPE::readAnimationFile(const char* src)
{
    FILE*                       pFile = nullptr;
    std::filesystem::path       srcPath(src);
    Mesh::CreateIndiceBufferArg arg;

    if (srcPath.extension() != ENGINE_MESH_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    //MeshHeader header;
    //// copy the file into the buffer:
    //fread(&header, sizeof(header), 1, pFile);

    //if (header.verticeLength)
    //{
    //    arg.vertices.assign(header.verticeLength, Mesh::Vertex{});
    //    fread(&arg.vertices[0], sizeof(arg.vertices[0]), header.verticeLength, pFile); // vertice buffer
    //}

    //if (header.indiceLength)
    //{
    //    arg.indices.assign(header.indiceLength, 0);
    //    fread(&arg.indices[0], sizeof(arg.indices[0]), header.indiceLength, pFile); // indice buffer
    //}

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File read from \"%s\"", src));

    return arg;
}

Animation* GPE::loadAnimationFile(const char* src)
{
    std::filesystem::path srcPath(src);

    //if (Animation* const pAnim = Engine::getInstance()->resourceManager.get<Animation>(srcPath.filename().string()))
    //    return pAnim;
    //return &Engine::getInstance()->resourceManager.add<Animation>(srcPath.filename().string(), readMeshFile(src));
    return nullptr;
}
