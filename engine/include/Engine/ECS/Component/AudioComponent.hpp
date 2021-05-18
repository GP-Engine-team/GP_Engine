/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"
#include "Engine/Resources/Sound.hpp"

#include <OpenAL_Soft_Debug.hpp>
#include <unordered_map>

// Generated
#include "Generated/AudioComponent.rfk.h"

namespace GPE RFKNamespace()
{
    struct SourceSettings
    {
        ALfloat   pitch       = 1.f;
        ALfloat   gain        = 1.f;
        ALfloat   position[3] = {0, 0, 0};
        ALfloat   velocity[3] = {0, 0, 0};
        ALboolean loop        = AL_FALSE;
    };

    class RFKClass(Inspect(), Serialize()) AudioComponent : public Component
    {
    public:
        AudioComponent(GameObject & owner);

        virtual ~AudioComponent();

        AudioComponent()                            = default;
        AudioComponent(const AudioComponent& other) = delete;
        AudioComponent& operator=(AudioComponent const& other) = delete;

        // AudioComponent(AudioComponent&& other) noexcept = default;
        AudioComponent& operator                         =(AudioComponent&& other);
        AudioComponent(AudioComponent && other) noexcept = default;

    private:
        int m_key = -1;

    public:
        struct RFKStruct(Serialize()) SourceData
        {
            RFKField(Serialize())
            ALuint source;
            RFKField(Serialize())
            ALint  state = AL_INITIAL;

            SourceData_GENERATED
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
        void playSound(const char* name, bool forceStart) noexcept;

        /**
         * @brief Stop the current bound sound
         */
        void stopSound(const char* name) noexcept;

        /**
         * @brief Stop all the sound bound to the Audio Component
         * @return
         */
        void stopAllSound() noexcept;

        [[nodiscard]] int getKey() const noexcept
        {
            return m_key;
        }

        /**
         * @brief Add or remove current component from it's system which have for effect to enable or disable it
         * @param newState
         * @return
         */
        void setActive(bool newState) noexcept override;

        virtual void onPostLoad() override;

    private:
        [[nodiscard]] SourceData* getSource(const char* name) noexcept;

        AudioComponent_GENERATED
    };
} // namespace )
