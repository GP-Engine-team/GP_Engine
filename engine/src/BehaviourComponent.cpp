#include "Engine/ECS/System/BehaviourSystem.hpp"
#include "imgui.h"
#include "Engine/Engine.hpp"

#include "Engine/ECS/Component/BehaviourComponent.hpp"

using namespace GPE;

BehaviourComponent::BehaviourComponent(GameObject& owner) noexcept : Component(owner)
{
    Engine::getInstance()->behaviourSystem.addBehaviour(this);
}

BehaviourComponent::BehaviourComponent() noexcept
{
    Engine::getInstance()->behaviourSystem.addBehaviour(this);
}

BehaviourComponent::~BehaviourComponent() noexcept
{
    Engine::getInstance()->behaviourSystem.removeBehaviour(this);

    if (m_isFixedUpdated)
        Engine::getInstance()->behaviourSystem.removeFixedUpdate(*this);

    if (m_isUpdated)
        Engine::getInstance()->behaviourSystem.removeUpdate(*this);

    DataChunk<BehaviourComponent>::getInstance()->destroy(this);
}

BehaviourComponent::BehaviourComponent(BehaviourComponent&& other) noexcept
    : Component(*other.m_gameObject), m_isUpdated(std::move(other.m_isUpdated)),
      m_isFixedUpdated(std::move(other.m_isFixedUpdated))
{
    Engine::getInstance()->behaviourSystem.updateBehaviourPointer(this, &other);
}

BehaviourComponent& BehaviourComponent::operator=(BehaviourComponent&& other) noexcept
{
    m_isUpdated      = std::move(other.m_isUpdated);
    m_isFixedUpdated = std::move(other.m_isFixedUpdated);

    Engine::getInstance()->behaviourSystem.updateBehaviourPointer(this, &other);

    return static_cast<BehaviourComponent&>(Component::operator=(std::move(other)));
}

void BehaviourComponent::enableUpdate(bool flag) noexcept
{
    if (m_isUpdated == flag)
        return;

    if (m_isUpdated = flag)
        Engine::getInstance()->behaviourSystem.addUpdate(*this);
    else
        Engine::getInstance()->behaviourSystem.removeUpdate(*this);
}

void BehaviourComponent::enableFixedUpdate(bool flag) noexcept
{
    if (m_isFixedUpdated == flag)
        return;

    if (m_isFixedUpdated = flag)
        Engine::getInstance()->behaviourSystem.addFixedUpdate(*this);
    else
        Engine::getInstance()->behaviourSystem.removeFixedUpdate(*this);
}
