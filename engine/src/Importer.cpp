﻿#include <Engine/Resources/Importer/Importer.hpp>

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
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/ResourcesManagerType.hpp>
#include <Engine/Resources/Texture.hpp>
#include <Engine/Serialization/binary/BinaryLoader.hpp>
#include <Engine/Serialization/binary/BinarySaver.hpp>
#include <GPM/Shape3D/AABB.hpp>
#include <GPM/Shape3D/Sphere.hpp>

#include <Engine/Core/Tools/Hash.hpp>

using namespace GPE;
using namespace GPM;

std::string GPE::saveSceneToStringImp(GPE::Scene* scene, GPE::SavedScene::EType saveMode)
{
    if (saveMode == GPE::SavedScene::EType::XML)
    {
        rapidxml::xml_document<> doc;
        XmlSaver                 context(doc);
        context.addWeakPtr(scene);
        scene->save(context);

        return docToString(doc);
    }

    FUNCT_WARNING("Unsupported save type");
    return "";
}

void GPE::saveSceneToPathImp(GPE::Scene* scene, const char* path, GPE::SavedScene::EType saveMode)
{
    GPE::SavedScene::CreateArg args;
    args.data = GPE::saveSceneToStringImp(scene, saveMode);
    args.type = saveMode;

    GPE::writeSceneFile(path, args);
}

void GPE::loadSceneFromStringImp(GPE::Scene* scene, const std::string& str, GPE::SavedScene::EType type)
{
    if (type == GPE::SavedScene::EType::XML)
    {
        // Load xml doc
        rapidxml::xml_document<> doc;
        std::unique_ptr<char[]>  buffer;
        GPE::SavedScene::toDoc(doc, buffer, str.c_str(), str.size());

        XmlLoader context(doc);
        // Load each element
        scene->load(context);

        // Tell that pointers to the old scene should be replaced by pointers to the new scene
        context.addConvertedPtr(scene->getWorld().pOwnerScene, scene);

        // Update old pointers into new ones
        context.updateLazyPtrs();
        context.updateLinker(scene->getWorld());

        // Call onPostLoad on GameObjects
        struct Rec
        {
        private:
            static void recTransform(GPE::GameObject* g)
            {
                g->getTransform().onPostLoad();

                for (GPE::GameObject* g2 : g->children)
                {
                    recTransform(g2);
                }
            };
            static void recComponent(GPE::GameObject* g)
            {
                for (GPE::Component* comp : g->getComponents())
                {
                    comp->onPostLoad();
                }

                for (GPE::GameObject* g2 : g->children)
                {
                    recComponent(g2);
                }
            };

        public:
            static void rec(GPE::GameObject* g)
            {
                recTransform(g);
                g->updateSelfAndChildren();
                recComponent(g);
            }
        };
        Rec::rec(&scene->getWorld()); // can't do recursives with lambdas, and std::function would be overkill
    }
}

void GPE::loadSceneFromPathImp(GPE::Scene* scene, const char* path)
{
    GPE::SavedScene::CreateArg savedScene = GPE::readSceneFile(path);
    loadSceneFromStringImp(scene, savedScene.data, savedScene.type);
}

std::string GPE::savePrefabToStringImp(GPE::GameObject& prefab, GPE::SavedScene::EType saveMode)
{
    Scene             tempScene;
    GameObject* const pPreviousParent     = prefab.getParent();
    Scene* const      pPreviousOwnedScene = prefab.pOwnerScene;
    std::string       data;

    tempScene.getWorld().children.emplace_back(&prefab);
    prefab.forceSetParent(tempScene.getWorld());
    prefab.pOwnerScene = nullptr;

    data = saveSceneToStringImp(&tempScene, saveMode);

    prefab.pOwnerScene = pPreviousOwnedScene;
    prefab.forceSetParent(*pPreviousParent);
    tempScene.getWorld().children.clear();

    return data;
}

void GPE::savePrefabToPathImp(GPE::GameObject& prefab, const char* path, GPE::SavedScene::EType saveMode)
{
    GPE::SavedScene::CreateArg args;
    args.data = savePrefabToStringImp(prefab, saveMode);
    args.type = saveMode;

    GPE::writeSceneFile(path, args);
}

