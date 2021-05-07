#include <Engine/Resources/Prefab.hpp>

#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Scene.hpp>

// Generated
#include "Generated/Prefab.rfk.h"
File_GENERATED

    using namespace GPE;

void Prefab::loadPrefabFromPath(const char* path)
{
    Scene                      prefabScene;
    GPE::SavedScene::CreateArg savedScene = GPE::readPrefabFile(path);

    if (savedScene.type == GPE::SavedScene::EType::XML)
    {
        // Load xml doc
        rapidxml::xml_document<> doc;
        std::unique_ptr<char[]>  buffer;
        GPE::SavedScene::toDoc(doc, buffer, savedScene.data.c_str(), savedScene.data.size());

        XmlLoader context(doc);
        // Load each element
        prefabScene.load(context);
    }

    if (!prefabScene.getWorld().children.empty())
    {
        GameObject* const pGo = prefabScene.getWorld().children.front();
        pGo->setParent(nullptr);
        m_pPrefab.reset(pGo);
    }
}

GameObject& Prefab::clone(GameObject& parent)
{
    // Awake GameObjects
    struct Rec
    {
        static void rec(GPE::GameObject& g)
        {
            for (GPE::Component* comp : g.getComponents())
            {
                comp->awake();
            }

            g.getTransform().awake();

            for (GPE::GameObject* g2 : g.children)
            {
                rec(*g2);
            }
        };
    };
    Rec::rec(*m_pPrefab.get()); // can't do recursives with lambdas, and std::function would be overkill

    return parent;
}