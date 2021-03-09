/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include "Engine/Core/Debug/OpenAL_Soft_Debug.hpp"
#include "Engine/Intermediate/Component.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"
#include "Engine/Resources/Sound.hpp"
#include <string>
#include <unordered_map>

namespace GPE
{

struct SourceSettings
{
    ALfloat   pitch       = 1.f;
    ALfloat   gain        = 1.f;
    ALfloat   position[3] = {0, 0, 0};
    ALfloat   velocity[3] = {0, 0, 0};
    ALboolean loop        = AL_FALSE;
};

class AudioComponent : public Component
{
public:
    AudioComponent(const AudioComponent& other) noexcept;
    AudioComponent(AudioComponent&& other) noexcept;
    virtual ~AudioComponent() noexcept;
    AudioComponent(GameObject& owner) noexcept;

private:
    ALboolean   m_enumeration;
    ALCdevice*  m_device;
    ALCcontext* m_openALContext;
    ALCboolean  m_contextMadeCurrent = false;
    ALenum      m_format             = 0;
    ALCboolean  m_closed;
    int         m_key = -1;

public:
    struct SourceData
    {
        ALuint source;
        ALint  state = AL_INITIAL;
        ALuint buffer;
    };
    std::unordered_map<std::string, SourceData> sources;

    /**
     * @brief Find and return the corresponding source in the source list
     * @param name of the source
     * @return the source
     */
    [[nodiscard]] SourceData* findSource(const std::string& name) noexcept;

    /**
     * @brief Bind a sound on the current source
     * @param sound
     */
    void setSound(const Sound& sound, const std::string& sourceName, const SourceSettings& settings) noexcept;

    /**
     * @brief Play the current bound sound
     */
    void playSound(SourceData* source) noexcept;

    /**
     * @brief Stop the current bound sound
     */
    void stopSound(SourceData* source) noexcept;

private:
    [[nodiscard]] SourceData* getSource(const std::string& name) noexcept;
};
} // namespace GPE
