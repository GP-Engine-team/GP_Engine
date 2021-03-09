#include "Engine/Core/Sound/AudioComponent.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Sound/SoundSystem.hpp"

using namespace GPE;
using namespace std;

AudioComponent::AudioComponent(GameObject& owner) noexcept : Component(owner)
{
    // Check if OpenAL Soft handle m_enumeration
    {
        m_enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
        if (m_enumeration == AL_FALSE)
        {
            FUNCT_ERROR("Enumeration not supported !");
        }

        else
        {
            Log::logAddMsg("Enumeration supported ...");
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

    m_key = SoundSystem::getInstance()->addComponent(this);
}

AudioComponent::AudioComponent(const AudioComponent& other) noexcept : Component(other.m_gameObject)
{
}

AudioComponent::AudioComponent(AudioComponent&& other) noexcept : Component(other.m_gameObject)
{
}

void AudioComponent::setSound(const Sound& sound, const string& sourceName, const SourceSettings& settings) noexcept
{
    SourceData* source = getSource(sourceName);

    AL_CALL(alGenBuffers, 1, &source->buffer);

    if (sound.channels == 1 && sound.bitsPerSample == 8)
        m_format = AL_FORMAT_MONO8;
    else if (sound.channels == 1 && sound.bitsPerSample == 16)
        m_format = AL_FORMAT_MONO16;
    else if (sound.channels == 2 && sound.bitsPerSample == 8)
        m_format = AL_FORMAT_STEREO8;
    else if (sound.channels == 2 && sound.bitsPerSample == 16)
        m_format = AL_FORMAT_STEREO16;
    else
    {
        std::cerr << "ERROR: unrecognised wave format: " << sound.channels << " channels, " << sound.bitsPerSample
                  << " bps" << std::endl;
    }

    AL_CALL(alBufferData, source->buffer, m_format, sound.data, sound.size, sound.sampleRate);
    AL_CALL(alGenSources, 1, &source->source);
    AL_CALL(alSourcef, source->source, AL_PITCH, settings.pitch);
    AL_CALL(alSourcef, source->source, AL_GAIN, settings.gain);
    AL_CALL(alSource3f, source->source, AL_POSITION, settings.position[0], settings.position[1], settings.position[2]);
    AL_CALL(alSource3f, source->source, AL_VELOCITY, settings.velocity[0], settings.velocity[1], settings.velocity[2]);
    AL_CALL(alSourcei, source->source, AL_LOOPING, settings.loop);
    AL_CALL(alSourcei, source->source, AL_BUFFER, source->buffer);
}

AudioComponent::SourceData* AudioComponent::getSource(const string& name) noexcept
{
    if (sources.find(name) != sources.end())
    {
        return &sources.find(name)->second;
    }

    else
    {
        return &sources.emplace(name, SourceData{}).first->second;
    }
}

void AudioComponent::playSound(SourceData* source) noexcept
{
    AL_CALL(alSourcePlay, source->source);

    source->state = AL_PLAYING;
}

AudioComponent::~AudioComponent() noexcept
{
    // AL_CALL(alDeleteSources, 1, &sources[i].source);
    // AL_CALL(alDeleteBuffers, 1, &buffer);

    ALC_CALL(alcMakeContextCurrent, m_contextMadeCurrent, m_device, nullptr);
    ALC_CALL(alcDestroyContext, m_device, m_openALContext);
    ALC_CALL(alcCloseDevice, m_closed, m_device, m_device);
}

AudioComponent::SourceData* AudioComponent::findSource(const std::string& name) noexcept
{
    if (sources.find(name) != sources.end())
    {
        return &sources.find(name)->second;
    }

    else
    {
        FUNCT_ERROR("This source doesn't exist");
        return nullptr;
    }
}

void AudioComponent::stopSound(SourceData* source) noexcept
{
    AL_CALL(alSourceStop, source->source);

    source->state = AL_STOPPED;
}