void removeIllegalCharsForPath(std::string& targetString)
{
    const std::string illegalChars = "\\/:?\"<>|";

    for (std::string::iterator it = targetString.begin(); it < targetString.end();)
    {
        if (illegalChars.find(*it) != std::string::npos)
        {
            targetString.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// Try to open file in binary mode and check try to fixe path if it is invalid
FILE* openFileToWrite(const char* path)
{
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, path, "w+b"))
    {
        // file name can contain illegal character. Change it and retry
        std::filesystem::path fsPath   = path;
        std::string           fileName = fsPath.stem().string();
        removeIllegalCharsForPath(fileName);
        fsPath.replace_filename(fileName + fsPath.extension().string());

        if (fopen_s(&pFile, fsPath.string().c_str(), "w+b"))
        {
            Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", path));
            return nullptr;
        }
        else
        {
            Log::getInstance()->logError(
                stringFormat("The file \"%s\" contained illegal character and was rename", fileName));
        }
    }
    return pFile;
}

GPE::GameObject* GPE::loadPrefabFromStringImp(GPE::GameObject& parent, const std::string& str,
                                              GPE::SavedScene::EType type)
{
    Scene scene;

    // Load the file
    if (type == GPE::SavedScene::EType::XML)
    {
        // Load xml doc
        rapidxml::xml_document<> doc;
        std::unique_ptr<char[]>  buffer;
        GPE::SavedScene::toDoc(doc, buffer, str.c_str(), str.size());

        XmlLoader context(doc);
        // Load each element
        scene.load(context);

        // Tell that pointers to the old scene should be replaced by pointers to the new scene
        context.addConvertedPtr(scene.getWorld().pOwnerScene, &scene);

        // Update old pointers into new ones
        context.updateLazyPtrs();

        if (scene.getWorld().children.size())
        {
            GameObject* const go = scene.getWorld().children.front();
            if (go)
                context.updateLinker(*go);
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }

    // Init the prefab
    GameObject* const go = scene.getWorld().children.front();
    if (go)
    {
        go->forceUpdate(parent.getTransform().get().model);
        go->setParent(&parent);
        go->getTransform().setDirty();

        // Call onPostLoad on GameObjects
        struct Rec
        {
        private:
            static void recComponent(GPE::GameObject* g)
            {
                for (GPE::Component* comp : g->getComponents())
                {
                    comp->onPostLoad();
                }

                for (GPE::GameObject* g2 : g->children)
                {
                    recComponent(g2);
                }
            };

        public:
            static void rec(GPE::GameObject* g)
            {
                recComponent(g);
            }
        };
        Rec::rec(go); // can't do recursives with lambdas, and std::function would be overkill
    }

    scene.getWorld().children.clear();
    return go;
}

GPE::GameObject* GPE::loadPrefabFromPathImp(GPE::GameObject& parent, const char* path)
{
    GPE::SavedScene::CreateArg savedScene = GPE::readSceneFile(path);
    return loadPrefabFromStringImp(parent, savedScene.data, savedScene.type);
}

void GPE::importeModel(const char* srcPath, const char* dstPath, Mesh::EBoundingVolume boundingVolumeType) noexcept
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
    std::filesystem::path fileName = srcDirPath.stem();
    srcDirPath                     = srcDirPath.parent_path();

    std::filesystem::path dstDirPath(dstPath);
    dstDirPath = std::filesystem::relative(dstDirPath);

    Scene       prefab;
    GameObject& prefGO    = prefab.getWorld().addChild();
    Model&      prefModel = prefGO.addComponent<Model>();

    std::vector<Material*> matList;

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

                fsDstPath = dstDirPath / std::filesystem::path(texture->mFilename.C_Str()).stem();
                fsDstPath.replace_extension(ENGINE_TEXTURE_EXTENSION);
                materialArg.diffuseTexturePath = std::filesystem::relative(fsDstPath).string().c_str();

                stbi_uc* pix = stbi_load_from_memory(arg.pixels.get(), arg.len, &arg.w, &arg.h, &arg.comp, 0);
                arg.pixels.release(); // Assimp manage it's mamory
                arg.pixels.reset(pix);
                writeTextureFile(fsDstPath.string().c_str(), arg);
            }
            else
            {
                fsPath = path.C_Str();
                // If the path doesn't exist, try in relative path
                if (!std::filesystem::exists(fsPath))
                {
                    std::filesystem::path relativePath = srcPath;
                    relativePath                       = relativePath.parent_path();

                    // If the path doesn't exist, try in local with name only
                    if (!std::filesystem::exists(relativePath / fsPath))
                    {
                        // If asset is not found, discard
                        if (!std::filesystem::exists(relativePath / fsPath.filename()))
                        {
                            continue;
                        }
                        else
                        {
                            fsPath = relativePath / fsPath.filename();
                        }
                    }
                    else
                    {
                        fsPath = relativePath / fsPath;
                    }
                }
                std::string dstPath = (dstDirPath / fsPath.stem()).string() + ENGINE_TEXTURE_EXTENSION;

                materialArg.diffuseTexturePath = dstPath.c_str();
                importeTextureFile(fsPath.string().c_str(), dstPath.c_str(), textureArg);
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

                fsDstPath = dstDirPath / std::filesystem::path(texture->mFilename.C_Str()).stem();
                fsDstPath.replace_extension(ENGINE_TEXTURE_EXTENSION);
                materialArg.normalMapTexturePath = std::filesystem::relative(fsDstPath).string().c_str();

                stbi_uc* pix = stbi_load_from_memory(arg.pixels.get(), arg.len, &arg.w, &arg.h, &arg.comp, 0);
                arg.pixels.release(); // Assimp manage it's mamory
                arg.pixels.reset(pix);
                writeTextureFile(fsDstPath.string().c_str(), arg);
            }
            else
            {
                fsPath = path.C_Str();
                // If the path doesn't exist, try in relative path
                if (!std::filesystem::exists(fsPath))
                {
                    std::filesystem::path relativePath = srcPath;
                    relativePath                       = relativePath.parent_path();

                    // If the path doesn't exist, try in local with name only
                    if (!std::filesystem::exists(relativePath / fsPath))
                    {
                        // If asset is not found, discard
                        if (!std::filesystem::exists(relativePath / fsPath.filename()))
                        {
                            continue;
                        }
                        else
                        {
                            fsPath = relativePath / fsPath.filename();
                        }
                    }
                    else
                    {
                        fsPath = relativePath / fsPath;
                    }
                }

                std::string dstPath = (dstDirPath / fsPath.stem()).string() + ENGINE_TEXTURE_EXTENSION;

                materialArg.normalMapTexturePath = dstPath.c_str();
                importeTextureFile(fsPath.string().c_str(), dstPath.c_str(), textureArg);
            }
        }

        std::filesystem::path dstMaterialPath = dstDirPath / scene->mMaterials[i]->GetName().C_Str();
        dstMaterialPath += ENGINE_MATERIAL_EXTENSION;

        writeMaterialFile(std::filesystem::relative(dstMaterialPath).string().c_str(), materialArg);

        // TODO: Not efficient to pass from HD -> RAM -> HD -> RAM
        matList.emplace_back(loadMaterialFile(dstMaterialPath.string().c_str()));

        // Set shader
        const char* shaderPath   = materialArg.normalMapTexturePath.path.size() ? "DefaultWithNormalMap" : "Default";
        Shader&     shaderToUser = *Engine::getInstance()->resourceManager.get<Shader>(shaderPath);
        matList.back()->setShader(shaderToUser);

        writeMaterialFile(std::filesystem::relative(dstMaterialPath).string().c_str(), *matList.back());
    }

    // Mesh
    for (size_t i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh* pMesh = scene->mMeshes[i];

        Mesh::VertexData arg;

        if (pMesh->HasPositions())
            arg.descriptor.emplace_back(ATTRIB_POSITION_NAME, ATTRIB_POSITION_TYPE);

        if (pMesh->HasNormals())
            arg.descriptor.emplace_back(ATTRIB_NORMAL_NAME, ATTRIB_NORMAL_TYPE);

        for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
        {
            if (pMesh->HasTextureCoords(i))
                arg.descriptor.emplace_back(ATTRIB_UV_NAME + std::to_string(i), ATTRIB_UV_TYPE);
        }

        for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
        {
            if (pMesh->HasVertexColors(i))
                arg.descriptor.emplace_back(ATTRIB_COLOR_NAME + std::to_string(i), ATTRIB_COLOR_TYPE);
        }

        if (pMesh->HasTangentsAndBitangents())
        {
            arg.descriptor.emplace_back(ATTRIB_TANGEANTE_NAME, ATTRIB_TANGEANTE_TYPE);
            // Add it thank's to option
            // arg.descriptor.emplace_back(ATTRIB_BITANGEANTE_NAME, ATTRIB_BITANGEANTE_TYPE);
        }

        arg.initVerticeBuffer(pMesh->mNumVertices);
        arg.initIndicesBuffer(pMesh->mNumFaces * 3u);
        void* currentVerticePos = arg.data;

        for (size_t verticeId = 0; verticeId < pMesh->mNumVertices; ++verticeId)
        {
            if (pMesh->HasPositions())
            {
                *reinterpret_cast<decltype(pMesh->mVertices)>(currentVerticePos) = pMesh->mVertices[verticeId];
                currentVerticePos = static_cast<char*>(currentVerticePos) + sizeof(decltype(*pMesh->mVertices));
            }

            if (pMesh->HasNormals())
            {
                *reinterpret_cast<decltype(pMesh->mNormals)>(currentVerticePos) = pMesh->mNormals[verticeId];
                currentVerticePos = static_cast<char*>(currentVerticePos) + sizeof(decltype(*pMesh->mNormals));
            }

            for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
            {
                if (pMesh->HasTextureCoords(i))
                {
                    *reinterpret_cast<aiVector2D*>(currentVerticePos) = {pMesh->mTextureCoords[i][verticeId].x,
                                                                         pMesh->mTextureCoords[i][verticeId].y};
                    currentVerticePos = static_cast<char*>(currentVerticePos) + sizeof(aiVector2D);
                }
            }

            for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
            {
                if (pMesh->HasVertexColors(i))
                {
                    *reinterpret_cast<decltype(pMesh->mColors[i])>(currentVerticePos) = pMesh->mColors[i][verticeId];
                    currentVerticePos = static_cast<char*>(currentVerticePos) + sizeof(decltype(*pMesh->mColors[i]));
                }
            }

            if (pMesh->HasTangentsAndBitangents())
            {
                *reinterpret_cast<decltype(pMesh->mTangents)>(currentVerticePos) = pMesh->mTangents[verticeId];
                currentVerticePos = static_cast<char*>(currentVerticePos) + sizeof(decltype(*pMesh->mTangents));
            }
        }

        // Indices
        unsigned int* currentIndicePos = arg.indices;
        for (size_t iFace = 0; iFace < pMesh->mNumFaces; ++iFace)
        {
            *(currentIndicePos++) = pMesh->mFaces[iFace].mIndices[0];
            *(currentIndicePos++) = pMesh->mFaces[iFace].mIndices[1];
            *(currentIndicePos++) = pMesh->mFaces[iFace].mIndices[2];
        }

        // Generate bounding volume
        arg.boundingVolumeType = boundingVolumeType;

        std::filesystem::path dstPath = dstDirPath / pMesh->mName.C_Str();
        if (i != 0 &&
            pMesh->mName == scene->mMeshes[i - 1]->mName) // Add different name if the FBX contains mesh with same name
            dstPath += "0";

        std::filesystem::path dstMeshPath = dstPath;
        dstMeshPath += ENGINE_MESH_EXTENSION;

        writeMeshFile(dstMeshPath.string().c_str(), arg);

        prefModel.addSubModel(
            SubModel::CreateArg{matList[pMesh->mMaterialIndex],
                                &Engine::getInstance()->resourceManager.add<Mesh>(dstMeshPath.string().c_str(), arg)});

        if (pMesh->HasBones())
        {
            Skin::CreateArgs     skinArgs;
            Skeleton::CreateArgs skeletonArgs;
            Skeleton::readHierarchyData(skeletonArgs.m_root, scene->mRootNode);
            loadSkinAndSkeleton(skinArgs.m_verticesBoneData, skeletonArgs.m_boneInfoMap, pMesh);
            writeSkeletonFile((dstPath.string() + ENGINE_SKELETON_EXTENSION).c_str(), skeletonArgs);
            writeSkinFile((dstPath.string() + ENGINE_SKIN_EXTENSION).c_str(), skinArgs);
        }

        GPE_ASSERT(scene->mNumAnimations <= 1, "If the number of animations is higher");
    }

    // for (aiAnimation* aiAnim = scene->mAnimations[0]; &aiAnim < scene->mAnimations + scene->mNumAnimations; aiAnim++)
    for (size_t i = 0; i < scene->mNumAnimations; i++)
    {
        aiAnimation*          aiAnim   = scene->mAnimations[i];
        Animation::CreateArgs animArgs = Animation::CreateArgs(aiAnim);
        // m_currentAnimation = new Animation(scene->mAnimations[0]);
        std::filesystem::path dstAnimPath = dstDirPath / aiAnim->mName.C_Str();
        dstAnimPath += ENGINE_ANIMATION_EXTENSION;
        writeAnimationFile(dstAnimPath.string().c_str(), animArgs);
    }

    std::filesystem::path dstPrefPath = dstDirPath / fileName;
    dstPrefPath += ENGINE_PREFAB_EXTENSION;

    if (!prefab.getWorld().children.empty())
    {
        prefab.getWorld().children.front()->setName(dstPrefPath.stem().string().c_str());
        prefab.getWorld().children.front()->pOwnerScene = nullptr;
        saveSceneToPathImp(&prefab, dstPrefPath.string().c_str(), GPE::SavedScene::EType::XML);
        prefab.getWorld().children.clear();
    }

    Log::getInstance()->logInitializationEnd("Model importion");
}

