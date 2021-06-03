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
    GPE::load(context, shaderName, XmlLoader::LoadInfo{"pShader", "Shader*", 0});
    if (!(data = Engine::getInstance()->resourceManager.get<GPE::Shader>(shaderName)))
    {
        data = loadShaderFile(shaderName.c_str());
    }
}

template <>
void load(XmlLoader& context, Mesh*& data, const XmlLoader::LoadInfo& info)
{
    std::string MeshName;
    GPE::load(context, MeshName, XmlLoader::LoadInfo{"pMesh", "Mesh*", 0});
    if (!(data = Engine::getInstance()->resourceManager.get<GPE::Mesh>(MeshName)))
    {
        data = loadMeshFile(MeshName.c_str());
    }
}

template <>
void load(XmlLoader& context, Material*& data, const XmlLoader::LoadInfo& info)
{
    std::string MaterialName;
    GPE::load(context, MaterialName, XmlLoader::LoadInfo{"pMaterial", "Material*", 0});
    if (!(data = Engine::getInstance()->resourceManager.get<GPE::Material>(MaterialName)))
    {
        data = loadMaterialFile(MaterialName.c_str());
    }
}

template <>
void load(XmlLoader& context, Texture*& data, const XmlLoader::LoadInfo& info)
{
    std::string TextureName;
    GPE::load(context, TextureName, XmlLoader::LoadInfo{"pTexture", "Texture*", 0});
    if (!(data = Engine::getInstance()->resourceManager.get<GPE::Texture>(TextureName)))
    {
        data = loadTextureFile(TextureName.c_str());
    }
}

} // namespace GPE