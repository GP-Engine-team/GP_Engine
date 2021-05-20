/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>

#include <Generated/myFpsScript.rfk.h>

namespace GPE
{

class InputComponent;
class AudioComponent;
class CharacterController;

} // namespace GPE

namespace GPG RFKNamespace()
{

    class Firearm;

    class RFKClass(Inspect(), ComponentGen, Serialize()) MyFpsScript : public GPE::BehaviourComponent
    {
    private:
        RFKField(Serialize()) GPE::InputComponent*        input         = nullptr;
        RFKField(Serialize()) GPE::AudioComponent*        source        = nullptr;
        RFKField(Serialize()) GPE::CharacterController*   controller    = nullptr;
        RFKField(Serialize()) Firearm*                    m_fireArme    = nullptr;
        RFKField(Inspect(), Serialize()) GPE::GameObject* m_decalPrefab = nullptr;

    public:
        MyFpsScript() noexcept = default;
        MyFpsScript(GPE::GameObject & owner) noexcept;
        MyFpsScript(const MyFpsScript& other) noexcept = delete;
        MyFpsScript(MyFpsScript && other) noexcept     = delete;
        virtual ~MyFpsScript() noexcept                = default;

        MyFpsScript& operator=(MyFpsScript const& other) noexcept = delete;
        MyFpsScript& operator=(MyFpsScript&& other) noexcept = delete;

    public:
        RFKMethod() void jump();
        RFKMethod() void forward();
        RFKMethod() void backward();
        RFKMethod() void left();
        RFKMethod() void right();
        RFKMethod() void leave();
        RFKMethod() void sprintStart();
        RFKMethod() void sprintEnd();
        RFKMethod() void raycastExample();
        RFKMethod() void shoot();
        RFKMethod() void playAmbiantMusic();
        RFKMethod() void playAmbiantMusicForce();
        RFKMethod() void stopAllMusic();
        // RFKMethod() void growUpSphereCollider  ();
        // RFKMethod() void growDownSphereCollider();

        void rotate(const GPM::Vec2& deltaDisplacement);
        void start() final;
        void onGUI() final;
        void fixedUpdate(double deltaTime) final;
        void update(double deltaTime) final;
        void onPostLoad() final;

        MyFpsScript_GENERATED
    };

} // namespace )
