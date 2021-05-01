/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Serialization/Inspect.hpp"
#include "Engine/Serialization/Serialize.hpp"

#include <FireArme/FireArme.hpp>

#include "Generated/FuturistWeapon.rfk.h"

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), Serialize()) FuturistWeapon : public FireArme
    {
    public:
        FuturistWeapon() noexcept : FireArme(GunMagazine(Bullet(5.f), 30, 30), 3.f)
        {
        }

        FuturistWeapon(const FuturistWeapon& other) noexcept = delete;
        FuturistWeapon(FuturistWeapon && other) noexcept     = default;
        virtual ~FuturistWeapon() noexcept                   = default;
        FuturistWeapon& operator=(FuturistWeapon const& other) noexcept = delete;
        FuturistWeapon& operator=(FuturistWeapon&& other) noexcept = delete;

        FuturistWeapon_GENERATED
    };
} // namespace )
