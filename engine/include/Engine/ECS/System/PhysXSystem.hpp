/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Debug/Log.hpp>
#include <PxPhysics.h>
#include <PxScene.h>
#include <PxSceneDesc.h>
#include <Pxfoundation.h>
#include <cooking/PxCooking.h>
#include <foundation/PxErrorCallback.h>
#include <pvd/PxPvd.h>

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

private:
    physx::PxFoundation* m_Foundation;
    physx::PxPvd*        m_Pvd;
    physx::PxPhysics*    m_Physics;
    physx::PxCooking*    m_Cooking;
    physx::PxScene*      m_Scene;
};

} // namespace GPE
