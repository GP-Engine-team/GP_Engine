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
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/ResourcesManagerType.hpp>
#include <Engine/Resources/Texture.hpp>
#include <Engine/Serialization/binary/BinarySaver.hpp>
#include <Engine/Serialization/binary/BinaryLoader.hpp>
#include <GPM/Shape3D/AABB.hpp>
#include <GPM/Shape3D/Sphere.hpp>

#include <Engine/Core/Tools/Hash.hpp>

using namespace GPE;
using namespace GPM;

void GPE::saveSceneToPathImp(GPE::Scene* scene, const char* path, GPE::SavedScene::EType saveMode)
{
    if (saveMode == GPE::SavedScene::EType::XML)
    {
        rapidxml::xml_document<> doc;
        XmlSaver                 context(doc);
        context.addWeakPtr(scene);
        scene->save(context);

        GPE::SavedScene::CreateArg args;
        args.data = docToString(doc);
        args.type = GPE::SavedScene::EType::XML;

        GPE::writeSceneFile(path, args);
    }
}

void GPE::loadSceneFromPathImp(GPE::Scene* scene, const char* path)
{
    GPE::SavedScene::CreateArg savedScene = GPE::readSceneFile(path);

    if (savedScene.type == GPE::SavedScene::EType::XML)
    {
        // Load xml doc
        rapidxml::xml_document<> doc;
        std::unique_ptr<char[]>  buffer;
        GPE::SavedScene::toDoc(doc, buffer, savedScene.data.c_str(), savedScene.data.size());

        XmlLoader context(doc);
        // Load each element
        scene->load(context);

        // Tell that pointers to the old scene should be replaced by pointers to the new scene
        context.addConvertedPtr(scene->getWorld().pOwnerScene, scene);

        // Update old pointers into new ones
        context.updateLazyPtrs();

        // Call onPostLoad on GameObjects
        struct Rec
        {
            static void rec(GPE::GameObject* g)
            {
                g->getTransform().onPostLoad();

                for (GPE::Component* comp : g->getComponents())
                {
                    comp->onPostLoad();
                }

                for (GPE::GameObject* g2 : g->children)
                {
                    rec(g2);
                }
            };
        };
        Rec::rec(&scene->getWorld()); // can't do recursives with lambdas, and std::function would be overkill
    }
}

