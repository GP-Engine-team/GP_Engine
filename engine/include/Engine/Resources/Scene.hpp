/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/System/SceneRenderSystem.hpp"
#include "Engine/Intermediate/GameObject.hpp"

#include <string> // std::string

namespace GPE
{
class Scene
{
    friend class SceneManager;

protected:
    GameObject* m_pWorld = nullptr;

    std::unordered_map<std::string, unsigned int>
        m_loadedResourcesPath; // Indicate witch resource is loaded with counter

public:
    SceneRenderSystem sceneRenderer;

public:
    Scene() noexcept;
    ~Scene() noexcept;

    // TODO: Can scene be copied ? How to manage resource
    constexpr inline Scene(const Scene& other) noexcept = delete;

    // TODO: Can scene be moved ? How to manage resource
    constexpr inline Scene(Scene&& other) noexcept = delete;

    // TODO: Can scene be copied ? How to manage resource
    constexpr inline Scene& operator=(Scene const& other) noexcept = delete;

    // TODO: Can scene be moved ? How to manage resource
    constexpr inline Scene& operator=(Scene&& other) noexcept = delete;

    /**
     * @brief Get the Entity object in function of path in arg
     *
     * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3
     * @return GraphEntity&
     */
    GameObject* getGameObject(const std::string& path) noexcept;

    GameObject& getWorld() noexcept;

    void addLoadedResourcePath(const char* path) noexcept;
    void removeLoadedResourcePath(const char* path) noexcept;
};
} /*namespace GPE*/