static size_t getSizeFromCursorToEOF(FILE& file)
{
    int cursor = ftell(&file);
    fseek(&file, 0, SEEK_END);
    int end = ftell(&file);
    fseek(&file, cursor, SEEK_SET);
    return end - cursor;
}

struct TextureHeader
{
    char                      assetID = (char)EFileType::TEXTURE;
    Texture::RenderProperties props;
    int                       textureLenght = 0;
};

void GPE::importeTextureFile(const char* srcPath, const char* dstPath, const TextureImportConfig& config)
{
    Texture::ImportArg arg;

    stbi_set_flip_vertically_on_load(config.verticalFlip);
    arg.pixels.reset(stbi_load(srcPath, &arg.w, &arg.h, &arg.comp, 0));
    stbi_set_flip_vertically_on_load(false);

    writeTextureFile(dstPath, arg, config);
}

void GPE::writeTextureFile(const char* dst, const Texture::ImportArg& data, const TextureImportConfig& config)
{
    if (FILE* pFile = openFileToWrite(dst))
    {
        int                      newLen;
        std::unique_ptr<stbi_uc> imgData;

        switch (config.format)
        {
        case TextureImportConfig::EFormatType::PNG:
        default: {
            imgData.reset(stbi_write_png_to_mem((const unsigned char*)data.pixels.get(), data.w * data.comp, data.w,
                                                data.h, data.comp, &newLen));

            if (imgData == NULL)
            {
                FUNCT_ERROR((std::string("STBI cannot convert pixels to PNG to this dst : ") + dst).c_str());
            }

            break;
        }
        }

        TextureHeader header{(char)EFileType::TEXTURE, data.properties, newLen};
        fwrite(&header, sizeof(header), 1, pFile); // header

        fwrite(imgData.get(), newLen, 1, pFile);
        fclose(pFile);

        Log::getInstance()->log(stringFormat("Texture write to \"%s\"", dst));
    }
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

        arg.properties = header.props;
        arg.len        = header.textureLenght;

        std::vector<stbi_uc> texBuffer;
        texBuffer.assign(header.textureLenght, 0);

        fread(&texBuffer[0], sizeof(stbi_uc), header.textureLenght, pFile); // Texture buffer

        arg.pixels.reset(stbi_load_from_memory(texBuffer.data(), header.textureLenght, &arg.w, &arg.h, &arg.comp, 0));
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
    if (Texture* const pTexture = Engine::getInstance()->resourceManager.get<Texture>(src))
        return pTexture;

    return &Engine::getInstance()->resourceManager.add<Texture>(src, readTextureFile(src));
}

