/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/BehaviourComponent.hpp"
#include "Engine/ECS/Component/InputComponent.hpp"
#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/Intermediate/GameObject.hpp"

#include <iostream>

namespace GPE
{
class FreeFly final : public BehaviourComponent
{
protected:
    float m_speed         = 1.f;
    float m_rotationSpeed = 0.001f;

public:
    FreeFly(GameObject& owner) noexcept : BehaviourComponent(owner)
    {
        enableUpdate(true);
        InputComponent& input = owner.addComponent<InputComponent>();

        input.bindAction("up", EKeyMode::KEY_DOWN, this, "up");
        input.bindAction("down", EKeyMode::KEY_DOWN, this, "down");
        input.bindAction("right", EKeyMode::KEY_DOWN, this, "right");
        input.bindAction("left", EKeyMode::KEY_DOWN, this, "left");
        input.bindAction("forward", EKeyMode::KEY_DOWN, this, "forward");
        input.bindAction("back", EKeyMode::KEY_DOWN, this, "back");
        input.bindAction("sprint", EKeyMode::KEY_DOWN, this, "sprint");
    }

    void rotate(const GPM::Vec2& deltaDisplacement)
    {
        if (deltaDisplacement.length() > 0.4)
        {
            getOwner().getTransform().setRotation(
                getOwner().getTransform().getSpacialAttribut().rotation *
                GPM::Quaternion::angleAxis(-deltaDisplacement.y * m_rotationSpeed, {1, 0, 0}));
            getOwner().getTransform().setRotation(
                GPM::Quaternion::angleAxis(-deltaDisplacement.x * m_rotationSpeed, {0, 1, 0}) *
                getOwner().getTransform().getSpacialAttribut().rotation);
        }
    }

    RFKMethod()
    inline void up()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorUp() * m_speed);
    }

    RFKMethod()
    inline void down()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorUp() * -m_speed);
    }

    RFKMethod()
    inline void forward()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorForward() * -m_speed);
    }

    RFKMethod()
    inline void back()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorForward() * m_speed);
    }

    RFKMethod()
    inline void left()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorRight() * -m_speed);
    }

    RFKMethod()
    inline void right()
    {
        getOwner().getTransform().translate(getOwner().getTransform().getVectorRight() * m_speed);
    }

    RFKMethod()
    inline void sprint()
    {
        m_speed = 2;
    }

    void update(float deltaTime) final;
};
} // namespace GPE
