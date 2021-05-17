#include "Engine/ECS/Component/BehaviourComponent.hpp"

File_GENERATED

#include "Engine/ECS/System/BehaviourSystem.hpp"
#include "Engine/Engine.hpp"
#include "imgui.h"

using namespace GPE;

BehaviourComponent::BehaviourComponent(GameObject& owner) noexcept : Component(owner)
{
    Engine::getInstance()->behaviourSystem.addBehaviour(*this);
}

BehaviourComponent::BehaviourComponent() noexcept
{
    Engine::getInstance()->behaviourSystem.addBehaviour(*this);
}

void BehaviourComponent::awake()
{

}

BehaviourComponent::~BehaviourComponent() noexcept
{
    Engine::getInstance()->behaviourSystem.removeBehaviour(*this);

    if (m_useFixedUpdate)
        Engine::getInstance()->behaviourSystem.removeFixedUpdate(*this);

    if (m_useUpdate)
        Engine::getInstance()->behaviourSystem.removeUpdate(*this);

    if (m_useOnGUI)
        Engine::getInstance()->behaviourSystem.removeOnGUI(*this);
}

BehaviourComponent::BehaviourComponent(BehaviourComponent&& other) noexcept
    : Component(*other.m_gameObject), m_useUpdate(std::move(other.m_useUpdate)),
      m_useFixedUpdate(std::move(other.m_useFixedUpdate)), m_useOnGUI(std::move(other.m_useOnGUI))
{
    Engine::getInstance()->behaviourSystem.updateBehaviourPointer(this, &other);
}

BehaviourComponent& BehaviourComponent::operator=(BehaviourComponent&& other) noexcept
{
    m_useUpdate      = std::move(other.m_useUpdate);
    m_useFixedUpdate = std::move(other.m_useFixedUpdate);
    m_useOnGUI       = std::move(other.m_useOnGUI);

    Engine::getInstance()->behaviourSystem.updateBehaviourPointer(this, &other);

    return static_cast<BehaviourComponent&>(Component::operator=(std::move(other)));
}

void BehaviourComponent::enableUpdate(bool flag) noexcept
{
    if (m_useUpdate == flag)
        return;

    if (m_useUpdate = flag)
        Engine::getInstance()->behaviourSystem.addUpdate(*this);
    else
        Engine::getInstance()->behaviourSystem.removeUpdate(*this);
}

void BehaviourComponent::enableFixedUpdate(bool flag) noexcept
{
    if (m_useFixedUpdate == flag)
        return;

    if (m_useFixedUpdate = flag)
        Engine::getInstance()->behaviourSystem.addFixedUpdate(*this);
    else
        Engine::getInstance()->behaviourSystem.removeFixedUpdate(*this);
}

void BehaviourComponent::enableOnGUI(bool flag) noexcept
{
    if (m_useOnGUI == flag)
        return;

    if (m_useOnGUI = flag)
        Engine::getInstance()->behaviourSystem.addOnGUI(*this);
    else
        Engine::getInstance()->behaviourSystem.removeOnGUI(*this);
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
        Engine::getInstance()->behaviourSystem.addBehaviour(*this);
    else
        Engine::getInstance()->behaviourSystem.removeBehaviour(*this);
}