struct MaterialHeader
{
    char              assetID                  = (char)EFileType::MATERIAL;
    MaterialComponent component                = {};
    int               pathShaderLenght         = 0;
    int               pathDiffuseTextureLenght = 0;
    int               pathNormapTextureLenght  = 0;
    size_t            uniformLenght            = 0;
};

void GPE::writeMaterialFile(const char* dst, const Material& mat)
{
    if (FILE* pFile = openFileToWrite(dst))
    {
        std::string uniformStr = "";

        { // Save the uniforms buffers
            rapidxml::xml_document<> doc;
            XmlSaver                 context(doc);

            save(context, mat.getUniforms(), XmlSaver::SaveInfo{"uniforms", "Unordered_map", 0});
            uniformStr = docToString(doc);
        }

        std::string shaderPath;
        std::string diffuseTexturePath;
        std::string normalMapTexturePath;

        if (mat.getShader())
            if (const std::string* pStr = Engine::getInstance()->resourceManager.getKey(mat.getShader()))
                shaderPath = *pStr;

        if (mat.getDiffuseTexture())
            if (const std::string* pStr = Engine::getInstance()->resourceManager.getKey(mat.getDiffuseTexture()))
                diffuseTexturePath = *pStr;

        if (mat.getNormalMapTexture())
            if (const std::string* pStr = Engine::getInstance()->resourceManager.getKey(mat.getNormalMapTexture()))
                normalMapTexturePath = *pStr;

        MaterialHeader header{(char)EFileType::MATERIAL,
                              mat.getComponent(),
                              static_cast<int>(shaderPath.size()),
                              static_cast<int>(diffuseTexturePath.size()),
                              static_cast<int>(normalMapTexturePath.size()),
                              uniformStr.size()};

        fwrite(&header, sizeof(header), 1, pFile); // header

        fwrite(shaderPath.data(), sizeof(char), header.pathShaderLenght, pFile);                  // string buffer
        fwrite(diffuseTexturePath.data(), sizeof(char), header.pathDiffuseTextureLenght, pFile);  // string buffer
        fwrite(normalMapTexturePath.data(), sizeof(char), header.pathNormapTextureLenght, pFile); // string buffer
        fwrite(uniformStr.data(), sizeof(char), uniformStr.size(), pFile);                        // string buffer

        fclose(pFile);
        Log::getInstance()->log(stringFormat("Material write to \"%s\"", dst));
    }
}

