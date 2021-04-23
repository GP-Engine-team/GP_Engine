/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include "Generated/myFpsScript.rfk.h"

namespace GPE
{

class InputComponent;
class AudioComponent;
class CharacterController;

}

namespace GPG RFKNamespace()
{

class RFKClass(Inspect(), ComponentGen, Serialize()) MyFpsScript : public GPE::BehaviourComponent
{
private:
    RFKField(Serialize()) GPE::InputComponent*      input      = nullptr;
    RFKField(Serialize()) GPE::AudioComponent*      source     = nullptr;
    RFKField(Serialize()) GPE::CharacterController* controller = nullptr;

    /* Variable setter serialization example
    RFKField(Inspect("setPrintHello"))
    bool printHello = false;

    void setPrintHello(bool p);
    */

public:
    MyFpsScript           (GPE::GameObject& owner)   noexcept;
    MyFpsScript           ()                         noexcept = default;
    MyFpsScript           (const MyFpsScript& other) noexcept = delete;
    MyFpsScript           (MyFpsScript && other)     noexcept = delete;
    virtual ~MyFpsScript  ()                         noexcept;

    MyFpsScript& operator=(MyFpsScript const& other) noexcept = delete;
    MyFpsScript& operator=(MyFpsScript&& other)      noexcept = delete;

public:
    RFKMethod() void jump                  ();
    RFKMethod() void forward               ();
    RFKMethod() void backward              ();
    RFKMethod() void left                  ();
    RFKMethod() void right                 ();
    RFKMethod() void leave                 ();
    RFKMethod() void sprintStart           ();
    RFKMethod() void sprintEnd             ();
    //RFKMethod() void growUpSphereCollider  ();
    //RFKMethod() void growDownSphereCollider();
    RFKMethod() void swapInputModeToGame01 ();
    RFKMethod() void swapInputModeToGame02 ();

    void rotate                            (const GPM::Vec2& deltaDisplacement);
    void onGUI                             () final;
    void fixedUpdate                       (float deltaTime) final;

    MyFpsScript_GENERATED
};

} // namespace GPG
