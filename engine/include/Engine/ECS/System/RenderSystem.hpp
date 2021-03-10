/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <functional> //std::function
#include <vector>     //std::vector

#include "Engine/ECS/System/SceneRenderSystem.hpp"
#include "GPM/Transform.hpp"

namespace GPE
{

class RenderSystem
{
public:
    using RenderPipeline =
        std::function<void(RenderSystem&, std::vector<Renderer*>&, std::vector<SceneRenderSystem*>&)>;

protected:
    std::vector<Renderer*>          m_pRenderers;
    std::vector<SceneRenderSystem*> m_pSceneRenderSystems;

public:
    RenderSystem() noexcept = default;

    RenderPipeline defaultRenderPipeline() const noexcept;
    void           draw(RenderPipeline renderPipeline) noexcept;

public:
    // TODO: Remove this shit and create variadic templated system
    void addRenderer(Renderer* pRenderer) noexcept;

    void updateRendererPointer(Renderer* newPointerRenderer, Renderer* exPointerRenderer) noexcept;

    void removeRenderer(Renderer* pRenderer) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addSceneRenderSystem(SceneRenderSystem* pSceneRenderSystem) noexcept;

    void updateSceneRenderSystemPointer(SceneRenderSystem* newPointerSceneRenderSystem,
                                        SceneRenderSystem* exPointerSceneRenderSystem) noexcept;

    void removeSceneRenderSystem(SceneRenderSystem* pSubModel) noexcept;
};

} /*namespace GPE*/
