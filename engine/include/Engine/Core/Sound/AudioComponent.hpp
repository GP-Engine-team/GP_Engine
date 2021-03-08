/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include "Engine/Core/Debug/OpenAL_Soft_Debug.hpp"
#include "Engine/Intermediate/Component.hpp"
#include "Engine/Resources/Sound.hpp"

namespace GPE
{
class AudioComponent : public Component
{
public:
    AudioComponent(const AudioComponent& other) noexcept;
    AudioComponent(AudioComponent&& other) noexcept;
    virtual ~AudioComponent() noexcept;
    AudioComponent(GameObject& owner) noexcept;

private:
    ALboolean   enumeration;
    ALCdevice*  device;
    ALCcontext* openALContext;
    ALCboolean  contextMadeCurrent = false;
    ALuint      buffer;
    ALenum      format = 0;
    ALCboolean  closed;
    int         m_key = -1;

public:
    ALuint source;
    ALint  state = AL_INITIAL;
    /**
     * @brief Bind a sound on the current source
     * @param sound
     */
    void setSound(const Sound& sound) noexcept;

    /**
     * @brief Play the current bound sound
     */
    void playSound() noexcept;

    /*private:
        [[nodiscard]] ALuint getSource() noexcept;*/
};
} // namespace GPE
