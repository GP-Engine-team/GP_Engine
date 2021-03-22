﻿/*
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

        input.bindAction("up", EKeyMode::KEY_DOWN, this, &FreeFly::up);
        input.bindAction("down", EKeyMode::KEY_DOWN, this, &FreeFly::down);
        input.bindAction("right", EKeyMode::KEY_DOWN, this, &FreeFly::right);
        input.bindAction("left", EKeyMode::KEY_DOWN, this, &FreeFly::left);
        input.bindAction("forward", EKeyMode::KEY_DOWN, this, &FreeFly::forward);
        input.bindAction("back", EKeyMode::KEY_DOWN, this, &FreeFly::back);
        input.bindAction("sprint", EKeyMode::KEY_DOWN, this, &FreeFly::sprint);
    }

    void rotate(const GPM::Vec2& deltaDisplacement)
    {
        if (deltaDisplacement.length() > 0.4)
        {
            m_gameObject.getTransform().setRotation(
                m_gameObject.getTransform().getSpacialAttribut().rotation *
                GPM::Quaternion::angleAxis(-deltaDisplacement.y * m_rotationSpeed, {1, 0, 0}));
            m_gameObject.getTransform().setRotation(
                GPM::Quaternion::angleAxis(-deltaDisplacement.x * m_rotationSpeed, {0, 1, 0}) *
                m_gameObject.getTransform().getSpacialAttribut().rotation);
        }
    }

    inline void up()
    {
        m_gameObject.getTransform().translate(m_gameObject.getTransform().getVectorUp() * m_speed);
    }

    inline void down()
    {
        m_gameObject.getTransform().translate(m_gameObject.getTransform().getVectorUp() * -m_speed);
    }

    inline void forward()
    {
        m_gameObject.getTransform().translate(m_gameObject.getTransform().getVectorForward() * -m_speed);
    }

    inline void back()
    {
        m_gameObject.getTransform().translate(m_gameObject.getTransform().getVectorForward() * m_speed);
    }

    inline void left()
    {
        m_gameObject.getTransform().translate(m_gameObject.getTransform().getVectorRight() * -m_speed);
    }

    inline void right()
    {
        m_gameObject.getTransform().translate(m_gameObject.getTransform().getVectorRight() * m_speed);
    }

    inline void sprint()
    {
        m_speed = 2;
    }

    void update(float deltaTime) final
    {
        m_speed = 1;
        rotate(Engine::getInstance()->inputManager.getCursor().deltaPos);

        if (m_gameObject.getTransform().isDirty())
            m_gameObject.getTransform().update(GPM::Mat4::identity());
    }
};
} // namespace GPE
