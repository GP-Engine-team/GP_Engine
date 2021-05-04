/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Engine.hpp>
#include <GPM/Vector3.hpp>
#include <PxQueryReport.h>

namespace GPE
{
struct Raycast
{
    /**
     * @brief Fire a raycast in the scene
     * @param origin
     * @param direction
     * @param maxDistance
     * @return
     */
    void Fire(const GPM::Vec3& origin, const GPM::Vec3& direction, float maxDistance) noexcept
    {
        GPE::Engine::getInstance()->physXSystem.scene->raycast(
            GPE::PhysXSystem::GPMVec3ToPxVec3(origin), GPE::PhysXSystem::GPMVec3ToPxVec3(direction.normalized()),
            maxDistance, hit);
    }

    physx::PxRaycastBuffer& hit;
};
} // namespace GPE