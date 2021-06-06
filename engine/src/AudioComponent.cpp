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
    parentPos = getOwner().getTransform().getGlobalPosition();
    getOwner().getTransform().OnUpdate += Function::make(this, "updatePosition");
}

void AudioComponent::onPostLoad() noexcept
{
    Component::onPostLoad();

    parentPos = getOwner().getTransform().getGlobalPosition();
    getOwner().getTransform().OnUpdate += Function::make(this, "updatePosition");
}

void AudioComponent::setSound(const char* soundName, const char* sourceName, const SourceSettings& settings) noexcept
{
    SourceData*    source = getSource(sourceName);
    Sound::Buffer* buffer = Engine::getInstance()->resourceManager.get<Sound::Buffer>(soundName);

    source->isRelative = settings.relative;
    source->settings   = settings;
    source->sourceName = sourceName;
    source->soundName  = soundName;

    AL_CALL(alGenSources, 1, &source->source);
    AL_CALL(alSourcef, source->source, AL_PITCH, settings.pitch);
    AL_CALL(alSourcef, source->source, AL_GAIN, settings.gain);
    AL_CALL(alSource3f, source->source, AL_POSITION, settings.position.x, settings.position.y, settings.position.z);
    AL_CALL(alSource3f, source->source, AL_VELOCITY, settings.velocity.x, settings.velocity.y, settings.velocity.z);
    AL_CALL(alSourcei, source->source, AL_LOOPING, settings.loop);
    AL_CALL(alSourcei, source->source, AL_SOURCE_RELATIVE, settings.relative);
    AL_CALL(alSourcei, source->source, AL_ROLLOFF_FACTOR, ALint(roundf(settings.rollOffFactor)));
    AL_CALL(alSourcei, source->source, AL_MAX_DISTANCE, source->settings.radius);
    AL_CALL(alSourcei, source->source, AL_BUFFER, buffer->buffer);
}

void AudioComponent::updateSource(SourceData* source)
{
    AL_CALL(alSourcef, source->source, AL_PITCH, source->settings.pitch);
    AL_CALL(alSourcef, source->source, AL_GAIN, source->settings.gain);
    AL_CALL(alSourcei, source->source, AL_LOOPING, source->settings.loop);
    AL_CALL(alSource3f, source->source, AL_POSITION, parentPos.x + source->settings.position.x,
            parentPos.y + source->settings.position.y, parentPos.z + +source->settings.position.z);
    AL_CALL(alSourcei, source->source, AL_SOURCE_RELATIVE, source->settings.relative);
    AL_CALL(alSourcei, source->source, AL_ROLLOFF_FACTOR, ALint(roundf(source->settings.rollOffFactor)));
    AL_CALL(alSourcei, source->source, AL_MAX_DISTANCE, source->settings.radius);
}

SourceData* AudioComponent::getSource(const char* name) noexcept
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

SourceData* AudioComponent::findSource(const char* name) noexcept
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
    if (!sources.empty())
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
}

void AudioComponent::updateToSystem() noexcept
{
    if (m_isActivated)
        m_key = Engine::getInstance()->soundSystem.addComponent(this);
    else
    {
        stopAllSound();
        Engine::getInstance()->soundSystem.removeComponent(m_key);
    }
}

void AudioComponent::updatePosition()
{
    GPM::Vec3 pos = getOwner().getTransform().getGlobalPosition();

    if (parentPos == pos)
    {
        return;
    }

    parentPos = pos;

    for (auto& [key, value] : sources)
    {
        if (value.isRelative == AL_FALSE)
        {
            AL_CALL(alSource3f, value.source, AL_POSITION, pos.x + value.settings.position.x,
                    pos.y + +value.settings.position.y, pos.z + +value.settings.position.z);
        }
    }
}

void AudioComponent::updateSources()
{
    if (!sources.empty())
    {
        for (auto& [key, value] : sources)
        {
            if (value.isDirty == true)
            {
                updateSource(&value);
                value.isDirty = false;
            }
        }
    }
}

AudioComponent::~AudioComponent()
{
    setActive(false);

    if (sources.size() > 0)
    {
        for (auto& [key, value] : sources)
        {
            AL_CALL(alDeleteSources, 1, &value.source);
        }

        sources.clear();
    }

    getOwner().getTransform().OnUpdate -= Function::make(this, "updatePosition");
}
