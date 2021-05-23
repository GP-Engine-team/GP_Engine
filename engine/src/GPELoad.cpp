#include "Engine/Serialization/GPELoad.hpp"

#include "Engine/Resources/Type.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Resources/Prefab.hpp>
#include <Engine/Resources/ResourcesManager.hpp>

namespace GPE
{

template <>
void load(XmlLoader& context, AmbiantComponent& data, const XmlLoader::LoadInfo& info)
{
    GPE::load(context, data.rgbi, info);
}

// template <>
// void load(XmlLoader& context, Prefab*& data, const rfk::Field& info)
//{
//    GPE::load(context, data, fieldToLoadInfo(info));
//}

template <>
void load(XmlLoader& context, Prefab*& data, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        std::string path;
        GPE::load(context, path, XmlLoader::LoadInfo{info.name, info.typeName, 0});

        GameObject* go = nullptr;
        if (SharedPrefab* pSPref = Engine::getInstance()->resourceManager.get<SharedPrefab>(path.c_str()))
        {
            data = &pSPref->pref;
            ++pSPref->instanceCounter;
        }
        else
        {
            SharedPrefab& sPref = Engine::getInstance()->resourceManager.add<SharedPrefab>(path.c_str(), path.c_str());
            data                = &sPref.pref;
            ++sPref.instanceCounter;
        }
        context.pop();
    }
}

} // namespace GPE