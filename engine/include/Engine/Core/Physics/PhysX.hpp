/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include "Engine/Core/Debug/Log.hpp"
#include "PxFoundation.h"
#include "foundation/PxErrorCallback.h"
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

class PhysX
{
public:
    PhysX();
    ~PhysX();

private:
    physx::PxFoundation* m_foundation;
};

} // namespace GPE
