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
        RFKField(Serialize(), Inspect()) Bullet m_bulletStored;

        RFKField(Serialize(), Inspect()) unsigned int m_magazineCapacity = 0;
        RFKField(Serialize(), Inspect()) unsigned int m_bulletsRemaining = 0;

    public:
        GunMagazine(const Bullet& bulletStored, unsigned int magazineCapacity, unsigned int bulletsRemaining) noexcept;

        void triggeredBullet();

        GETTER_BY_CONST_REF(Capacity, m_magazineCapacity)
        GETTER_BY_CONST_REF(BulletsRemaining, m_bulletsRemaining)

        void reload();

        bool isEmpty() const;

        GunMagazine() noexcept                         = default;
        GunMagazine(const GunMagazine& other) noexcept = default;
        GunMagazine(GunMagazine && other) noexcept     = default;
        virtual ~GunMagazine() noexcept                = default;
        GunMagazine& operator=(GunMagazine const& other) noexcept = default;
        GunMagazine& operator=(GunMagazine&& other) noexcept = default;

        GunMagazine_GENERATED
    };
} // namespace )
