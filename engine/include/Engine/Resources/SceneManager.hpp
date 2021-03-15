/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>
#include <unordered_map>

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

public:
    SceneManager() noexcept = default;

    SceneManager(const SceneManager& other) noexcept = default;

    SceneManager(SceneManager&& other) noexcept = default;

    ~SceneManager() noexcept = default;

    SceneManager& operator=(SceneManager const& other) noexcept = default;

    SceneManager& operator=(SceneManager&& other) noexcept = default;

    Scene* getCurrentScene() noexcept
    {
        return m_pCurrentScene;
    }

    void addEmpty(const std::string& sceneName)
    {
        static_cast<void>(m_scenes[sceneName]); // emaplce with default constructor of scene
    }

    void loadScene(const std::string&    sceneName,
                   ESceneGraphManagement sceneGraphloadType = ESceneGraphManagement::REPLACE,
                   EResourceManagement   resourcesloadType  = EResourceManagement::RECYCLING)
    {
        if (!m_pCurrentScene)
        {
            m_pCurrentScene = &m_scenes[sceneName];
            return;
        }

        // TODO: To remove
        m_pCurrentScene = &m_scenes[sceneName];
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
};

} /*namespace GPE*/
