﻿/*
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
    struct RFKStruct(Serialize(), Inspect()) SourceSettings
    {
        RFKField(Serialize(), Inspect()) float     pitch         = 1.f;
        RFKField(Serialize(), Inspect()) float     gain          = 1.f;
        RFKField(Serialize(), Inspect()) GPM::Vec3 position      = {0, 0, 0};
        RFKField(Serialize(), Inspect()) GPM::Vec3 velocity      = {0, 0, 0};
        RFKField(Serialize(), Inspect()) bool      loop          = AL_FALSE;
        RFKField(Serialize(), Inspect()) bool      relative      = AL_FALSE;
        RFKField(Serialize(), Inspect()) bool      spatialized   = AL_FALSE;
        RFKField(Serialize(), Inspect()) float     rollOffFactor = 1.f;

        SourceSettings_GENERATED
    };

    struct RFKStruct(Serialize(), Inspect()) SourceData
    {
        RFKField(Serialize(), Inspect()) ALuint         source;
        RFKField(Serialize(), Inspect()) ALint          state      = AL_INITIAL;
        RFKField(Serialize(), Inspect()) bool           isRelative = AL_FALSE;
        RFKField(Serialize(), Inspect()) SourceSettings settings;
        RFKField(Serialize(), Inspect()) std::string    name    = "default";
        bool                                            isDirty = false;

        SourceData_GENERATED
    };

    class RFKClass(Inspect(), Serialize()) AudioComponent : public Component
    {
    public:
        AudioComponent(GameObject & owner);

        virtual ~AudioComponent();

        AudioComponent() = default;

    private:
        int m_key = -1;

    public:
        RFKField(Serialize(), Inspect()) std::unordered_map<std::string, SourceData> sources;

    protected:
        virtual void updateToSystem() noexcept override;

    public:
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

        RFKMethod() void updatePosition();

        [[nodiscard]] int getKey() const noexcept
        {
            return m_key;
        }

    private:
        [[nodiscard]] SourceData* getSource(const char* name) noexcept;

        AudioComponent_GENERATED
    };
} // namespace )
