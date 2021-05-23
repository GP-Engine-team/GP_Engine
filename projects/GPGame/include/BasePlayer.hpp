﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <BaseCharacter.hpp>

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
        RFKField(Serialize()) GPE::InputComponent*        input         = nullptr;
        RFKField(Serialize()) GPE::AudioComponent*        source        = nullptr;
        RFKField(Serialize()) Firearm*                    m_fireArme    = nullptr;
        RFKField(Inspect(), Serialize()) GPE::GameObject* m_decalPrefab = nullptr;

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

        void rotate(const GPM::Vec2& deltaDisplacement);
        void start() final;
        void onGUI() final;
        void update(double deltaTime) final;
        void onPostLoad() final;

        BasePlayer_GENERATED
    };

} // namespace )