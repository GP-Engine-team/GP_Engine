/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Serialization/Inspect.hpp"
#include "Engine/Serialization/Serialize.hpp"

#include <FireArme/FireArme.hpp>

#include "Generated/PPSH41.rfk.h"

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), Serialize()) PPSH41 : public FireArme
    {
    public:
        PPSH41() noexcept : FireArme(GunMagazine(Bullet(5.f), 30, 30), 3.f)
        {
        }

        PPSH41(const PPSH41& other) noexcept = delete;
        PPSH41(PPSH41 && other) noexcept     = default;
        virtual ~PPSH41() noexcept           = default;
        PPSH41& operator=(PPSH41 const& other) noexcept = delete;
        PPSH41& operator=(PPSH41&& other) noexcept = delete;

        PPSH41_GENERATED
    };
} // namespace )
