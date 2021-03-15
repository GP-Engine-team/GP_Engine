#include "Engine/ECS/Component/BehaviourComponent.hpp"

#include "Engine/ECS/System/BehaviourSystem.hpp"
#include "Engine/ECS/System/SystemsManager.hpp"

using namespace GPE;

BehaviourComponent::BehaviourComponent(GameObject& owner) noexcept : Component(owner)
{
    SystemsManager::getInstance()->behaviourSystem.addBehaviour(this);
}

BehaviourComponent::~BehaviourComponent() noexcept
{
    SystemsManager::getInstance()->behaviourSystem.removeBehaviour(this);

    if (m_isFixedUpdated)
        SystemsManager::getInstance()->behaviourSystem.removeFixedUpdate(*this);

    if (m_isUpdated)
        SystemsManager::getInstance()->behaviourSystem.removeUpdate(*this);
}

BehaviourComponent::BehaviourComponent(BehaviourComponent&& other) noexcept
    : Component(other.m_gameObject), m_isUpdated(std::move(other.m_isUpdated)),
      m_isFixedUpdated(std::move(other.m_isFixedUpdated))
{
    SystemsManager::getInstance()->behaviourSystem.updateBehaviourPointer(this, &other);
}

void BehaviourComponent::enableUpdate(bool flag) noexcept
{
    if (m_isUpdated == flag)
        return;

    if (m_isUpdated = flag)
        SystemsManager::getInstance()->behaviourSystem.addUpdate(*this);
    else
        SystemsManager::getInstance()->behaviourSystem.removeUpdate(*this);
}

void BehaviourComponent::enableFixedUpdate(bool flag) noexcept
{
    if (m_isFixedUpdated == flag)
        return;

    if (m_isFixedUpdated = flag)
        SystemsManager::getInstance()->behaviourSystem.addFixedUpdate(*this);
    else
        SystemsManager::getInstance()->behaviourSystem.removeFixedUpdate(*this);
}
