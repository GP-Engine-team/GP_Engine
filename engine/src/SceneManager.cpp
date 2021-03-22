#include "Engine/Resources/SceneManager.hpp"

using namespace GPE;

void SceneManager::addEmpty(const std::string& sceneName)
{
    m_scenes[sceneName]; // emaplce with default constructor of scene
    Log::getInstance()->log("New empty scene create with name : " + sceneName);
}

void SceneManager::loadScene(const std::string& sceneName, ESceneGraphManagement sceneGraphloadType,
                             EResourceManagement resourcesloadType)
{
    if (!m_pCurrentScene)
    {
        m_pCurrentScene = &m_scenes[sceneName];
        Log::getInstance()->log("New empty scene create and load with name : " + sceneName);
        return;
    }

    // TODO: To remove
    m_pCurrentScene = &m_scenes[sceneName];
    Log::getInstance()->log("Scene \"" + sceneName + "\" load");
    return;

    switch (sceneGraphloadType)
    {
    case ESceneGraphManagement::REPLACE: {
        std::unique_ptr<Scene> newScene = std::make_unique<Scene>();

        switch (resourcesloadType)
        {
        case EResourceManagement::RECYCLING:

            break;

        case EResourceManagement::KEEP_IN_MEMORY:

            break;

        case EResourceManagement::BYPASS_RECYLCING:

            break;

        default:
            break;
        }
        break;
    }

    case ESceneGraphManagement::MERGE:

        break;

    default:
        break;
    }
}