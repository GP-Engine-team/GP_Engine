#include "Engine/Resources/SceneManager.hpp"

#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Resources/Importer/Importer.hpp>

using namespace GPE;

void SceneManager::removeCurrentScene()
{
    for (auto&& it = m_scenes.begin(); it != m_scenes.end(); ++it)
    {
        if (&it->second == m_pCurrentScene)
        {
            m_scenes.erase(it);
            return;
        }
    }
}

Scene& SceneManager::loadFirstScene()
{
    if (m_pCurrentScene)
        removeCurrentScene();

    if (std::filesystem::exists(firstLoadedScene))
    {
        Scene& scene = m_scenes[firstLoadedScene.string()];
        scene.setName(firstLoadedScene.stem().string().c_str());
        m_pCurrentScene = &scene;

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

void SceneManager::defferedReloadCurrentScene()
{
    for (auto&& [key, value] : m_scenes)
    {
        if (&value == m_pCurrentScene)
        {
            m_pathNextSceneToLoad = key;
            return;
        }
    }
}

void SceneManager::defferedLoadNewScene(const char* path)
{
    m_pathNextSceneToLoad = path;
}

Scene& SceneManager::setCurrentScene(const std::string& sceneName)
{
    Log::getInstance()->log("New empty scene " + sceneName + " created");

    if (m_pCurrentScene)
        removeCurrentScene();

    Scene& scene = m_scenes[sceneName];
    scene.setName(std::filesystem::path(sceneName).stem().string().c_str());

    m_pCurrentScene = &scene;

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
    removeScene(scene.getName());
}

void SceneManager::removeScenes()
{
    m_scenes.clear();
    m_pCurrentScene = nullptr;
}

void SceneManager::update()
{
    if (!m_pathNextSceneToLoad.empty())
    {
        loadSceneFromPath(m_pathNextSceneToLoad);
        m_pathNextSceneToLoad.clear();
    }
}

void SceneManager::saveSceneToPath(const std::filesystem::path& path, GPE::SavedScene::EType saveMode)
{
    saveSceneToPathImp(m_pCurrentScene, path.string().c_str(), saveMode);
}

void SceneManager::loadSceneFromPath(const std::filesystem::path& path)
{
    if (std::filesystem::exists(path))
    {
        loadSceneFromPathImp(&setCurrentScene(path.string()), path.string().c_str());

        if (OnSceneChange)
            OnSceneChange();
    }
    else
    {
        Log::getInstance()->logWarning(
            stringFormat("Scene with path %s don't exist and don't loaded", path.string().c_str()));
    }
}