GPE::GameObject* GPE::loadPrefabFromPathImp(GPE::GameObject& parent, const char* path)
{
    Scene                      scene;
    GPE::SavedScene::CreateArg savedScene = GPE::readSceneFile(path);

    // Load the file
    if (savedScene.type == GPE::SavedScene::EType::XML)
    {
        // Load xml doc
        rapidxml::xml_document<> doc;
        std::unique_ptr<char[]>  buffer;
        GPE::SavedScene::toDoc(doc, buffer, savedScene.data.c_str(), savedScene.data.size());

        XmlLoader context(doc);
        // Load each element
        scene.load(context);

        // Tell that pointers to the old scene should be replaced by pointers to the new scene
        context.addConvertedPtr(scene.getWorld().pOwnerScene, &scene);

        // Update old pointers into new ones
        context.updateLazyPtrs();
    }

    // Init the prefab
    GameObject* const go = scene.getWorld().children.front();
    if (go)
    {
        go->setParent(&parent);
        go->getTransform().setDirty();

        // Call onPostLoad on GameObjects
        struct Rec
        {
            static void rec(GPE::GameObject* const g)
            {
                g->getTransform().onPostLoad();

                for (GPE::Component* comp : g->getComponents())
                {
                    comp->onPostLoad();
                }

                for (GPE::GameObject* g2 : g->children)
                {
                    rec(g2);
                }
            };
        };
        Rec::rec(go); // can't do recursives with lambdas, and std::function would be overkill
    }

    scene.getWorld().children.clear();
    return go;
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

                fsDstPath = dstDirPath / std::filesystem::path(texture->mFilename.C_Str()).stem();
                fsDstPath.replace_extension(ENGINE_TEXTURE_EXTENSION);
                materialArg.ambianteTexturePath = std::filesystem::relative(fsDstPath).string().c_str();

                stbi_uc* pix = stbi_load_from_memory(arg.pixels.get(), arg.len, &arg.w, &arg.h, &arg.comp, 0);
                arg.pixels.release(); // Assimp manage it's mamory
                arg.pixels.reset(pix);
                writeTextureFile(fsDstPath.string().c_str(), arg);
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

        writeMaterialFile(std::filesystem::relative(dstMaterialPath).string().c_str(), materialArg);

        // TODO: Not efficient to pass from HD -> RAM -> HD -> RAM
        matList.emplace_back(loadMaterialFile(dstMaterialPath.string().c_str()));
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

            //GPE::Mesh::setVertexBoneDataToDefault(arg.vertices.back());
        }

        // Indices
        for (size_t iFace = 0; iFace < pMesh->mNumFaces; ++iFace)
        {
            arg.indices.emplace_back(pMesh->mFaces[iFace].mIndices[0]);
            arg.indices.emplace_back(pMesh->mFaces[iFace].mIndices[1]);
            arg.indices.emplace_back(pMesh->mFaces[iFace].mIndices[2]);
        }

        // Generate bounding volume
        arg.boundingVolumeType = boundingVolumeType;

        std::filesystem::path dstPath = dstDirPath / pMesh->mName.C_Str();
        if (i != 0 &&
            pMesh->mName == scene->mMeshes[i - 1]->mName) // Add differente name if the FBX containe mesh with same name
            dstPath += "0";

        std::filesystem::path dstMeshPath = dstPath;
        dstMeshPath += ENGINE_MESH_EXTENSION;

        writeMeshFile(dstMeshPath.string().c_str(), arg);

        // DefaultNormalMap
        const char* idShader =
            matList[pMesh->mMaterialIndex]->getNormalMapTexture() ? "DefaultWithNormalMap" : "Default";

        prefModel.addSubModel(SubModel::CreateArg{
            Engine::getInstance()->resourceManager.get<Shader>(idShader), matList[pMesh->mMaterialIndex],
            &Engine::getInstance()->resourceManager.add<Mesh>(dstMeshPath.string().c_str(), arg)});



        Skin::CreateArgs     skinArgs;
        Skeleton::CreateArgs skeletonArgs;
        Skeleton::readHierarchyData(skeletonArgs.m_root, scene->mRootNode);
        loadSkinAndSkeleton(skinArgs.m_verticesBoneData, skeletonArgs.m_boneInfoMap, pMesh);
        writeSkeletonFile((dstPath.string() + ENGINE_SKELETON_EXTENSION).c_str(), skeletonArgs);
        writeSkinFile((dstPath.string() + ENGINE_SKIN_EXTENSION).c_str(), skinArgs);
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
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    int                      newLen;
    std::unique_ptr<stbi_uc> imgData;

    switch (config.format)
    {
    case TextureImportConfig::EFormatType::PNG:
    default: {
        imgData.reset(stbi_write_png_to_mem((const unsigned char*)data.pixels.get(), data.w * data.comp, data.w, data.h,
                                            data.comp, &newLen));

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
        arg.normalMapTexturePath.assign(header.pathBaseColorTextureLenght, '\0');
        fread(arg.normalMapTexturePath.data(), sizeof(char), header.pathBaseColorTextureLenght,
              pFile); // string buffer
    }

    fclose(pFile);
    Log::getInstance()->log(stringFormat("Material read from \"%s\"", src));
    return arg;
}

Material* GPE::loadMaterialFile(const char* src)
{
    Material::ImporteArg importeArg = readMaterialFile(src);
    Material::CreateArg  arg;
    arg.comp = importeArg.comp;

    if (!importeArg.ambianteTexturePath.empty())
        arg.pAmbianteTexture = loadTextureFile(importeArg.ambianteTexturePath.c_str());

    if (!importeArg.diffuseTexturePath.empty())
        arg.pDiffuseTexture = loadTextureFile(importeArg.diffuseTexturePath.c_str());

    if (!importeArg.normalMapTexturePath.empty())
        arg.pNormalMapTexture = loadTextureFile(importeArg.normalMapTexturePath.c_str());

    if (Material* const pMat = Engine::getInstance()->resourceManager.get<Material>(src))
        return pMat;
    return &Engine::getInstance()->resourceManager.add<Material>(src, arg);
}

struct MeshHeader
{
    char          assetID            = (char)EFileType::MESH;
    int           verticeLenght      = 0;
    int           indiceLenght       = 0;
    unsigned char boundingVolumeType = (unsigned char)Mesh::EBoundingVolume::NONE;
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
                      static_cast<int>(arg.indices.size()), static_cast<unsigned char>(arg.boundingVolumeType)};
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

    arg.boundingVolumeType = (Mesh::EBoundingVolume)header.boundingVolumeType;

    fclose(pFile);
    Log::getInstance()->log(stringFormat("Mesh read from \"%s\"", src));

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
    char     assetID            = (char)EFileType::SHADER;
    int      vertexPathLenght   = 0;
    int      fragmentPathLenght = 0;
    uint16_t featureMask        = 0;
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
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    SceneHeader header{(char)EFileType::PREFAB, (uint16_t)arg.type};
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

    if ((srcPath.extension() == ENGINE_PREFAB_EXTENSION) && fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    SceneHeader header;

    // copy the file into the buffer. Read from head to EOF
    fread(&header, sizeof(header), 1, pFile);
    int cursor = ftell(pFile);
    fseek(pFile, 0, SEEK_END);
    int end = ftell(pFile);
    fseek(pFile, cursor, SEEK_SET);
    int sizeRemainig = end - cursor;

    arg.type = (SavedScene::EType)header.type;
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
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    SceneHeader header{(char)EFileType::SCENE, (uint16_t)arg.type};
    fwrite(&header, sizeof(header), 1, pFile); // header

    fwrite(arg.data.data(), sizeof(char), arg.data.size(), pFile); // string buffer
    fclose(pFile);

    Log::getInstance()->log(stringFormat("Scene write to \"%s\"", dst));
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
    int cursor = ftell(pFile);
    fseek(pFile, 0, SEEK_END);
    int end = ftell(pFile);
    fseek(pFile, cursor, SEEK_SET);
    int sizeRemainig = end - cursor;

    arg.type = (SavedScene::EType)header.type;
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
    char assetID       = (char)EFileType::SKELETON;
    int  nbBones;
};