void GPE::writeMaterialFile(const char* dst, const Material::ImporteArg& arg)
{
    if (FILE* pFile = openFileToWrite(dst))
    {
        std::string uniformStr = "";

        { // Save the uniforms buffers
            rapidxml::xml_document<> doc;
            XmlSaver                 context(doc);

            save(context, arg.uniforms, XmlSaver::SaveInfo{"uniforms", "Unordered_map", 0});
            uniformStr = docToString(doc);
        }

        MaterialHeader header{(char)EFileType::MATERIAL,
                              arg.comp,
                              static_cast<int>(arg.shaderPath.path.size()),
                              static_cast<int>(arg.diffuseTexturePath.path.size()),
                              static_cast<int>(arg.normalMapTexturePath.path.size()),
                              uniformStr.size()};

        fwrite(&header, sizeof(header), 1, pFile); // header

        fwrite(arg.shaderPath.path.data(), sizeof(char), header.pathShaderLenght, pFile); // string buffer
        fwrite(arg.diffuseTexturePath.path.data(), sizeof(char), header.pathDiffuseTextureLenght,
               pFile); // string buffer
        fwrite(arg.normalMapTexturePath.path.data(), sizeof(char), header.pathNormapTextureLenght,
               pFile);                                                     // string buffer
        fwrite(uniformStr.data(), sizeof(char), uniformStr.size(), pFile); // string buffer

        fclose(pFile);
        Log::getInstance()->log(stringFormat("Material write to \"%s\"", dst));
    }
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

    if (header.pathShaderLenght)
    {
        arg.shaderPath.path.assign(header.pathShaderLenght, '\0');
        fread(arg.shaderPath.path.data(), sizeof(char), header.pathShaderLenght,
              pFile); // string buffer
    }

    if (header.pathDiffuseTextureLenght)
    {
        arg.diffuseTexturePath.path.assign(header.pathDiffuseTextureLenght, '\0');
        fread(arg.diffuseTexturePath.path.data(), sizeof(char), header.pathDiffuseTextureLenght,
              pFile); // string buffer
    }

    if (header.pathNormapTextureLenght)
    {
        arg.normalMapTexturePath.path.assign(header.pathNormapTextureLenght, '\0');
        fread(arg.normalMapTexturePath.path.data(), sizeof(char), header.pathNormapTextureLenght,
              pFile); // string buffer
    }

    rapidxml::xml_document<> doc;
    XmlLoader                context(doc);

    if (header.uniformLenght)
    {
        std::string str;
        str.assign(header.uniformLenght, '\0');

        fread(str.data(), sizeof(char), header.uniformLenght, pFile); // string buffer
        doc.parse<0>(str.data());
        load(context, arg.uniforms, XmlLoader::LoadInfo{"uniforms", "Unordered_map", 0});
        doc.clear();
    }

    fclose(pFile);
    Log::getInstance()->log(stringFormat("Material read from \"%s\"", src));
    return arg;
}

