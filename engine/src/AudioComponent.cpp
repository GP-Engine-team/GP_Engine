#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Engine.hpp"
#include <Engine/ECS/Component/AudioComponent.hpp>

using namespace GPE;
using namespace std;

AudioComponent::AudioComponent(GameObject& owner) : Component(owner)
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

    m_key = Engine::getInstance()->soundSystem.addComponent(this);
}

AudioComponent& AudioComponent::operator=(AudioComponent&& other)
{
    m_enumeration        = std::move(other.m_enumeration);
    m_device             = std::move(other.m_device);
    m_openALContext      = std::move(other.m_openALContext);
    m_contextMadeCurrent = std::move(other.m_contextMadeCurrent);
    m_closed             = std::move(other.m_closed);
    m_key                = std::move(other.m_key);
    sources              = std::move(other.sources);

    Engine::getInstance()->soundSystem.updateComponent(this);

    Component::operator=(std::move(other));
    return *this;
}

void AudioComponent::setSound(const char* soundName, const char* sourceName, const SourceSettings& settings) noexcept
{

    SourceData* source = getSource(sourceName);

    AL_CALL(alGenSources, 1, &source->source);
    AL_CALL(alSourcef, source->source, AL_PITCH, settings.pitch);
    AL_CALL(alSourcef, source->source, AL_GAIN, settings.gain);
    AL_CALL(alSource3f, source->source, AL_POSITION, settings.position[0], settings.position[1], settings.position[2]);
    AL_CALL(alSource3f, source->source, AL_VELOCITY, settings.velocity[0], settings.velocity[1], settings.velocity[2]);
    AL_CALL(alSourcei, source->source, AL_LOOPING, settings.loop);
    AL_CALL(alSourcei, source->source, AL_BUFFER,
            Engine::getInstance()->resourceManager.get<Sound::Buffer>(soundName)->buffer);
}

AudioComponent::SourceData* AudioComponent::getSource(const char* name) noexcept
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

void AudioComponent::playSound(const char* name) noexcept
{
    SourceData* source = findSource(name);
    AL_CALL(alSourcePlay, source->source);
    source->state = AL_PLAYING;
}

AudioComponent::~AudioComponent()
{
    // AL_CALL(alDeleteSources, 1, &sources[i].source);
    // AL_CALL(alDeleteBuffers, 1, &buffer);

    ALC_CALL(alcMakeContextCurrent, m_contextMadeCurrent, m_device, nullptr);
    ALC_CALL(alcDestroyContext, m_device, m_openALContext);
    ALC_CALL(alcCloseDevice, m_closed, m_device, m_device);
}

AudioComponent::SourceData* AudioComponent::findSource(const char* name) noexcept
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

void AudioComponent::stopSound(const char* name) noexcept
{
    SourceData* source = findSource(name);
    AL_CALL(alSourceStop, source->source);
    source->state = AL_STOPPED;
}
