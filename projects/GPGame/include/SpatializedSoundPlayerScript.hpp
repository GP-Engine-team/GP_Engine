/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>
#include <gpm/Vector3.hpp>

// Generated
#include <Generated/SpatializedSoundPlayerScript.rfk.h>

namespace GPE
{
class AudioComponent;
}

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) SpatializedSoundPlayerScript : public GPE::BehaviourComponent
    {
    public:
        SpatializedSoundPlayerScript(GPE::GameObject & owner);
        SpatializedSoundPlayerScript() noexcept = default;
        virtual ~SpatializedSoundPlayerScript() = default;

        void start() override;
        // void update(double deltaTime) override;
        void onPostLoad() override;

    protected:
        RFKField(Serialize()) GPE::AudioComponent* source = nullptr;

        SpatializedSoundPlayerScript_GENERATED
    };

} // namespace )

File_GENERATED