Material* GPE::loadMaterialFile(const char* src)
{
    if (Material* const pMat = Engine::getInstance()->resourceManager.get<Material>(src))
        return pMat;

    Material::ImporteArg importeArg = readMaterialFile(src);
    Material::CreateArg  arg;
    arg.comp     = importeArg.comp;
    arg.uniforms = std::move(importeArg.uniforms);

    if (!importeArg.shaderPath.path.empty())
        arg.pShader = loadShaderFile(importeArg.shaderPath.path.c_str());

    if (!importeArg.diffuseTexturePath.path.empty())
        arg.pDiffuseTexture = loadTextureFile(importeArg.diffuseTexturePath.path.c_str());

    if (!importeArg.normalMapTexturePath.path.empty())
        arg.pNormalMapTexture = loadTextureFile(importeArg.normalMapTexturePath.path.c_str());

    return &Engine::getInstance()->resourceManager.add<Material>(src, std::move(arg));
}

struct MeshHeader
{
    char          assetID            = (char)EFileType::MESH;
    int           dataBufferSize     = 0;
    int           indiceCount        = 0;
    size_t        descriptorStrSize  = 0;
    unsigned char boundingVolumeType = (unsigned char)Mesh::EBoundingVolume::NONE;
};

void GPE::writeMeshFile(const char* dst, const Mesh::VertexData& arg)
{
    if (FILE* pFile = openFileToWrite(dst))
    {
        rapidxml::xml_document<> doc;
        XmlSaver                 context(doc);
        GPE::save(context, arg.descriptor, XmlSaver::SaveInfo{"Descriptor", "Vector<Shader::Attribute>", 0});
        std::string descriptorStr = docToString(doc);

        MeshHeader header{(char)EFileType::MESH, static_cast<int>(arg.dataBufferSize),
                          arg.indices ? static_cast<int>(arg.elemCount) : 0, descriptorStr.size(),
                          static_cast<unsigned char>(arg.boundingVolumeType)};
        fwrite(&header, sizeof(header), 1, pFile); // header

        fwrite(descriptorStr.data(), sizeof(char), descriptorStr.size(), pFile); // header

        fwrite(arg.data, header.dataBufferSize, 1, pFile);                    // vertice buffer
        fwrite(arg.indices, sizeof(unsigned int), header.indiceCount, pFile); // indice buffer

        for (unsigned int i = 0; i < header.indiceCount; ++i)
        {
            unsigned int a = arg.indices[i];
        }

        fclose(pFile);
        Log::getInstance()->log(stringFormat("Mesh write to \"%s\"", dst));
    }
}

void GPE::readMeshFile(const char* src, Mesh::VertexData& arg)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);

    if (srcPath.extension() != ENGINE_MESH_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return;
    }

    MeshHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);

    std::string descriptorStr;
    descriptorStr.assign(header.descriptorStrSize, '\0');
    fread(descriptorStr.data(), sizeof(char), header.descriptorStrSize, pFile); // string buffer

    rapidxml::xml_document<> doc;
    doc.parse<0>(descriptorStr.data());
    XmlLoader context(doc);
    {
        GPE::load(context, arg.descriptor, XmlLoader::LoadInfo{"Descriptor", "Vector<Shader::Attribute>", 0});
    }

    if (header.dataBufferSize)
    {
        arg.initVerticeBuffer(header.dataBufferSize / arg.getVertexSize());
        fread(arg.data, header.dataBufferSize, 1, pFile); // vertice buffer
    }

    if (header.indiceCount)
    {
        arg.initIndicesBuffer(header.indiceCount);
        fread(arg.indices, sizeof(unsigned int), header.indiceCount, pFile); // indice buffer
    }

    arg.boundingVolumeType = (Mesh::EBoundingVolume)header.boundingVolumeType;

    fclose(pFile);
    Log::getInstance()->log(stringFormat("Mesh read from \"%s\"", src));
}

