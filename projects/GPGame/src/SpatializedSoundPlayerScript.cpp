﻿#include <SpatializedSoundPlayerScript.hpp>

#include <Generated/SpatializedSoundPlayerScript.rfk.h>
File_GENERATED

#include <Engine/Core/Debug/assert.hpp>
#include <Engine/ECS/Component/AudioComponent.hpp>
#include <Engine/ECS/System/SoundSystem.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>
#include <algorithm>

using namespace GPE;
using namespace GPG;
using namespace GPM;

SpatializedSoundPlayerScript::SpatializedSoundPlayerScript(GameObject& owner)
    : BehaviourComponent(owner), source{&owner.addComponent<GPE::AudioComponent>()}
{
    GPE::Wave testSound3("./resources/sounds/E_Western.wav", "E_Western");

    GPE::SourceSettings sourceSettings;
    sourceSettings.pitch = 1.f;
    sourceSettings.loop  = AL_TRUE;
    // sourceSettings.position = ALfloat(0.f);

    source->setSound("E_Western", "E_Western", sourceSettings);
    source->stopSound("E_Western");
    // source->setSound("Western", "Western", sourceSettings);
}

void SpatializedSoundPlayerScript::onPostLoad()
{
    BehaviourComponent::onPostLoad();
}

void SpatializedSoundPlayerScript::start()
{
    enableUpdate(true);
    GAME_ASSERT(source, "null");

    source->playSound("E_Western", true);
}
