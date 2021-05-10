/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <PhysX/PxQueryReport.h>

namespace GPM
{
union Vector3;
}

namespace GPE
{

struct Raycast
{
    physx::PxRaycastBuffer hit;

    /**
     * @brief Fire a raycast in the scene
     * @param origin
     * @param direction
     * @param maxDistance
     * @return
     */
    void fire(const GPM::Vector3& origin, const GPM::Vector3& direction, float maxDistance) noexcept;

};

} // namespace GPE