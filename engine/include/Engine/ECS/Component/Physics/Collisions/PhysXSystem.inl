#include <Engine/ECS/System/PhysXSystem.hpp>

inline int PhysXSystem::addComponent(CollisionComponent* colComp) noexcept
{
    int key = static_cast<int>(m_CollisionComponents.size());
    m_CollisionComponents.emplace(key, colComp);

    return key;
}

inline void PhysXSystem::removeComponent(int key) noexcept
{
    m_CollisionComponents.erase(key);
}
