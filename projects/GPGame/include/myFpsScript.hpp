/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <FireArme/PPSH41.hpp>

#include <memory>

#include "Generated/myFpsScript.rfk.h"

namespace GPE
{

class InputComponent;
class AudioComponent;
class CharacterController;

} // namespace GPE

namespace GPG RFKNamespace()
{

    class RFKClass(Inspect(), ComponentGen, Serialize()) MyFpsScript : public GPE::BehaviourComponent
    {
    private:
        RFKField(Serialize()) GPE::InputComponent*      input      = nullptr;
        RFKField(Serialize()) GPE::AudioComponent*      source     = nullptr;
        RFKField(Serialize()) GPE::CharacterController* controller = nullptr;
        RFKField(Serialize()) FireArme*                 m_fireArme = nullptr;

    public:
        MyFpsScript(GPE::GameObject & owner) noexcept;
        MyFpsScript() noexcept                         = default;
        MyFpsScript(const MyFpsScript& other) noexcept = delete;
        MyFpsScript(MyFpsScript && other) noexcept     = delete;
        virtual ~MyFpsScript() noexcept;

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
        RFKMethod() void shoot();
        // RFKMethod() void growUpSphereCollider  ();
        // RFKMethod() void growDownSphereCollider();

        void rotate(const GPM::Vec2& deltaDisplacement);
        void onGUI() final;
        void update(double deltaTime) final;
        void fixedUpdate(double deltaTime) final;

        MyFpsScript_GENERATED
    };

} // namespace )
