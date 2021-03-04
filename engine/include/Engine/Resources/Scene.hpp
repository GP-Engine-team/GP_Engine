/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"

#include <sstream> //std::getLine
#include <string>  // std::string

namespace GPE
{
class Scene
{
    friend class SceneManager;

private:
protected:
    GameObject m_world;

    constexpr void loadResource(ResourceManagerType& resourceManager) noexcept {};

public:
    constexpr inline Scene(ResourceManagerType& resourceManager) noexcept : m_world()
    {
        m_world.setName("World");
        loadResource(resourceManager);
    }

    // TODO: Can scene be created by default ?
    inline ~Scene() noexcept = delete;

    // TODO: Can scene be copied ? How to manage resource
    constexpr inline Scene(const Scene& other) noexcept = delete;

    // TODO: Can scene be moved ? How to manage resource
    constexpr inline Scene(Scene&& other) noexcept = delete;

    // TODO: Can scene be copied ? How to manage resource
    constexpr inline Scene& operator=(Scene const& other) noexcept = delete;

    // TODO: Can scene be moved ? How to manage resource
    constexpr inline Scene& operator=(Scene&& other) noexcept = delete;

    /**
     * @brief Get the World object
     *
     * @return GraphEntity*
     */
    GameObject& getWorld() noexcept
    {
        return m_world;
    }

    const GameObject& getWorld() const noexcept
    {
        return m_world;
    }

    /**
     * @brief Get the Entity object in function of path in arg
     *
     * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3
     * @return GraphEntity&
     */
    GameObject* getGameObject(const std::string& path) noexcept
    {
        GPE_ASSERT(!path.empty());

        std::stringstream sPath(path);
        std::string       word;
        GameObject*       currentEntity = &m_world;

        while (std::getline(sPath, word, '/'))
        {
            if (word.empty() || word == "." || word == "world")
                continue;

            bool isFound = false;
            for (auto&& child : currentEntity->children)
            {
                if (child->getName() == word)
                {
                    currentEntity = child.get();
                    isFound       = true;
                    break;
                }
            }

            if (!isFound)
            {
                Log::logError(std::string("Canno't found \"") + word + "\" in scene graph \"" + path + "\"");
                return nullptr;
            }
        }
        return currentEntity;
    }
};
} /*namespace GPE*/
