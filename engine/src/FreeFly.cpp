/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Engine.hpp>

#include <Engine/Resources/Script/FreeFly.hpp>
#include <Generated/FreeFly.rfk.h>
File_GENERATED

using namespace GPE;

FreeFly::FreeFly(GameObject & owner) noexcept
    : BehaviourComponent(owner), timeSys{Engine::getInstance()->timeSystem}
{
}

FreeFly::~FreeFly() noexcept
{
    DataChunk<FreeFly>::getInstance()->destroy(this);
}

void FreeFly::update(double deltaTime)
{
    const GPM::Vec2 deltaPos = Engine::getInstance()->inputManager.getCursor().deltaPos;

    if (deltaPos.x || deltaPos.y)
    {
        rotate(deltaPos);
    }
}

// TODO: the rotation should depend on deltaTime
void FreeFly::rotate(const GPM::Vector2& deltaDisplacement)
{
    using namespace GPM;

    const Quat& orientation{getOwner().getTransform().getSpacialAttribut().rotation};
    const Vec2  axis{deltaDisplacement.rotated90()};
    const Quat  rotX{Quat::angleAxis(axis.x * m_rotationSpeed, Vec3::right())};
    const Quat  rotY{Quat::angleAxis(axis.y * m_rotationSpeed, Vec3::up())};
    const Quat  newRot{rotY * orientation * rotX};

    getOwner().getTransform().setRotation(newRot);
}