#include "Engine/Intermediate/BehaviourComponent.hpp"

BehaviourComponent::BehaviourComponent(GameObject& owner) noexcept : Component(owner)
{
    BehaviourSystem::getInstance()->addBehaviour(this);
}

BehaviourComponent::~BehaviourComponent() noexcept
{
    BehaviourSystem::getInstance()->removeBehaviour(this);

    if (m_isFixedUpdated)
        BehaviourSystem::getInstance()->removeFixedUpdate(this);

    if (m_isUpdated)
        BehaviourSystem::getInstance()->removeUpdate(this);
}

BehaviourComponent::BehaviourComponent(BehaviourComponent&& other) noexcept
    :   Component(other.m_gameObject), 
        m_isUpdated(std::move(other.m_isUpdated)),
        m_isFixedUpdated(std::move(other.m_isFixedUpdated))
{
    BehaviourSystem::getInstance()->updateBehaviourPointer(this, &other);
}

void BehaviourComponent::enableUpdate(bool flag) noexcept
{
    if (m_isUpdated = flag)
        BehaviourSystem::getInstance()->addUpdate(this);
    else
        BehaviourSystem::getInstance()->removeUpdate(this);
}

void BehaviourComponent::enableFixedUpdate(bool flag) noexcept
{
    if (m_isFixedUpdated = flag)
        BehaviourSystem::getInstance()->addFixedUpdate(this);
    else
        BehaviourSystem::getInstance()->removeFixedUpdate(this);
}