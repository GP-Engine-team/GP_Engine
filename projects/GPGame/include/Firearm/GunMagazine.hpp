/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>

#include "Bullet.hpp"
#include <Engine/Core/Tools/ClassUtility.hpp>

#include <Generated/GunMagazine.rfk.h>

namespace GPG RFKNamespace()
{

    class RFKClass(Inspect(), Serialize()) GunMagazine
    {
    protected:
        RFKField(Inspect(), Serialize()) unsigned int m_magazineCapacity = 0;
        RFKField(Inspect(), Serialize()) unsigned int m_bulletsRemaining = 0;

    public:
        RFKField(Inspect(), Serialize()) Bullet bulletData;

    public:
        GunMagazine(const Bullet& bulletStored, unsigned int magazineCapacity, unsigned int bulletsRemaining) noexcept;
        GunMagazine() = default;

        void triggeredBullet();

        GETTER_BY_CONST_REF(Capacity, m_magazineCapacity)
        GETTER_BY_CONST_REF(BulletsRemaining, m_bulletsRemaining)

        void reload();

        bool isEmpty() const;

        GunMagazine_GENERATED
    };
} // namespace )

File_GENERATED