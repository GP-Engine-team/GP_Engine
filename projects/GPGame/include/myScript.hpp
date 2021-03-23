﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

//#include "Engine/ECS/Component/AudioComponent.hpp"
#include "Engine/ECS/Component/BehaviourComponent.hpp"
#include "Engine/ECS/Component/InputComponent.hpp"
#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"
//#include "Engine/Resources/Wave.hpp"

#include <iostream>

namespace GPG
{
class MyScript : public GPE::BehaviourComponent
{
public:
    inline MyScript(GPE::GameObject& owner) noexcept
        : GPE::BehaviourComponent(owner), input(owner.addComponent<GPE::InputComponent>()) /*,
      source(owner.addComponent<GPE::AudioComponent>())*/
    {
        enableUpdate(true);
        input.bindAction("jump", EKeyMode::KEY_DOWN, this, &MyScript::up);
        input.bindAction("down", EKeyMode::KEY_DOWN, this, &MyScript::down);
        input.bindAction("right", EKeyMode::KEY_DOWN, this, &MyScript::right);
        input.bindAction("left", EKeyMode::KEY_DOWN, this, &MyScript::left);
        input.bindAction("forward", EKeyMode::KEY_DOWN, this, &MyScript::forward);
        input.bindAction("back", EKeyMode::KEY_DOWN, this, &MyScript::back);
        input.bindAction("exit", EKeyMode::KEY_PRESSED, this, &MyScript::leave);
        input.bindAction("sprintStart", EKeyMode::KEY_PRESSED, this, &MyScript::sprintStart);
        input.bindAction("sprintEnd", EKeyMode::KEY_RELEASED, this, &MyScript::sprintEnd);

        /*GPE::Wave testSound("./resources/sounds/RickRoll.wav", "RICKROLL");
        GPE::Wave testSound2("./resources/sounds/YMCA.wav", "YMCA");
        GPE::Wave testSound3("./resources/sounds/E_Western.wav", "Western");

        GPE::SourceSettings sourceSettings;
        sourceSettings.pitch = 1;
        sourceSettings.loop  = AL_TRUE;

        source.setSound("Western", "Western", sourceSettings);
        source.playSound("Western");*/
    }

    MyScript() noexcept                      = delete;
    MyScript(const MyScript& other) noexcept = delete;
    MyScript(MyScript&& other) noexcept      = default;
    virtual ~MyScript() noexcept             = default;
    MyScript& operator=(MyScript const& other) noexcept = delete;
    MyScript& operator=(MyScript&& other) noexcept = delete;

    GPE::InputComponent& input;
    // GPE::AudioComponent& source;
    float speed;

    void rotate(const GPM::Vec2& deltaDisplacement)
    {
        if (deltaDisplacement.length() > 0.4)
        {
            getOwner().getTransform().setRotation(getOwner().getTransform().getSpacialAttribut().rotation *
                                                  GPM::Quaternion::angleAxis(deltaDisplacement.y * 0.001f, {1, 0, 0}));
            getOwner().getTransform().setRotation(GPM::Quaternion::angleAxis(deltaDisplacement.x * 0.001f, {0, 1, 0}) *
                                                  getOwner().getTransform().getSpacialAttribut().rotation);
        }
    }

    inline void up()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorUp() * speed);
    }

    inline void down()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorUp() * -1 * speed);
    }

    inline void forward()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorForward() * -1 * speed);
    }

    inline void back()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorForward() * speed);
    }

    inline void left()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorRight() * -1 * speed);
    }

    inline void right()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorRight() * speed);
    }

    inline void leave()
    {
        exit(666);
    }

    inline void sprintStart()
    {
        speed = 2;
    }

    inline void sprintEnd()
    {
        speed = 1;
    }

    void update(float deltaTime) final
    {
        rotate(GPE::Engine::getInstance()->inputManager.getCursor().deltaPos);
    }
};
} /*namespace GPG*/
