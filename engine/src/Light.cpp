#include "Engine/ECS/Component/Light/Light.hpp"
#include <Engine/ECS/System/RenderSystem.hpp>

using namespace GPE;

void Light::onPostLoad()
{
    getOwner().pOwnerScene->sceneRenderer.addLight(*this);
}

void Light::setActive(bool newState) noexcept
{
    if (m_isActivated == newState)
        return;

    m_isActivated = newState;
    if (m_isActivated)
        getOwner().pOwnerScene->sceneRenderer.addLight(*this);
    else
        getOwner().pOwnerScene->sceneRenderer.removeLight(*this);
}

File_GENERATED
