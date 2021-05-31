#include "Engine/Serialization/GPESave.hpp"

#include "Engine/Engine.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Type.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Resources/Prefab.hpp>
#include <Engine/Resources/ResourcesManager.hpp>

namespace GPE
{

template <>
void save(XmlSaver& context, const AmbiantComponent& data, const XmlSaver::SaveInfo& info)
{
    GPE::save(context, data.rgbi, info);
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
void save(XmlSaver& context, Texture* const& data, const XmlSaver::SaveInfo& info)
{
    if (data == nullptr)
        return;

    if (const std::string* name = GPE::Engine::getInstance()->resourceManager.getKey(data))
    {
        GPE::save(context, *name, XmlSaver::SaveInfo{"pTexture", "Texture*", 0});
    }
}

template <>
void save(XmlSaver& context, Animation* const& data, const XmlSaver::SaveInfo& info)
{
    if (data == nullptr)
        return;

    if (const std::string* name = GPE::Engine::getInstance()->animResourcesManager.getKey(data))
    {
        GPE::save(context, *name, info);
    }
}

template <>
void save(XmlSaver& context, Skeleton* const& data, const XmlSaver::SaveInfo& info)
{
    if (data == nullptr)
        return;

    if (const std::string* name = GPE::Engine::getInstance()->animResourcesManager.getKey(data))
    {
        GPE::save(context, *name, info);
    }
}

template <>
void save(XmlSaver& context, Skin* const& data, const XmlSaver::SaveInfo& info)
{
    if (data == nullptr)
        return;

    if (const std::string* name = GPE::Engine::getInstance()->animResourcesManager.getKey(data))
    {
        GPE::save(context, *name, info);
    }
}

} // namespace GPE