/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Script/FreeFly.hpp"

#include "Engine/ECS/Component/InputComponent.hpp"
#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/Engine.hpp"

#include <GLFW/glfw3.h>

namespace GPE
{

FreeFly::FreeFly(GameObject& owner) noexcept
    : BehaviourComponent(owner)
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


FreeFly::~FreeFly() noexcept
{
    // TODO: unbind actions
    // InputComponent& input = owner.addComponent<InputComponent>();
    // input.unbindAction("up");
    // input.unbindAction("down");
    // input.unbindAction("right");
    // input.unbindAction("left");
    // input.unbindAction("forward");
    // input.unbindAction("backward");
    // input.unbindAction("sprint");

    DataChunk<FreeFly>::getInstance()->destroy(this);
}


void FreeFly::rotate(const GPM::Vector2& deltaDisplacement)
{
    if (deltaDisplacement.sqrLength() > .16f)
    {
        const GPM::Quaternion newRot =
            getOwner().getTransform().getSpacialAttribut().rotation *
            GPM::Quaternion::angleAxis(deltaDisplacement.y * m_rotationSpeed, {1.f, .0f, .0f});
        
        getOwner().getTransform().setRotation(
            GPM::Quaternion::angleAxis(deltaDisplacement.x * m_rotationSpeed, {.0f, 1.f, .0f}) *
                                       newRot);
    }
}


void FreeFly::update(float deltaTime)
{
    m_speed = 1.f;

    if (Engine::getInstance()->inputManager.getCursor().deltaPos.sqrLength() > .1f)
    {
        rotate(Engine::getInstance()->inputManager.getCursor().deltaPos);
    }
}


FreeFly& FreeFly::operator=(FreeFly&& other) noexcept
{
    m_speed         = std::move(other.m_speed);
    m_rotationSpeed = std::move(other.m_rotationSpeed);

    return static_cast<FreeFly&>(BehaviourComponent::operator=(std::move(other)));
}

} // namespace GPE
