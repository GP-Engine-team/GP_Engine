#include <Engine/ECS/System/PhysXSystem.hpp>

int PhysXSystem::addComponent(InputComponent* input) noexcept
{
    int key = static_cast<int>(m_inputComponents.size());
    m_inputComponents.emplace(key, input);

    return key;
}

void PhysXSystem::removeComponent(int key) noexcept
{
    m_inputComponents.erase(key);
}
