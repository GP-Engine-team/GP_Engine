#include <Engine/ECS/System/PhysXSystem.hpp>

inline size_t GPE::PhysXSystem::addComponent(RigidbodyStatic* rigidbody) noexcept
{
    m_RigidbodyStatics.push_back(rigidbody);

    return m_RigidbodyStatics.size();
}

inline void GPE::PhysXSystem::removeComponent(RigidbodyStatic* rigidbody) noexcept
{
    for (std::vector<RigidbodyStatic*>::iterator it = m_RigidbodyStatics.begin(); it != m_RigidbodyStatics.end(); it++)
    {
        if ((*it) == rigidbody)
        {
            std::swap<RigidbodyStatic*>(m_RigidbodyStatics.back(), (*it));
            m_RigidbodyStatics.pop_back();
            return;
        }
    }
}
