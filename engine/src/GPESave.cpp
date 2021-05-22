#include "Engine/Serialization/GPESave.hpp"

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

} // namespace GPE