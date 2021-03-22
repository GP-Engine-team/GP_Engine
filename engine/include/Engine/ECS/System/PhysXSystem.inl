#include <Engine/ECS/System/PhysXSystem.hpp>

size_t PhysXSystem::addComponent(CollisionComponent* colComp) noexcept
{
    m_CollisionComponents.push_back(colComp);

    return m_CollisionComponents.size();
}

void PhysXSystem::removeComponent(CollisionComponent* colComp) noexcept
{
    for (std::vector<CollisionComponent*>::iterator it = m_CollisionComponents.begin();
         it != m_CollisionComponents.end(); it++)
    {
        if ((*it) == colComp)
        {
            std::swap<CollisionComponent*>(m_CollisionComponents.back(), (*it));
            m_CollisionComponents.pop_back();
            return;
        }
    }
}
