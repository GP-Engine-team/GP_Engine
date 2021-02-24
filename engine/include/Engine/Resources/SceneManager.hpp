/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <memory>
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
    void loadScene(const std::string& path);

    std::unordered_map<std::string, std::string> m_scenesPath        = {}; // id / path
    std::unique_ptr<Scene>                       m_pCurrentSceneLoad = nullptr;

public:
    SceneManager() noexcept = default;

    SceneManager(const SceneManager& other) noexcept = default;

    SceneManager(SceneManager&& other) noexcept = default;

    ~SceneManager() noexcept = default;

    SceneManager& operator=(SceneManager const& other) noexcept = default;

    SceneManager& operator=(SceneManager&& other) noexcept = default;

    void loadNewScene(const std::string&    sceneID,
                      ESceneGraphManagement sceneGraphloadType = ESceneGraphManagement::REPLACE,
                      EResourceManagement   resourcesloadType  = EResourceManagement::RECYCLING)
    {
        if (!m_pCurrentSceneLoad)
        {
            loadScene(m_scenesPath[sceneID]); // Initialize m_pCurrentSceneLoad
            return;
        }

        switch (sceneGraphloadType)
        {
        case ESceneGraphManagement::REPLACE: {
            std::unique_ptr<Scene> newScene;

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
            m_pCurrentSceneLoad = std::move(newScene);
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
