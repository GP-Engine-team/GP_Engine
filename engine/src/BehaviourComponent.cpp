#include "Engine/ECS/Component/BehaviourComponent.hpp"

File_GENERATED

#include "Engine/ECS/System/BehaviourSystem.hpp"
#include "imgui.h"
#include <Engine/ECS/System/RenderSystem.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Scene.hpp>

    using namespace GPE;

BehaviourComponent::BehaviourComponent(GameObject& owner) noexcept : Component(owner)
{
    getOwner().pOwnerScene->behaviourSystem.addBehaviour(*this);
}

void BehaviourComponent::onPostLoad()
{
    if (m_isActivated)
    {
        getOwner().pOwnerScene->behaviourSystem.addBehaviour(*this);
    }
}

BehaviourComponent::~BehaviourComponent() noexcept
{
    if (!m_isActivated || !getOwner().pOwnerScene)
        return;

    getOwner().pOwnerScene->behaviourSystem.removeBehaviour(*this);

    if (m_useFixedUpdate)
        getOwner().pOwnerScene->behaviourSystem.removeFixedUpdate(*this);

    if (m_useUpdate)
        getOwner().pOwnerScene->behaviourSystem.removeUpdate(*this);

    if (m_useOnGUI)
        getOwner().pOwnerScene->behaviourSystem.removeOnGUI(*this);
}

BehaviourComponent::BehaviourComponent(BehaviourComponent&& other) noexcept
    : Component(*other.m_gameObject), m_useUpdate(std::move(other.m_useUpdate)),
      m_useFixedUpdate(std::move(other.m_useFixedUpdate)), m_useOnGUI(std::move(other.m_useOnGUI))
{
    getOwner().pOwnerScene->behaviourSystem.updateBehaviourPointer(this, &other);
}

BehaviourComponent& BehaviourComponent::operator=(BehaviourComponent&& other) noexcept
{
    m_useUpdate      = std::move(other.m_useUpdate);
    m_useFixedUpdate = std::move(other.m_useFixedUpdate);
    m_useOnGUI       = std::move(other.m_useOnGUI);

    getOwner().pOwnerScene->behaviourSystem.updateBehaviourPointer(this, &other);

    return static_cast<BehaviourComponent&>(Component::operator=(std::move(other)));
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

void BehaviourComponent::setActive(bool newState) noexcept
{
    if (m_isActivated == newState)
        return;

    m_isActivated = newState;
    if (m_isActivated)
        getOwner().pOwnerScene->behaviourSystem.addBehaviour(*this);
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