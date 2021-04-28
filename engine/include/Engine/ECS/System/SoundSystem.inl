﻿int SoundSystem::addComponent(AudioComponent* input) noexcept
{
    int key = static_cast<int>(m_audioComponents.size());
    m_audioComponents.emplace(key, input);

    return key;
}

void SoundSystem::updateComponent(AudioComponent* newPointorBehaviour) noexcept
{
    m_audioComponents[newPointorBehaviour->getKey()] = newPointorBehaviour;
}

void SoundSystem::removeComponent(int key) noexcept
{
    m_audioComponents.erase(key);
}
