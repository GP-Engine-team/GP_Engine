/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>

#include <Engine/Serialization/xml/xmlLoader.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>

#include "Engine/Resources/Scene.hpp"

namespace GPE
{
enum class ESceneGraphManagement
{
    REPLACE, // Change the scene graph
    MERGE,   // Merge both scene graph
};

enum class EResourceManagement
{
    RECYCLING,       // Enable recycling process
    KEEP_IN_MEMORY,  // Keep ALL resources in memory even if they are not used. Usefull to load fastly previous scene
    BYPASS_RECYLCING // Reload ALL resource even if there are used in both scen. Usefull if you know that any resource
                     // must be recycling
};

class SceneManager
{
private:
protected:
    std::unordered_map<std::string, Scene> m_scenes        = {};
    Scene*                                 m_pCurrentScene = nullptr;

    // Path to the next scene to load
    std::filesystem::path m_pathNextSceneToLoad;
    bool                  m_isDead = false; // usefull to signal to change th current scene

public:
    std::function<void(void)> OnSceneChange;

    // This scene will be load in first if possible
    std::filesystem::path firstLoadedScene;

protected:
    void removeCurrentScene();

public:
    SceneManager() noexcept = default;

    SceneManager(const SceneManager& other) noexcept = default;

    SceneManager(SceneManager&& other) noexcept = default;

    ~SceneManager() noexcept = default;

    SceneManager& operator=(SceneManager const& other) noexcept = default;

    SceneManager& operator=(SceneManager&& other) noexcept = default;

    /**
     * @brief Defered action. To load new scene or reload the current
     * @param sceneName
     */
    void defferedReloadCurrentScene();
    void defferedLoadNewScene(const char* path);

    /**
     * @brief Update deffered action
     */
    void update();

    Scene* getCurrentScene() noexcept
    {
        return m_pCurrentScene;
    }

    Scene& setCurrentScene(const std::string& sceneName);

    /**
     * @brief Load the first sc3ene register by the user
     */
    Scene& loadFirstScene();

    // Scene& loadScene(const std::string&    sceneName,
    //                 ESceneGraphManagement sceneGraphloadType = ESceneGraphManagement::REPLACE,
    //                 EResourceManagement   resourcesloadType  = EResourceManagement::RECYCLING);

    void removeScene(const std::string& sceneName);
    void removeScene(Scene& scene);
    void removeScenes();

    void saveSceneToPath(const std::filesystem::path& path, GPE::SavedScene::EType saveMode);
    void loadSceneFromPath(const std::filesystem::path& path);
};

} /*namespace GPE*/
