/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyStatic.hpp>
#include <PxPhysics.h>
#include <PxScene.h>
#include <PxSceneDesc.h>
#include <Pxfoundation.h>
#include <cooking/PxCooking.h>
#include <foundation/PxErrorCallback.h>
#include <pvd/PxPvd.h>
#include <vector>

namespace GPE
{
class UserErrorCallback : public physx::PxErrorCallback
{
public:
    virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
    {
        FUNCT_ERROR(message);
    }
};

class PhysXSystem
{
public:
    PhysXSystem();
    ~PhysXSystem();

    void advance(const double& deltaTime) noexcept;

    void drawDebugScene();

    /**
     * @brief add Collision component to the component list
     * @param rigidbody
     * @return
     */
    inline size_t addComponent(RigidbodyStatic* rigidbody) noexcept;

    /**
     * @brief remove Collision component to the component list
     * @param rigidbody
     * @return
     */
    inline void removeComponent(RigidbodyStatic* rigidbody) noexcept;

public:
    physx::PxFoundation*          foundation;
    physx::PxPvd*                 pvd;
    physx::PxPhysics*             physics;
    physx::PxCooking*             cooking;
    physx::PxScene*               scene;
    std::vector<RigidbodyStatic*> rigidbodyStatics;
};

#include <Engine/ECS/System/PhysXSystem.inl>

} // namespace GPE
