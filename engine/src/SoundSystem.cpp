#include "Engine/ECS/System/SoundSystem.hpp"

using namespace GPE;

SoundSystem::SoundSystem()
{
    // Check if OpenAL Soft handle enumeration
    {
        enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
        if (enumeration == AL_FALSE)
        {
            FUNCT_ERROR("Enumeration not supported !");
        }
    }

    // Gen device
    {
        device = alcOpenDevice(NULL);
        if (!device)
        {
            FUNCT_ERROR("No Device set !");
        }
    }

    // Gen Context
    if (!ALC_CALL(alcCreateContext, openALContext, device, device, nullptr) || !openALContext)
    {
        FUNCT_ERROR("ERROR: Could not create audio context");
    }

    if (!ALC_CALL(alcMakeContextCurrent, contextMadeCurrent, device, openALContext) || contextMadeCurrent != ALC_TRUE)
    {
        FUNCT_ERROR("ERROR: Could not create audio context");
    }

    if (!AL_CALL(alDistanceModel, AL_INVERSE_DISTANCE_CLAMPED))
    {
        FUNCT_ERROR("ERROR: Could not set Distance Model to AL_INVERSE_DISTANCE_CLAMPED");
    }
}

void SoundSystem::update()
{
    for (auto& [key, value] : m_audioComponents)
    {
        value->updateSources();
    }
}

SoundSystem::~SoundSystem()
{
    ALC_CALL(alcMakeContextCurrent, contextMadeCurrent, device, nullptr);
    ALC_CALL(alcDestroyContext, device, openALContext);
    ALC_CALL(alcCloseDevice, closed, device, device);
}
