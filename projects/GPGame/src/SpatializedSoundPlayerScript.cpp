#include <SpatializedSoundPlayerScript.hpp>

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
    onPostLoad();
}

void SpatializedSoundPlayerScript::onPostLoad()
{
    BehaviourComponent::onPostLoad();

    if (!source)
    {
        source = getOwner().getComponent<GPE::AudioComponent>();
    }

    GPE::SourceSettings sourceSettings;
    sourceSettings.pitch       = 2.f;
    sourceSettings.loop        = AL_TRUE;
    sourceSettings.spatialized = AL_TRUE;
    sourceSettings.relative    = AL_FALSE;
    sourceSettings.gain *= 1000;

    GPE::Wave testSound3("./resources/sounds/BuriedAlive.wav", "BA", sourceSettings.spatialized);
    source->setSound("BA", "BA", sourceSettings);
}

void SpatializedSoundPlayerScript::start()
{
    enableUpdate(true);
    GAME_ASSERT(source, "null");

    source->playSound("BA", true);
}
