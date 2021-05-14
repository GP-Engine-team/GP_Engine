#include "Engine/ECS/System/SoundSystem.hpp"

using namespace GPE;

SoundSystem::SoundSystem()
{
    // Check if OpenAL Soft handle m_enumeration
    {
        m_enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
        if (m_enumeration == AL_FALSE)
        {
            FUNCT_ERROR("Enumeration not supported !");
        }
    }

    // Gen device
    {
        m_device = alcOpenDevice(NULL);
        if (!m_device)
        {
            FUNCT_ERROR("No Device set !");
        }
    }

    // Gen Context
    if (!ALC_CALL(alcCreateContext, m_openALContext, m_device, m_device, nullptr) || !m_openALContext)
    {
        FUNCT_ERROR("ERROR: Could not create audio context");
    }

    if (!ALC_CALL(alcMakeContextCurrent, m_contextMadeCurrent, m_device, m_openALContext) ||
        m_contextMadeCurrent != ALC_TRUE)
    {
        FUNCT_ERROR("ERROR: Could not create audio context");
    }
}

SoundSystem::~SoundSystem()
{
    ALC_CALL(alcMakeContextCurrent, m_contextMadeCurrent, m_device, nullptr);
    ALC_CALL(alcDestroyContext, m_device, m_openALContext);
    ALC_CALL(alcCloseDevice, m_closed, m_device, m_device);
}