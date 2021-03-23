/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/BehaviourComponent.hpp"
#include "Engine/ECS/Component/InputComponent.hpp"
#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"

#include <iostream>

namespace GPG
{
class MyScript : public GPE::BehaviourComponent
{
public:
    inline MyScript(GPE::GameObject& owner) noexcept
        : GPE::BehaviourComponent(owner), input(owner.addComponent<GPE::InputComponent>())
    {
        enableUpdate(true);
        input.bindAction("jump", EKeyMode::KEY_DOWN, this, &MyScript::up);
        input.bindAction("down", EKeyMode::KEY_DOWN, this, &MyScript::down);
        input.bindAction("right", EKeyMode::KEY_DOWN, this, &MyScript::right);
        input.bindAction("left", EKeyMode::KEY_DOWN, this, &MyScript::left);
        input.bindAction("forward", EKeyMode::KEY_DOWN, this, &MyScript::forward);
        input.bindAction("back", EKeyMode::KEY_DOWN, this, &MyScript::back);
        input.bindAction("exit", EKeyMode::KEY_DOWN, this, &MyScript::leave);
        input.bindAction("sprint", EKeyMode::KEY_DOWN, this, &MyScript::sprint);

        speed = 1;
    }

    MyScript() noexcept                      = delete;
    MyScript(const MyScript& other) noexcept = delete;
    MyScript(MyScript&& other) noexcept      = default;
    virtual ~MyScript() noexcept             = default;
    MyScript& operator=(MyScript const& other) noexcept = delete;
    MyScript& operator=(MyScript&& other) noexcept = delete;

    GPE::InputComponent& input;
    float                speed;

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

    inline void sprint()
    {
        speed = 2;
    }

    void update(float deltaTime) final
    {
        speed = 1;
        rotate(GPE::Engine::getInstance()->inputManager.getCursor().deltaPos);
    }
};
} /*namespace GPG*/
