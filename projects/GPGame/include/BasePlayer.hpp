/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <BaseCharacter.hpp>

#include <Engine/ECS/Component/ParticleComponent.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Serialization/DefaultInspect.hpp>
#include <Engine/Serialization/Separator.hpp>
#include <EvacuationPoint.hpp>
#include <Generated/BasePlayer.rfk.h>

namespace GPE
{
class InputComponent;
class AudioComponent;
class CharacterController;

} // namespace GPE

namespace GPG RFKNamespace()
{
    class Loot;
    class Firearm;

    class RFKClass(ComponentGen, Serialize(), DefaultInspect()) BasePlayer : public BaseCharacter
    {
    protected:
        RFKField(Serialize(), Inspect(), ReadOnly()) float m_animDepthCounter    = 0.f;
        RFKField(Inspect(), Serialize()) float             m_animDepthCounterMax = 3.f;

        RFKField(Inspect(), Serialize(), Separator(true, false)) float m_staminaCount         = 3.f;
        RFKField(Inspect(), Serialize()) float                         m_staminaMax           = 3.f;
        RFKField(Inspect(), Serialize()) float                         m_staminaSpeedRecharge = 0.5f;   // stamina by
                                                                                                        // second
        RFKField(Inspect(), Serialize(), Separator(false, true)) float m_staminaSpeedConsumation = 1.f; // stamina by
                                                                                                        // second

        RFKField(Inspect(), Serialize(), ReadOnly()) unsigned int m_lootCount      = 0u;
        RFKField(Inspect(), Serialize()) unsigned int             m_lootCountToWin = 6u;

        RFKField(Inspect(), Serialize()) GPE::Texture* m_buttonTexture;

        RFKField(Serialize(), Inspect(), ReadOnly()) float            m_animDamageAnimCounter    = 0.f;
        RFKField(Inspect(), Serialize()) float                        m_animDamageAnimCounterMax = 0.1f;
        RFKField(Inspect(), Serialize()) float                        m_damageShakeStrength      = 1.f;
        bool                                                          m_isPlayDamageAnimation    = false;
        RFKField(Inspect(), Serialize()) GPE::Linker<GPE::GameObject> m_cameraGO;

    public:
        RFKField(Inspect(), Serialize()) float radiusLootCollection = 50.f;

    protected:
        RFKField(Serialize()) bool isInEvacuationMode = false;
        RFKField(Serialize()) bool displayDepthMenu   = false;
        RFKField(Serialize()) bool displayWinMenu     = false;

        RFKField(Serialize()) GPE::InputComponent* input  = nullptr;
        RFKField(Serialize()) GPE::AudioComponent* source = nullptr;

        RFKField(Inspect(), Serialize()) std::vector<GPE::Linker<GPE::GameObject>> m_firearmsGO;
        RFKField() std::vector<Firearm*>                                           m_firearms;

        RFKField(Inspect(), Serialize()) GPE::Linker<GPE::ParticleComponent> m_groundParticleComponent;
        RFKField(Inspect(), Serialize()) GPE::Linker<EvacuationPoint>        m_evacuationPoint;

        RFKField(Serialize()) std::string m_mainMenuPath;

    protected:
        RFKMethod() void updateDamageAnimation(float t);

    public:
        BasePlayer() noexcept = default;
        BasePlayer(GPE::GameObject & owner) noexcept;
        BasePlayer(const BasePlayer& other) noexcept = delete;
        BasePlayer(BasePlayer && other) noexcept     = delete;
        virtual ~BasePlayer() noexcept;

        BasePlayer& operator=(BasePlayer const& other) noexcept = delete;
        BasePlayer& operator=(BasePlayer&& other) noexcept = delete;

    public:
        RFKMethod() void leave();
        RFKMethod() void raycastExample();
        RFKMethod() void shoot();
        RFKMethod() void aimBegin();
        RFKMethod() void aimEnd();
        RFKMethod() void playAmbiantMusic();
        RFKMethod() void playAmbiantMusicForce();
        RFKMethod() void stopAllMusic();
        RFKMethod() void updateListener();
        RFKMethod() void collectLoot(const Loot& loot);
        RFKMethod() void onDeath() override;
        RFKMethod() void takeDamage(float damage) override;
        RFKMethod() void onWin();

        void rotate(const GPM::Vec2& deltaDisplacement);
        void start() final;
        void onGUI() final;
        void update(double deltaTime) final;
        void onPostLoad() final;

        void inspect(GPE::InspectContext & context) override;

        BasePlayer_GENERATED
    };

} // namespace )
