#include "Engine/Resources/SceneManager.hpp"

#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Resources/Importer/Importer.hpp>

using namespace GPE;

Scene& SceneManager::loadFirstScene()
{
    if (std::filesystem::exists(firstLoadedScene))
    {
        std::string name  = firstLoadedScene.stem().string();
        Scene&      scene = m_scenes[name];
        scene.m_name      = name;
        m_pCurrentScene   = &scene;

        loadSceneFromPathImp(&scene, firstLoadedScene.string().c_str());
        Log::getInstance()->log("First scene load with path" + firstLoadedScene.string());
        return scene;
    }
    else
    {
        Log::getInstance()->logWarning("First scene not set. Use default scene instead.");
        return setCurrentScene("Default");
    }
}

Scene& SceneManager::setCurrentScene(const std::string& sceneName)
{
    Log::getInstance()->log("New empty scene " + sceneName + " created");

    if (m_pCurrentScene)
        m_scenes.erase(m_pCurrentScene->getName());

    Scene& scene = m_scenes[sceneName];
    scene.m_name = sceneName;

    m_pCurrentScene = &scene;

    if (OnSceneChange)
        OnSceneChange();

    return scene; // emplace with default constructor of Scene
}

//
// Scene& SceneManager::loadScene(const std::string& sceneName, ESceneGraphManagement sceneGraphloadType,
//                               EResourceManagement resourcesloadType)
//{
//    if (!m_pCurrentScene)
//    {
//        m_pCurrentScene = &m_scenes[sceneName];
//        Log::getInstance()->log("New empty scene " + sceneName + " created and loaded");
//        return *m_pCurrentScene;
//    }
//
//    // TODO: To remove
//    m_scenes.erase(m_pCurrentScene->getName());
//    m_pCurrentScene = &m_scenes[sceneName];
//    Log::getInstance()->log("Scene \"" + sceneName + "\" loaded");
//
//    return *m_pCurrentScene;
//
//    /*
//    switch (sceneGraphloadType)
//    {
//    case ESceneGraphManagement::REPLACE:
//    {
//        std::unique_ptr<Scene> newScene = std::make_unique<Scene>();
//
//        switch (resourcesloadType)
//        {
//        case EResourceManagement::RECYCLING:
//
//            break;
//
//        case EResourceManagement::KEEP_IN_MEMORY:
//
//            break;
//
//        case EResourceManagement::BYPASS_RECYLCING:
//
//            break;
//
//        default:
//            break;
//        }
//        break;
//    }
//
//    case ESceneGraphManagement::MERGE:
//
//        break;
//
//    default:
//        break;
//    }
//    */
//}

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