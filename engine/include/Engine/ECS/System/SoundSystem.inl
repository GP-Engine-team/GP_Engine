int SoundSystem::addComponent(AudioComponent* input) noexcept
{
    key++;
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