void GPE::writeSkeletonFile(const char* dst, const Skeleton::CreateArgs& arg)
{
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    SkeletonHeader header{(char)EFileType::MESH, arg.m_boneInfoMap.size()};

    fwrite(&header, sizeof(header), 1, pFile); // header

    GPE::BinarySaver saver;
    saver.file = pFile;
    GPE::save(saver, arg.m_boneInfoMap, nullptr);
    GPE::save(saver, arg.m_root, nullptr);

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
}

Skeleton::CreateArgs GPE::readSkeletonFile(const char* src)
{
    FILE*                       pFile = nullptr;
    std::filesystem::path       srcPath(src);
    Skeleton::CreateArgs        arg;

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

     if (Skeleton* const pSkeleton =
     Engine::getInstance()->animResourcesManager.get<Skeleton>(srcPath.filename().string()))
        return pSkeleton;
     return &Engine::getInstance()->animResourcesManager.add<Skeleton>(srcPath.filename().string(), readSkeletonFile(src));
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

    // MeshHeader header{(char)EFileType::MESH, static_cast<int>(arg.vertices.size()),
    //                  static_cast<int>(arg.indices.size())};
    // fwrite(&header, sizeof(header), 1, pFile);                                         // header
    // fwrite(arg.vertices.data(), sizeof(arg.vertices[0]), header.verticeLength, pFile); // vertice buffer
    // fwrite(arg.indices.data(), sizeof(arg.indices[0]), header.indiceLength, pFile);    // indice buffer

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
}

Animation::CreateArgs GPE::readAnimationFile(const char* src)
{
    FILE*                       pFile = nullptr;
    std::filesystem::path       srcPath(src);
    Animation::CreateArgs       arg;

    if (srcPath.extension() != ENGINE_ANIMATION_EXTENSION || fopen_s(&pFile, src, "rb"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", src));
        return arg;
    }

    // MeshHeader header;
    //// copy the file into the buffer:
    // fread(&header, sizeof(header), 1, pFile);

    // if (header.verticeLength)
    //{
    //    arg.vertices.assign(header.verticeLength, Mesh::Vertex{});
    //    fread(&arg.vertices[0], sizeof(arg.vertices[0]), header.verticeLength, pFile); // vertice buffer
    //}

    // if (header.indiceLength)
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

    // if (Animation* const pAnim = Engine::getInstance()->resourceManager.get<Animation>(srcPath.filename().string()))
    //    return pAnim;
    // return &Engine::getInstance()->resourceManager.add<Animation>(srcPath.filename().string(), readMeshFile(src));
    return nullptr;
}


struct SkinHeader
{
    char assetID       = (char)EFileType::SKIN;
};

void GPE::writeSkinFile(const char* dst, const Skin::CreateArgs& arg)
{
    FILE* pFile = nullptr;

    if (fopen_s(&pFile, dst, "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", dst));
        return;
    }

    MeshHeader header{(char)EFileType::MESH};
    fwrite(&header, sizeof(header), 1, pFile); // header

    GPE::BinarySaver saver;
    saver.file = pFile;
    GPE::save(saver, arg.m_verticesBoneData, nullptr);

    fclose(pFile);
    Log::getInstance()->log(stringFormat("File write to \"%s\"", dst));
}

Skin::CreateArgs GPE::readSkinFile(const char* src)
{
    FILE*                       pFile = nullptr;
    std::filesystem::path       srcPath(src);
    Skin::CreateArgs            arg;

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

     if (Skin* const pSkin = Engine::getInstance()->animResourcesManager.get<Skin>(srcPath.filename().string()))
        return pSkin;
     return &Engine::getInstance()->animResourcesManager.add<Skin>(srcPath.filename().string(), readSkinFile(src));
}
