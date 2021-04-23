/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Engine.hpp"

#include "Engine/Resources/Script/FreeFly.hpp"

#include "Generated/FreeFly.rfk.h"
File_GENERATED

namespace GPE
{

FreeFly::FreeFly(GameObject& owner) noexcept
    : BehaviourComponent(owner)
{
    enableUpdate(true);
}


FreeFly::~FreeFly() noexcept
{
    DataChunk<FreeFly>::getInstance()->destroy(this);
}


void FreeFly::update(float deltaTime)
{
    rotate(Engine::getInstance()->inputManager.getCursor().deltaPos);
}


void FreeFly::rotate(const GPM::Vector2& deltaDisplacement)
{
    const GPM::Quaternion newRot =
        getOwner().getTransform().getSpacialAttribut().rotation *
        GPM::Quaternion::angleAxis(deltaDisplacement.y * m_rotationSpeed, {1.f, .0f, .0f});

    getOwner().getTransform().setRotation(
        GPM::Quaternion::angleAxis(deltaDisplacement.x * m_rotationSpeed, {.0f, 1.f, .0f}) * newRot);
}


FreeFly& FreeFly::operator=(FreeFly&& other) noexcept
{
    m_speed         = std::move(other.m_speed);
    m_rotationSpeed = std::move(other.m_rotationSpeed);

    return static_cast<FreeFly&>(BehaviourComponent::operator=(std::move(other)));
}

} // namespace GPE
