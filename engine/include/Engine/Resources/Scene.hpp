/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/ResourcesManagerType.hpp"

namespace GPE
{
class Scene
{
    friend class SceneManager;

private:
protected:
    ResourceManagerType m_resourceManager;

    constexpr void loadResource(ResourceManagerType& resourceManager) noexcept {};

public:
    constexpr inline Scene(ResourceManagerType& resourceManager) noexcept
    {
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
};
} /*namespace GPE*/
