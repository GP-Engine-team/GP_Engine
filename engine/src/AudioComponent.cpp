#include "Engine/Core/Sound/AudioComponent.hpp"
#include "Engine/Core/Sound/SoundSystem.hpp"

using namespace GPE;

AudioComponent::AudioComponent(GameObject& owner) noexcept : Component(owner)
{
    // Check if OpenAL Soft handle enumeration
    {
        enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
        if (enumeration == AL_FALSE)
        {
            // LogError(SeverityEnum::Low, "alcIsExtensionPresent()", "Enumeration not supported !");
        }

        else
        {
            // Loading::Print(InfluenceEnum::Small, "Enumeration supported ...");
        }
    }

    // Gen device
    {
        device = alcOpenDevice(NULL);
        if (!device)
        {
            // LogError(SeverityEnum::Low, "alcOpenDevice()", "No Device set !");
        }
    }

    // Gen Context
    if (!ALC_CALL(alcCreateContext, openALContext, device, device, nullptr) || !openALContext)
    {
        // LogError(SeverityEnum::High, "alcCreateContext()", "ERROR: Could not create audio context");
    }

    if (!ALC_CALL(alcMakeContextCurrent, contextMadeCurrent, device, openALContext) || contextMadeCurrent != ALC_TRUE)
    {
        // LogError(SeverityEnum::High, "alcCreateContext()", "ERROR: Could not create audio context");
    }

    m_key = SoundSystem::getInstance()->addComponent(this);
}

AudioComponent::AudioComponent(const AudioComponent& other) noexcept : Component(other.m_gameObject)
{
}

AudioComponent::AudioComponent(AudioComponent&& other) noexcept : Component(other.m_gameObject)
{
}

void AudioComponent::setSound(const Sound& sound) noexcept
{
    AL_CALL(alGenBuffers, 1, &buffer);

    if (sound.channels == 1 && sound.bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if (sound.channels == 1 && sound.bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if (sound.channels == 2 && sound.bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if (sound.channels == 2 && sound.bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else
    {
        std::cerr << "ERROR: unrecognised wave format: " << sound.channels << " channels, " << sound.bitsPerSample
                  << " bps" << std::endl;
    }

    AL_CALL(alBufferData, buffer, format, sound.data, sound.size, sound.sampleRate);

    source;
    AL_CALL(alGenSources, 1, &source);
    AL_CALL(alSourcef, source, AL_PITCH, 1);
    AL_CALL(alSourcef, source, AL_GAIN, 1.f);
    AL_CALL(alSource3f, source, AL_POSITION, 0, 0, 0);
    AL_CALL(alSource3f, source, AL_VELOCITY, 0, 0, 0);
    AL_CALL(alSourcei, source, AL_LOOPING, AL_FALSE);
    AL_CALL(alSourcei, source, AL_BUFFER, buffer);
}

/*ALuint AudioComponent::getSource()
{
}*/

void AudioComponent::playSound() noexcept
{
    AL_CALL(alSourcePlay, source);

    state = AL_PLAYING;
}

AudioComponent::~AudioComponent() noexcept
{
    AL_CALL(alDeleteSources, 1, &source);
    AL_CALL(alDeleteBuffers, 1, &buffer);

    ALC_CALL(alcMakeContextCurrent, contextMadeCurrent, device, nullptr);
    ALC_CALL(alcDestroyContext, device, openALContext);

    closed;
    ALC_CALL(alcCloseDevice, closed, device, device);
}
