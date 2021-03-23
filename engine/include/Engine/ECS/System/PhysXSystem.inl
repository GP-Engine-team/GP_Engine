#include <Engine/ECS/System/PhysXSystem.hpp>

size_t PhysXSystem::addComponent(RigidbodyStatic* colComp) noexcept
{
    m_RigidbodyStatics.push_back(colComp);

    return m_RigidbodyStatics.size();
}

void PhysXSystem::removeComponent(RigidbodyStatic* colComp) noexcept
{
    for (std::vector<RigidbodyStatic*>::iterator it = m_RigidbodyStatics.begin(); it != m_RigidbodyStatics.end(); it++)
    {
        if ((*it) == colComp)
        {
            std::swap<RigidbodyStatic*>(m_RigidbodyStatics.back(), (*it));
            m_RigidbodyStatics.pop_back();
            return;
        }
    }
}
