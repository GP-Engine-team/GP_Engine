﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/AudioComponent.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/InputComponent.hpp>
#include <Engine/ECS/System/InputManagerGLFW.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>

#include <iostream>

namespace GPG
{
class MyFreeFlyScript : public GPE::BehaviourComponent
{
public:
    inline MyFreeFlyScript(GPE::GameObject& owner) noexcept
        : GPE::BehaviourComponent(owner), input(owner.addComponent<GPE::InputComponent>()),
          source(owner.addComponent<GPE::AudioComponent>())
    {
        enableUpdate(true);
        input.bindAction("jump", EKeyMode::KEY_DOWN, this, &MyFreeFlyScript::up);
        input.bindAction("down", EKeyMode::KEY_DOWN, this, &MyFreeFlyScript::down);
        input.bindAction("right", EKeyMode::KEY_DOWN, this, &MyFreeFlyScript::right);
        input.bindAction("left", EKeyMode::KEY_DOWN, this, &MyFreeFlyScript::left);
        input.bindAction("forward", EKeyMode::KEY_DOWN, this, &MyFreeFlyScript::forward);
        input.bindAction("back", EKeyMode::KEY_DOWN, this, &MyFreeFlyScript::back);
        input.bindAction("exit", EKeyMode::KEY_PRESSED, this, &MyFreeFlyScript::leave);
        input.bindAction("sprintStart", EKeyMode::KEY_PRESSED, this, &MyFreeFlyScript::sprintStart);
        input.bindAction("sprintEnd", EKeyMode::KEY_RELEASED, this, &MyFreeFlyScript::sprintEnd);

        GPE::Wave testSound("./resources/sounds/RickRoll.wav", "RICKROLL");
        GPE::Wave testSound2("./resources/sounds/YMCA.wav", "YMCA");
        GPE::Wave testSound3("./resources/sounds/E_Western.wav", "Western");

        GPE::SourceSettings sourceSettings;
        sourceSettings.pitch = 1;
        sourceSettings.loop  = AL_TRUE;

        source.setSound("Western", "Western", sourceSettings);
        source.playSound("Western");
    }

    MyFreeFlyScript() noexcept                             = delete;
    MyFreeFlyScript(const MyFreeFlyScript& other) noexcept = delete;
    MyFreeFlyScript(MyFreeFlyScript&& other) noexcept      = default;
    virtual ~MyFreeFlyScript() noexcept                    = default;
    MyFreeFlyScript& operator=(MyFreeFlyScript const& other) noexcept = delete;
    MyFreeFlyScript& operator=(MyFreeFlyScript&& other) noexcept = delete;

    GPE::InputComponent& input;
    GPE::AudioComponent& source;
    float                speed = 1;

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
        GPE::TransformComponent& transform = getOwner().getTransform();
        transform.translate(transform.getVectorUp() * speed);
    }

    inline void down()
    {
        GPE::TransformComponent& transform = getOwner().getTransform();
        transform.translate(transform.getVectorUp() * -speed);
    }

    inline void forward()
    {
        GPE::TransformComponent& transform = getOwner().getTransform();
        transform.translate(transform.getVectorForward() * -speed);
    }

    inline void back()
    {
        GPE::TransformComponent& transform = getOwner().getTransform();
        transform.translate(transform.getVectorForward() * speed);
    }

    inline void left()
    {
        GPE::TransformComponent& transform = getOwner().getTransform();
        transform.translate(transform.getVectorRight() * -speed);
    }

    inline void right()
    {
        GPE::TransformComponent& transform = getOwner().getTransform();
        transform.translate(transform.getVectorRight() * speed);
    }

    inline void leave()
    {
        exit(666);
    }

    inline void sprintStart()
    {
        speed *= 2.f;
    }

    inline void sprintEnd()
    {
        speed /= 2.f;
    }

    void update(float deltaTime) final
    {
        rotate(GPE::Engine::getInstance()->inputManager.getCursor().deltaPos);
    }
};
} /*namespace GPG*/