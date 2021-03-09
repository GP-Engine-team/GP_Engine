#include "Engine/ECS/System/RenderSystem.hpp"

#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Core/System/SystemsManager.hpp"

using namespace GPE;
using namespace GPM;

RenderSystem::RenderPipeline RenderSystem::defaultRenderPipeline() const noexcept
{
    return
        [](RenderSystem& rs, std::vector<Renderer*>& pRenderers, std::vector<SceneRenderSystem*>& pSceneRenderSystems)

    {
        int h, w;
        pRenderers[0]->getWindow()->getSize(w, h);

        pSceneRenderSystems[0]->draw(SystemsManager::getInstance()->resourceManager,
                                     pSceneRenderSystems[0]->defaultRenderPipeline(), 0);
    };
}

void RenderSystem::draw(RenderPipeline renderPipeline) noexcept
{
    renderPipeline(*this, m_pRenderers, m_pSceneRenderSystems);
}

void RenderSystem::addRenderer(Renderer* pRenderer) noexcept
{
    m_pRenderers.push_back(pRenderer);
}

void RenderSystem::updateRendererPointer(Renderer* newPointerRenderer, Renderer* exPointerRenderer) noexcept
{
    for (std::vector<Renderer*>::iterator it = m_pRenderers.begin(); it != m_pRenderers.end(); it++)
    {
        if ((*it) == exPointerRenderer)
        {
            *it = newPointerRenderer;
            return;
        }
    }
}

void RenderSystem::removeRenderer(Renderer* pRenderer) noexcept
{
    for (std::vector<Renderer*>::iterator it = m_pRenderers.begin(); it != m_pRenderers.end(); it++)
    {
        if ((*it) == pRenderer)
        {
            std::swap<Renderer*>(m_pRenderers.back(), (*it));
            m_pRenderers.pop_back();
            return;
        }
    }
}

void RenderSystem::addSceneRenderSystem(SceneRenderSystem* pSceneRenderSystem) noexcept
{
    m_pSceneRenderSystems.push_back(pSceneRenderSystem);
}

void RenderSystem::updateSceneRenderSystemPointer(SceneRenderSystem* newPointerSceneRenderSystem,
                                                  SceneRenderSystem* exPointerSceneRenderSystem) noexcept
{
    for (std::vector<SceneRenderSystem*>::iterator it = m_pSceneRenderSystems.begin();
         it != m_pSceneRenderSystems.end(); it++)
    {
        if ((*it) == exPointerSceneRenderSystem)
        {
            *it = newPointerSceneRenderSystem;
            return;
        }
    }
}

void RenderSystem::removeSceneRenderSystem(SceneRenderSystem* pSceneRenderSystem) noexcept
{
    for (std::vector<SceneRenderSystem*>::iterator it = m_pSceneRenderSystems.begin();
         it != m_pSceneRenderSystems.end(); it++)
    {
        if ((*it) == pSceneRenderSystem)
        {
            std::swap<SceneRenderSystem*>(m_pSceneRenderSystems.back(), (*it));
            m_pSceneRenderSystems.pop_back();
            return;
        }
    }
}
