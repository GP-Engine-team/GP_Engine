#include "Engine/ECS/System/SoundSystem.hpp"

int SoundSystem::addComponent(AudioComponent* input) noexcept
{
    int key = static_cast<int>(m_audioComponents.size());
    m_audioComponents.emplace(key, input);

    return key;
}

void SoundSystem::removeComponent(int key) noexcept
{
    m_audioComponents.erase(key);
}
