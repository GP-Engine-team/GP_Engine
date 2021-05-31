#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Engine.hpp"
#include <Engine/ECS/Component/AudioComponent.hpp>
#include <Engine/Intermediate/GameObject.hpp>

// Generated
#include "Generated/AudioComponent.rfk.h"

File_GENERATED

    using namespace GPE;
using namespace std;

AudioComponent::AudioComponent(GameObject& owner) : Component(owner)
{
    updateToSystem();
    getOwner().getTransform().OnUpdate += Function::make(this, "updatePosition");
}

void AudioComponent::setSound(const char* soundName, const char* sourceName, const SourceSettings& settings) noexcept
{
    SourceData*    source = getSource(sourceName);
    Sound::Buffer* buffer = Engine::getInstance()->resourceManager.get<Sound::Buffer>(soundName);

    source->isRelative = settings.relative;
    source->settings   = settings;

    AL_CALL(alGenSources, 1, &source->source);
    if (settings.spatialized == AL_TRUE)
    {
        AL_CALL(alSourcef, source->source, AL_PITCH, settings.pitch * 2);
    }

    else
    {
        AL_CALL(alSourcef, source->source, AL_PITCH, settings.pitch);
    }
    AL_CALL(alSourcef, source->source, AL_GAIN, settings.gain);
    AL_CALL(alSource3f, source->source, AL_POSITION, settings.position[0], settings.position[1], settings.position[2]);
    AL_CALL(alSource3f, source->source, AL_VELOCITY, settings.velocity[0], settings.velocity[1], settings.velocity[2]);
    AL_CALL(alSourcei, source->source, AL_LOOPING, settings.loop);
    AL_CALL(alSourcei, source->source, AL_SOURCE_RELATIVE, settings.relative);
    AL_CALL(alSourcei, source->source, AL_ROLLOFF_FACTOR, settings.rollOffFactor);
    AL_CALL(alSourcei, source->source, AL_BUFFER, buffer->buffer);
}

AudioComponent::SourceData* AudioComponent::getSource(const char* name) noexcept
{
    auto it = sources.find(name);
    if (it != sources.end())
    {
        return &it->second;
    }

    else
    {
        return &sources.emplace(name, SourceData{}).first->second;
    }
}

void AudioComponent::playSound(const char* name, bool forceStart) noexcept
{
    if (m_isActivated)
    {
        SourceData* source = findSource(name);
        if (source && (source->state != AL_PLAYING || forceStart))
        {
            AL_CALL(alSourcePlay, source->source);
            source->state = AL_PLAYING;
        }
    }
}

AudioComponent::SourceData* AudioComponent::findSource(const char* name) noexcept
{
    auto it = sources.find(name);
    if (it != sources.end())
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

void AudioComponent::stopAllSound() noexcept
{
    for (auto& [key, value] : sources)
    {
        if (value.state == AL_PLAYING)
        {
            AL_CALL(alSourceStop, value.source);
            value.state = AL_STOPPED;
        }
    }
}

void AudioComponent::updateToSystem() noexcept
{
    if (m_isActivated)
        Engine::getInstance()->soundSystem.addComponent(this);
    else
    {
        stopAllSound();
        Engine::getInstance()->soundSystem.removeComponent(m_key);
    }
}

void AudioComponent::updatePosition()
{
    GPM::Vec3 pos = getOwner().getTransform().getGlobalPosition();
    for (auto& [key, value] : sources)
    {
        if (value.isRelative == AL_FALSE)
        {
            AL_CALL(alSource3f, value.source, AL_POSITION, pos.x, pos.y, pos.z);
        }
    }
}

AudioComponent::~AudioComponent()
{
    setActive(false);
    getOwner().getTransform().OnUpdate -= Function::make(this, "updatePosition");
}
