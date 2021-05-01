/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Serialization/Inspect.hpp"
#include "Engine/Serialization/Serialize.hpp"

#include <FireArme/Bullet.hpp>

#include "Generated/GunMagazine.rfk.h"

namespace GPG RFKNamespace()
{

    class RFKClass(Inspect(), Serialize()) GunMagazine
    {
    protected:
        RFKField(Inspect(), Serialize()) Bullet m_bulletStored;

        RFKField(Inspect(), Serialize()) unsigned int m_magazineCapacity = 0;
        RFKField(Inspect(), Serialize()) unsigned int m_bulletsRemaining = 0;

    public:
        GunMagazine(const Bullet& bulletStored, unsigned int magazineCapacity, unsigned int bulletsRemaining) noexcept
            : m_bulletStored{bulletStored}, m_magazineCapacity{magazineCapacity}, m_bulletsRemaining{bulletsRemaining}
        {
        }

        void triggeredBullet()
        {
            if (m_bulletsRemaining)
            {
                --m_bulletsRemaining;
                m_bulletStored.triggered();
            }
        }

        void reload()
        {
            m_bulletsRemaining = m_magazineCapacity;
        }

        bool isEmpty()
        {
            return m_bulletsRemaining == 0;
        }

        GunMagazine() noexcept                         = default;
        GunMagazine(const GunMagazine& other) noexcept = default;
        GunMagazine(GunMagazine && other) noexcept     = default;
        virtual ~GunMagazine() noexcept                = default;
        GunMagazine& operator=(GunMagazine const& other) noexcept = default;
        GunMagazine& operator=(GunMagazine&& other) noexcept = default;

        GunMagazine_GENERATED
    };
} // namespace )
