#include "Engine/ECS/System/AnimationSystem.hpp"

#include <algorithm>


namespace GPE
{

void AnimationSystem::update(double deltaTime) const noexcept
{

}

void AnimationSystem::addBehaviour(AnimationComponent& behaviour) noexcept
{
    m_pAnimComponents.emplace_back(&behaviour);
}

void AnimationSystem::removeBehaviour(AnimationComponent& behaviour) noexcept
{
    m_pAnimComponents.erase(std::remove(m_pAnimComponents.begin(), m_pAnimComponents.end(), &behaviour), m_pAnimComponents.end());
}

} // namespace GPE