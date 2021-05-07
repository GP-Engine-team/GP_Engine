#include "Engine/Resources/SceneManager.hpp"

#include <Engine/Core/Debug/Log.hpp>

using namespace GPE;

Scene& SceneManager::addEmpty(const std::string& sceneName)
{
    Log::getInstance()->log("New empty scene " + sceneName + " created");

    Scene& scene = m_scenes[sceneName];
    scene.m_name = sceneName;
    return scene; // emplace with default constructor of Scene
}

Scene& SceneManager::loadScene(const std::string& sceneName, ESceneGraphManagement sceneGraphloadType,
                               EResourceManagement resourcesloadType)
{
    if (!m_pCurrentScene)
    {
        m_pCurrentScene = &m_scenes[sceneName];
        Log::getInstance()->log("New empty scene " + sceneName + " created and loaded");
        return *m_pCurrentScene;
    }

    // TODO: To remove
    m_pCurrentScene = &m_scenes[sceneName];
    Log::getInstance()->log("Scene \"" + sceneName + "\" loaded");

    return *m_pCurrentScene;

    /*
    switch (sceneGraphloadType)
    {
    case ESceneGraphManagement::REPLACE:
    {
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
    */
}

void SceneManager::removeScene(const std::string& sceneName)
{
    using const_iterator = std::unordered_map<std::string, Scene>::const_iterator;

    if (const_iterator it{m_scenes.find(sceneName)}; it != m_scenes.end())
    {
        const bool currentSceneSetNull = &it->second == m_pCurrentScene;

        if (currentSceneSetNull)
            m_pCurrentScene = nullptr;

        m_scenes.erase(it);

        if (currentSceneSetNull && m_scenes.size() != 0u)
            m_pCurrentScene = &m_scenes.begin()->second;
    }
}

void SceneManager::removeScene(Scene& scene)
{
    removeScene(scene.m_name);
}

void SceneManager::removeScenes()
{
    m_scenes.clear();
}