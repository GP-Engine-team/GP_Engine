#include "Engine/ECS/Component/BehaviourComponent.hpp"

File_GENERATED

#include "imgui.h"
#include <Engine/ECS/Component/TransformComponent.hpp>
#include <Engine/ECS/System/BehaviourSystem.hpp>
#include <Engine/ECS/System/RenderSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Scene.hpp>

    using namespace GPE;

BehaviourComponent::BehaviourComponent(GameObject& owner) noexcept : Component(owner)
{
    updateToSystem();
}

BehaviourComponent::~BehaviourComponent() noexcept
{
    setActive(false);
}

void BehaviourComponent::enableUpdate(bool flag) noexcept
{
    if (m_useUpdate == flag)
        return;

    if (m_useUpdate = flag)
        getOwner().pOwnerScene->behaviourSystem.addUpdate(*this);
    else
        getOwner().pOwnerScene->behaviourSystem.removeUpdate(*this);
}

void BehaviourComponent::enableFixedUpdate(bool flag) noexcept
{
    if (m_useFixedUpdate == flag)
        return;

    if (m_useFixedUpdate = flag)
        getOwner().pOwnerScene->behaviourSystem.addFixedUpdate(*this);
    else
        getOwner().pOwnerScene->behaviourSystem.removeFixedUpdate(*this);
}

void BehaviourComponent::enableOnGUI(bool flag) noexcept
{
    if (m_useOnGUI == flag)
        return;

    if (m_useOnGUI = flag)
        getOwner().pOwnerScene->behaviourSystem.addOnGUI(*this);
    else
        getOwner().pOwnerScene->behaviourSystem.removeOnGUI(*this);
}

void BehaviourComponent::enableUpdateEditor(bool flag) noexcept
{
    if (m_useUpdateEditor == flag)
        return;

    if (m_useUpdateEditor = flag)
        getOwner().pOwnerScene->behaviourSystem.addUpdateEditor(*this);
    else
        getOwner().pOwnerScene->behaviourSystem.removeUpdateEditor(*this);
}

bool BehaviourComponent::isUpdateEnable() const noexcept
{
    return m_useUpdate;
}

bool BehaviourComponent::isFixedUpdateEnable() const noexcept
{
    return m_useFixedUpdate;
}

bool BehaviourComponent::isOnGUIEnable() const noexcept
{
    return m_useOnGUI;
}

bool BehaviourComponent::isUpdateEditorEnable() const noexcept
{
    return m_useUpdateEditor;
}

void BehaviourComponent::updateToSystem() noexcept
{
    if (!getOwner().pOwnerScene)
        return;

    if (m_isActivated)
    {
        if (m_useUpdate)
            getOwner().pOwnerScene->behaviourSystem.addUpdate(*this);
        else
            getOwner().pOwnerScene->behaviourSystem.removeUpdate(*this);

        if (m_useFixedUpdate)
            getOwner().pOwnerScene->behaviourSystem.addFixedUpdate(*this);
        else
            getOwner().pOwnerScene->behaviourSystem.removeFixedUpdate(*this);

        if (m_useOnGUI)
            getOwner().pOwnerScene->behaviourSystem.addOnGUI(*this);
        else
            getOwner().pOwnerScene->behaviourSystem.removeOnGUI(*this);

        if (m_useUpdateEditor)
            getOwner().pOwnerScene->behaviourSystem.addUpdateEditor(*this);
        else
            getOwner().pOwnerScene->behaviourSystem.removeUpdateEditor(*this);

        getOwner().pOwnerScene->behaviourSystem.addBehaviour(*this);
    }
    else
        getOwner().pOwnerScene->behaviourSystem.removeBehaviour(*this);
}

void BehaviourComponent::gameAssert(bool condition, const char* msg)
{
    getOwner().pOwnerScene->behaviourSystem.gameAssert(condition, msg);
}

void BehaviourComponent::drawDebugSphere(const GPM::Vec3& position, float radius, const ColorRGBA& color,
                                         float duration, bool enableBackFaceCullling) noexcept
{
    getOwner().pOwnerScene->sceneRenderer.drawDebugSphere(position, radius, color, duration,
                                                          RenderSystem::EDebugShapeMode::FILL, enableBackFaceCullling);
}

void BehaviourComponent::drawDebugCube(const GPM::Vec3& position, const GPM::Quat& rotation, const GPM::Vec3& scale,
                                       const ColorRGBA& color, float duration, bool enableBackFaceCullling) noexcept
{
    getOwner().pOwnerScene->sceneRenderer.drawDebugCube(position, rotation, scale, color, duration,
                                                        RenderSystem::EDebugShapeMode::FILL, enableBackFaceCullling);
}

void BehaviourComponent::drawDebugQuad(const GPM::Vec3& position, const GPM::Vec3& dir, const GPM::Vec3& scale,
                                       const ColorRGBA& color, float duration, bool enableBackFaceCullling) noexcept
{
    getOwner().pOwnerScene->sceneRenderer.drawDebugQuad(position, dir, scale, color, duration,
                                                        RenderSystem::EDebugShapeMode::FILL, enableBackFaceCullling);
}

void BehaviourComponent::drawDebugLine(const GPM::Vec3& pt1, const GPM::Vec3& pt2, float width,
                                       const ColorRGBA& color) noexcept
{
    getOwner().pOwnerScene->sceneRenderer.drawDebugLine(pt1, pt2, width, color);
}

void BehaviourComponent::log(const std::string& msg)
{
    Log::getInstance()->log(msg);
}

void BehaviourComponent::logWarning(const std::string& msg)
{
    Log::getInstance()->logWarning(msg);
}

void BehaviourComponent::logError(const std::string& msg)
{
    Log::getInstance()->logError(msg);
}

TransformComponent& BehaviourComponent::transform()
{
    return getOwner().getTransform();
}

void BehaviourComponent::reloadScene()
{
    Engine::getInstance()->sceneManager.defferedReloadCurrentScene();
}

void BehaviourComponent::loadNewScene(const char* path)
{
    Engine::getInstance()->sceneManager.defferedLoadNewScene(path);
}

void BehaviourComponent::closeApplication()
{
    Engine::getInstance()->isRunning = false;
}