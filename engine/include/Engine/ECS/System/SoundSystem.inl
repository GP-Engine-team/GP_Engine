#include "Engine/ECS/System/SoundSystem.hpp"

int SoundSystem::addComponent(AudioComponent* input) noexcept
{
    int key = static_cast<int>(m_audioComponent.size());
    m_audioComponent.emplace(key, input);

    return key;
}

void SoundSystem::removeComponent(int key) noexcept
{
    m_audioComponent.erase(key);
}
