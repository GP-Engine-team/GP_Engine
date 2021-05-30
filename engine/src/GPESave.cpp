#include "Engine/Serialization/GPESave.hpp"

#include "Engine/Engine.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Type.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Color.hpp>
#include <Engine/Resources/Prefab.hpp>
#include <Engine/Resources/ResourcesManager.hpp>
#include <Engine/Resources/Linker.hpp>

namespace GPE
{

template <>
void save(XmlSaver& context, const AmbiantComponent& data, const XmlSaver::SaveInfo& info)
{
    GPE::save(context, data.rgbi, info);
}

template <>
void save(XmlSaver& context, const ColorRGBA& data, const rfk::Field& info)
{
    GPE::save(context, data.v, info);
}

template <>
void save(XmlSaver& context, const ColorRGBA& data, const XmlSaver::SaveInfo& info)
{
    GPE::save(context, data.v, info);
}

template <>
void save(XmlSaver& context, const ColorRGB& data, const rfk::Field& info)
{
    GPE::save(context, data.v, info);
}

template <>
void save(XmlSaver& context, const ColorRGB& data, const XmlSaver::SaveInfo& info)
{
    GPE::save(context, data.v, info);
}

template <>
void save(XmlSaver& context, Prefab* const& data, const rfk::Field& info)
{
    GPE::save(context, data, fieldToSaveInfo(info));
}

template <>
void save(XmlSaver& context, Prefab* const& data, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    if (data != nullptr)
    {
        for (auto& [key, value] : GPE::Engine::getInstance()->resourceManager.getAll<SharedPrefab>())
        {
            if (&value.pref == data)
            {
                GPE::save(context, key, XmlSaver::SaveInfo{info.name, info.typeName, 0});
                break;
            }
        }
    }

    context.pop();
}

inline void saveGameObjectLinker(XmlSaver& context, const GPE::Linker<GPE::GameObject>& data,
                                             const XmlSaver::SaveInfo& info)
{
    std::string str = "";
    if (data.pData)
    {
        str = data.pData->getAbsolutePath();
        str.erase(0, str.find_first_of('/', 0) + 1); // remove the world
    }

    GPE::save(context, str, XmlSaver::SaveInfo{"GLinker", "GLinker", 0});
}

template <>
void GPE::save<GPE::Linker<GPE::GameObject>>(XmlSaver& context, const GPE::Linker<GPE::GameObject>& data,
                                const XmlSaver::SaveInfo& info)
{
    saveGameObjectLinker(context, data, info);
}

template <>
void GPE::save<GPE::GameObject>(XmlSaver& context, const GPE::Linker<GPE::GameObject>& data,
                                             const XmlSaver::SaveInfo& info)
{
    saveGameObjectLinker(context, data, info);
}

template <>
void save(XmlSaver& context, Shader* const& data, const rfk::Field& info)
{
    GPE::save(context, data, fieldToSaveInfo(info));
}

template <>
void save(XmlSaver& context, Shader* const& data, const XmlSaver::SaveInfo& info)
{
    if (data == nullptr)
        return;

    if (const std::string* name = GPE::Engine::getInstance()->resourceManager.getKey(data))
    {
        GPE::save(context, *name, XmlSaver::SaveInfo{"pShader", "Shader*", 0});
    }
}

template <>
void save(XmlSaver& context, Material* const& data, const rfk::Field& info)
{
    GPE::save(context, data, fieldToSaveInfo(info));
}

template <>
void save(XmlSaver& context, Material* const& data, const XmlSaver::SaveInfo& info)
{
    if (data == nullptr)
        return;

    if (const std::string* name = GPE::Engine::getInstance()->resourceManager.getKey(data))
    {
        GPE::save(context, *name, XmlSaver::SaveInfo{"pMaterial", "Material*", 0});
    }
}

template <>
void save(XmlSaver& context, Mesh* const& data, const rfk::Field& info)
{
    GPE::save(context, data, fieldToSaveInfo(info));
}

template <>
void save(XmlSaver& context, Mesh* const& data, const XmlSaver::SaveInfo& info)
{
    if (data == nullptr)
        return;

    if (const std::string* name = GPE::Engine::getInstance()->resourceManager.getKey(data))
    {
        GPE::save(context, *name, XmlSaver::SaveInfo{"pMesh", "Mesh*", 0});
    }
}

template <>
void save(XmlSaver& context, Texture* const& data, const rfk::Field& info)
{
    GPE::save(context, data, fieldToSaveInfo(info));
}

template <>
void save(XmlSaver& context, Texture* const& data, const XmlSaver::SaveInfo& info)
{
    if (data == nullptr)
        return;

    if (const std::string* name = GPE::Engine::getInstance()->resourceManager.getKey(data))
    {
        GPE::save(context, *name, XmlSaver::SaveInfo{"pTexture", "Texture*", 0});
    }
}

} // namespace GPE