Mesh* GPE::loadMeshFile(const char* src)
{
    if (Mesh* const pMesh = Engine::getInstance()->resourceManager.get<Mesh>(src))
        return pMesh;
    Mesh::VertexData arg;
    readMeshFile(src, arg);
    return &Engine::getInstance()->resourceManager.add<Mesh>(src, arg);
}

struct ShaderHeader
{
    char     assetID            = (char)EFileType::SHADER;
    int      vertexPathLenght   = 0;
    int      fragmentPathLenght = 0;
    uint16_t featureMask        = 0;
};

void GPE::writeShaderFile(const char* dst, const ShaderCreateConfig& arg)
{
    if (FILE* pFile = openFileToWrite(dst))
    {
        ShaderHeader header{(char)EFileType::SHADER, static_cast<int>(arg.vertexShaderPath.size()),
                            static_cast<int>(arg.fragmentShaderPath.size()), arg.featureMask};
        fwrite(&header, sizeof(header), 1, pFile); // header

        fwrite(arg.vertexShaderPath.data(), sizeof(char), arg.vertexShaderPath.size(), pFile);     // string buffer
        fwrite(arg.fragmentShaderPath.data(), sizeof(char), arg.fragmentShaderPath.size(), pFile); // string buffer

        fclose(pFile);

        Log::getInstance()->log(stringFormat("Shader write to \"%s\"", dst));
    }
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
    if (Shader* const pShader = Engine::getInstance()->resourceManager.get<Shader>(src))
        return pShader;

    ShaderCreateConfig arg = readShaderFile(src);

    return &Engine::getInstance()->resourceManager.add<Shader>(
        src, (std::filesystem::current_path() / arg.vertexShaderPath).string().c_str(),
        (std::filesystem::current_path() / arg.fragmentShaderPath).string().c_str(), arg.featureMask);
}

struct SceneHeader
{
    char     assetID = (char)EFileType::SCENE;
    uint16_t type    = 0;
};

struct PrefabHeader
{
    char     assetID = (char)EFileType::PREFAB;
    uint16_t type    = 0;
};

void GPE::writePrefabFile(const char* dst, const SavedScene::CreateArg& arg)
{
    if (FILE* pFile = openFileToWrite(dst))
    {
        SceneHeader header{(char)EFileType::PREFAB, (uint16_t)arg.type};
        fwrite(&header, sizeof(header), 1, pFile); // header

        fwrite(arg.data.data(), sizeof(char), arg.data.size(), pFile); // string buffer
        fclose(pFile);

        Log::getInstance()->log(stringFormat("Prefab write to \"%s\"", dst));
    }
}

SavedScene::CreateArg GPE::readPrefabFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);
    SavedScene::CreateArg arg;

    if ((srcPath.extension() == ENGINE_PREFAB_EXTENSION) && fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    SceneHeader header;

    // copy the file into the buffer. Read from head to EOF
    fread(&header, sizeof(header), 1, pFile);
    arg.type = (SavedScene::EType)header.type;

    size_t sizeRemainig = getSizeFromCursorToEOF(*pFile);
    arg.data.assign(sizeRemainig, '\0');
    fread(arg.data.data(), sizeof(char), sizeRemainig, pFile); // string buffer

    fclose(pFile);

    Log::getInstance()->log(stringFormat("Prefab read from \"%s\"", src));
    return arg;
}

SavedScene::CreateArg GPE::loadPrefabFile(const char* src)
{
    return readPrefabFile(src);
}

void GPE::writeSceneFile(const char* dst, const SavedScene::CreateArg& arg)
{
    if (FILE* pFile = openFileToWrite(dst))
    {
        SceneHeader header{(char)EFileType::SCENE, (uint16_t)arg.type};
        fwrite(&header, sizeof(header), 1, pFile); // header

        fwrite(arg.data.data(), sizeof(char), arg.data.size(), pFile); // string buffer
        fclose(pFile);

        Log::getInstance()->log(stringFormat("Scene write to \"%s\"", dst));
    }
}

#include <fstream>
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

    // copy the file into the buffer. Read from head to EOF
    fread(&header, sizeof(header), 1, pFile);
    arg.type = (SavedScene::EType)header.type;

    size_t sizeRemainig = getSizeFromCursorToEOF(*pFile);

    arg.data.assign(sizeRemainig, '\0');
    fread(arg.data.data(), sizeof(char), sizeRemainig, pFile); // string buffer

    fclose(pFile);

    Log::getInstance()->log(stringFormat("Scene read from \"%s\"", src));
    return arg;
}

