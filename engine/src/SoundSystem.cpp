#include "Engine/ECS/System/SoundSystem.hpp"

using namespace GPE;

SoundSystem* SoundSystem::m_soundSystem = nullptr;

SoundSystem* SoundSystem::getInstance()
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if (m_soundSystem == nullptr)
    {
        m_soundSystem = new SoundSystem();
    }
    return m_soundSystem;
}
