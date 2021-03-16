/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <OpenAL_Soft_Debug.hpp>
#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"
#include "Engine/Resources/Sound.hpp"
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

    // SourceSettings() = default;
    // SourceSettings(ALfloat pitch, ALfloat gain, ALfloat position[3], ALfloat velocity[3], ALboolean loop = AL_FALSE)
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
    ALCboolean  m_closed;
    int         m_key = -1;

public:
    struct SourceData
    {
        ALuint source;
        ALint  state = AL_INITIAL;
    };
    std::unordered_map<std::string, SourceData> sources;

    /**
     * @brief Find and return the corresponding source in the source list
     * @param name of the source
     * @return the source
     */
    [[nodiscard]] SourceData* findSource(const char* name) noexcept;

    /**
     * @brief Bind a sound on the current source
     * @param sound
     */
    void setSound(const char* soundName, const char* sourceName, const SourceSettings& settings) noexcept;

    /**
     * @brief Play the current bound sound
     */
    void playSound(const char* name) noexcept;

    /**
     * @brief Stop the current bound sound
     */
    void stopSound(const char* name) noexcept;

private:
    [[nodiscard]] SourceData* getSource(const char* name) noexcept;
};
} // namespace GPE
