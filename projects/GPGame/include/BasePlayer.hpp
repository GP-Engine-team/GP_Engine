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
#include <Generated/BasePlayer.rfk.h>

namespace GPE
{
class InputComponent;
class AudioComponent;
class CharacterController;

} // namespace GPE

namespace GPG RFKNamespace()
{
    class Firearm;

    class RFKClass(Inspect(), ComponentGen, Serialize()) BasePlayer : public BaseCharacter
    {
    protected:
        RFKField(Serialize()) float            m_animDepthCounter    = 0.f;
        RFKField(Inspect(), Serialize()) float m_animDepthCounterMax = 3.f;

        RFKField(Serialize()) bool displayDepthMenu = false;

        RFKField(Serialize()) GPE::InputComponent* input  = nullptr;
        RFKField(Serialize()) GPE::AudioComponent* source = nullptr;

        RFKField(Inspect(), Serialize()) std::vector<GPE::Linker<GPE::GameObject>> m_firearmsGO;
        RFKField() std::vector<Firearm*>                                           m_firearms;

        RFKField(Inspect(), Serialize()) GPE::Linker<GPE::GameObject> m_groundParticleGO;
        RFKField() GPE::ParticleComponent*                            m_pGroundParticleComponent;

    public:
        BasePlayer() noexcept = default;
        BasePlayer(GPE::GameObject & owner) noexcept;
        BasePlayer(const BasePlayer& other) noexcept = delete;
        BasePlayer(BasePlayer && other) noexcept     = delete;
        virtual ~BasePlayer() noexcept               = default;

        BasePlayer& operator=(BasePlayer const& other) noexcept = delete;
        BasePlayer& operator=(BasePlayer&& other) noexcept = delete;

    public:
        RFKMethod() void leave();
        RFKMethod() void raycastExample();
        RFKMethod() void shoot();
        RFKMethod() void playAmbiantMusic();
        RFKMethod() void playAmbiantMusicForce();
        RFKMethod() void stopAllMusic();
        RFKMethod() void updateListener();

        void rotate(const GPM::Vec2& deltaDisplacement);
        void start() final;
        void onGUI() final;
        void update(double deltaTime) final;
        void onPostLoad() final;

        BasePlayer_GENERATED
    };

} // namespace )
