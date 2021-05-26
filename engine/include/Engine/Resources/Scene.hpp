/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/ECS/System/BehaviourSystem.hpp>
#include <Engine/ECS/System/RenderSystem.hpp>
#include <Engine/Serialization/xml/xmlLoader.hpp>
#include <map>
#include <memory>
#include <string> // std::string

namespace GPE
{
class GameObject;
class Scene;

template <>
void load(XmlLoader& context, GPE::Scene*& inspected, const XmlLoader::LoadInfo& info);

class Scene
{
    friend class SceneManager;

protected:
    std::unique_ptr<GameObject> m_pWorld = nullptr;

    std::unordered_map<std::string, unsigned int>
        m_loadedResourcesPath; // Indicate witch resource is loaded with counter

public:
    RenderSystem    sceneRenderer;
    BehaviourSystem behaviourSystem;

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

    GameObject& getWorld() noexcept;

    void addLoadedResourcePath(const char* path) noexcept;
    void removeLoadedResourcePath(const char* path) noexcept;

    void save(XmlSaver&) const;
    void load(XmlLoader&);

    const std::string& getName() const;
    void               setName(const char* newName);
};
} /*namespace GPE*/