SavedScene::CreateArg GPE::loadSceneFile(const char* src)
{
    return readSceneFile(src);
}

struct SkeletonHeader
{
    char assetID = (char)EFileType::SKELETON;
    int  nbBones;
};

void GPE::writeSkeletonFile(const char* dst, const Skeleton::CreateArgs& arg)
{
    if (FILE* pFile = openFileToWrite(dst))
    {
        SkeletonHeader header{(char)EFileType::MESH, int(arg.m_boneInfoMap.size())};

        fwrite(&header, sizeof(header), 1, pFile); // header

        GPE::BinarySaver saver;
        saver.file = pFile;
        GPE::save(saver, arg.m_boneInfoMap, nullptr);
        GPE::save(saver, arg.m_root, nullptr);

        fclose(pFile);
        Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
    }
}

Skeleton::CreateArgs GPE::readSkeletonFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);
    Skeleton::CreateArgs  arg;

    if (srcPath.extension() != ENGINE_SKELETON_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    SkeletonHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);

    GPE::BinaryLoader loader;
    loader.file = pFile;
    GPE::load(loader, arg.m_boneInfoMap, nullptr);
    GPE::load(loader, arg.m_root, nullptr);

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File read from \"%s\"", src));

    return arg;
}

Skeleton* GPE::loadSkeletonFile(const char* src)
{
    std::filesystem::path srcPath(src);

    if (Skeleton* const pSkeleton = Engine::getInstance()->animResourcesManager.get<Skeleton>(src))
        return pSkeleton;
    return &Engine::getInstance()->animResourcesManager.add<Skeleton>(src, readSkeletonFile(src), src);
}

struct AnimationHeader
{
    char assetID       = (char)EFileType::ANIMATION;
    int  verticeLength = 0;
    int  indiceLength  = 0;
};

void GPE::writeAnimationFile(const char* dst, const Animation::CreateArgs& arg)
{
    if (FILE* pFile = openFileToWrite(dst))
    {
        AnimationHeader header{(char)EFileType::ANIMATION};
        fwrite(&header, sizeof(header), 1, pFile); // header
        GPE::BinarySaver saver;
        saver.file = pFile;
        GPE::save(saver, arg.duration, nullptr);
        GPE::save(saver, arg.nbTicksPerSecond, nullptr);
        GPE::save(saver, arg.bones, nullptr);

        fclose(pFile);
        Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
    }
}

Animation::CreateArgs GPE::readAnimationFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);
    Animation::CreateArgs arg;

    if (srcPath.extension() != ENGINE_ANIMATION_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    AnimationHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);
    GPE::BinaryLoader loader;
    loader.file = pFile;
    GPE::load(loader, arg.duration, nullptr);
    GPE::load(loader, arg.nbTicksPerSecond, nullptr);
    GPE::load(loader, arg.bones, nullptr);

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File read from \"%s\"", src));

    return arg;
}

Animation* GPE::loadAnimationFile(const char* src)
{
    std::hash<std::string>();
    std::filesystem::path srcPath(src);

    if (Animation* const pAnim = Engine::getInstance()->animResourcesManager.get<Animation>(src))
        return pAnim;
    return &Engine::getInstance()->animResourcesManager.add<Animation>(src, readAnimationFile(src), src);
}

struct SkinHeader
{
    char assetID = (char)EFileType::SKIN;
};

void GPE::writeSkinFile(const char* dst, const Skin::CreateArgs& arg)
{
    if (FILE* pFile = openFileToWrite(dst))
    {
        MeshHeader header{(char)EFileType::MESH};
        fwrite(&header, sizeof(header), 1, pFile); // header

        GPE::BinarySaver saver;
        saver.file = pFile;
        GPE::save(saver, arg.m_verticesBoneData, nullptr);

        fclose(pFile);
        Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
    }
}

Skin::CreateArgs GPE::readSkinFile(const char* src)
{
    FILE*                 pFile = nullptr;
    std::filesystem::path srcPath(src);
    Skin::CreateArgs      arg;

    if (srcPath.extension() != ENGINE_SKIN_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    MeshHeader header;
    // copy the file into the buffer:
    fread(&header, sizeof(header), 1, pFile);

    GPE::BinaryLoader loader;
    loader.file = pFile;
    GPE::load(loader, arg.m_verticesBoneData, nullptr);

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File read from \"%s\"", src));

    return arg;
}

Skin* GPE::loadSkinFile(const char* src)
{
    std::filesystem::path srcPath(src);

    if (Skin* const pSkin = Engine::getInstance()->animResourcesManager.get<Skin>(src))
        return pSkin;
    return &Engine::getInstance()->animResourcesManager.add<Skin>(src, readSkinFile(src));
}
