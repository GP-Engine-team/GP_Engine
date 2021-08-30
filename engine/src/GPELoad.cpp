#include "Engine/Serialization/GPELoad.hpp"

#include "Engine/Engine.hpp"
#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Texture.hpp"
#include "Engine/Resources/Type.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Prefab.hpp>
#include <Engine/Resources/ResourcesManager.hpp>

namespace GPE
{

template <>
void load(XmlLoader& context, AmbiantComponent& data, const XmlLoader::LoadInfo& info)
{
    GPE::load(context, data.rgbi, info);
}

template <>
void load(XmlLoader& context, ColorRGBA& data, const XmlLoader::LoadInfo& info)
{
    GPE::load(context, data.v, info);
}

template <>
void load(XmlLoader& context, ColorRGB& data, const XmlLoader::LoadInfo& info)
{
    GPE::load(context, data.v, info);
}

template <>
void load(XmlLoader& context, Prefab*& data, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        std::string path;
        GPE::load(context, path, XmlLoader::LoadInfo{info.name, info.typeName, 0});

        if (path.size())
        {
            GameObject* go = nullptr;
            if (SharedPrefab* pSPref = Engine::getInstance()->resourceManager.get<SharedPrefab>(path.c_str()))
            {
                data = &pSPref->pref;
                ++pSPref->instanceCounter;
            }
            else
            {
                SharedPrefab& sPref =
                    Engine::getInstance()->resourceManager.add<SharedPrefab>(path.c_str(), path.c_str());
                data = &sPref.pref;
                ++sPref.instanceCounter;
            }
        }
        else
        {
            data = nullptr;
        }
        context.pop();
    }
}

template <>
void load(XmlLoader& context, Shader*& data, const XmlLoader::LoadInfo& info)
{
    std::string shaderName;
    GPE::load(context, shaderName, XmlLoader::LoadInfo{info.name, info.typeName, 0});
    if (!(data = Engine::getInstance()->resourceManager.get<GPE::Shader>(shaderName)))
    {
        data = loadShaderFile(shaderName.c_str());
    }
}

template <>
void load(XmlLoader& context, Mesh*& data, const XmlLoader::LoadInfo& info)
{
    std::string meshName;
    GPE::load(context, meshName, XmlLoader::LoadInfo{info.name, info.typeName, 0});
    if (!(data = Engine::getInstance()->resourceManager.get<GPE::Mesh>(meshName)))
    {
        data = loadMeshFile(meshName.c_str());
    }
}

template <>
void load(XmlLoader& context, Material*& data, const XmlLoader::LoadInfo& info)
{
    std::string materialName;
    GPE::load(context, materialName, XmlLoader::LoadInfo{info.name, info.typeName, 0});
    if (!(data = Engine::getInstance()->resourceManager.get<GPE::Material>(materialName)))
    {
        data = loadMaterialFile(materialName.c_str());
    }
}

template <>
void load(XmlLoader& context, Texture*& data, const XmlLoader::LoadInfo& info)
{
    std::string textureName;
    GPE::load(context, textureName, XmlLoader::LoadInfo{info.name, info.typeName, 0});
    if (!(data = Engine::getInstance()->resourceManager.get<GPE::Texture>(textureName)))
    {
        data = loadTextureFile(textureName.c_str());
    }
}

template <>
void load(XmlLoader& context, Animation*& data, const XmlLoader::LoadInfo& info)
{
    std::string animName;
    GPE::load(context, animName, info);
    if (animName.empty())
    {
        data = nullptr;
    }
    else if (!(data = Engine::getInstance()->animResourcesManager.get<GPE::Animation>(animName)))
    {
        data = loadAnimationFile(animName.c_str());
    }
}

template <>
void load(XmlLoader& context, Skeleton*& data, const XmlLoader::LoadInfo& info)
{
    std::string skeletonName;
    GPE::load(context, skeletonName, info);
    if (skeletonName.empty())
    {
        data = nullptr;
    }
    else if (!(data = Engine::getInstance()->animResourcesManager.get<GPE::Skeleton>(skeletonName)))
    {
        data = loadSkeletonFile(skeletonName.c_str());
    }
}

template <>
void load(XmlLoader& context, Skin*& data, const XmlLoader::LoadInfo& info)
{
    std::string skinName;
    GPE::load(context, skinName, info);
    if (skinName.empty())
    {
        data = nullptr;
    }
    else if (!(data = Engine::getInstance()->animResourcesManager.get<GPE::Skin>(skinName)))
    {
        data = loadSkinFile(skinName.c_str());
    }
}

} // namespace GPE