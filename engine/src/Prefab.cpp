#include <Engine/Resources/Prefab.hpp>

#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Scene.hpp>

#include <filesystem>

// Generated
#include "Generated/Prefab.rfk.h"
File_GENERATED

    using namespace GPE;

void Prefab::loadPrefabFromPath(const char* path)
{
    GPE::SavedScene::CreateArg savedScene = GPE::readPrefabFile(path);

    if (savedScene.type == GPE::SavedScene::EType::XML)
    {
        m_pPrefabBluePrint = std::make_unique<rapidxml::xml_document<>>();
        GPE::SavedScene::toDoc(*m_pPrefabBluePrint, m_pData, savedScene.data.c_str(), savedScene.data.size());
    }

    std::filesystem::path fsPath = path;
    m_name                       = fsPath.stem().string();
}

const char* Prefab::getName() const
{
    return m_name.c_str();
}

GameObject* Prefab::clone(GameObject& parent)
{
    if (m_pPrefabBluePrint == nullptr)
        return nullptr;

    Scene     prefabScene;
    XmlLoader prefab(*m_pPrefabBluePrint);

    // Load each element
    prefabScene.load(prefab);

    // Tell that pointers to the old scene should be replaced by pointers to the new scene
    prefab.addConvertedPtr(prefabScene.getWorld().pOwnerScene, &prefabScene);

    // Update old pointers into new ones
    prefab.updateLazyPtrs();

    if (prefabScene.getWorld().children.empty())
        return nullptr;

    GameObject* const pGo = prefabScene.getWorld().children.front();

    prefab.updateGameObjectLinker(*pGo);
    pGo->setParent(&parent);
    pGo->getTransform().setDirty();

    // Awake GameObjects
    struct Rec
    {
        static void rec(GPE::GameObject& g)
        {
            g.getTransform().onPostLoad();

            for (auto&& it = g.getComponents().begin(); it != g.getComponents().end(); ++it)
            {
                (*it)->onPostLoad();
            }

            for (GPE::GameObject* g2 : g.children)
            {
                rec(*g2);
            }
        };
    };
    Rec::rec(*pGo); // can't do recursives with lambdas, and std::function would be overkill

    return pGo;
}