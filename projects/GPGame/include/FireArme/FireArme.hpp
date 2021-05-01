/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Serialization/Inspect.hpp"
#include "Engine/Serialization/Serialize.hpp"

#include <FireArme/GunMagazine.hpp>

#include "Generated/FireArme.rfk.h"

namespace GPG RFKNamespace()
{

    class RFKClass(Inspect(), Serialize()) FireArme
    {
    protected:
        RFKField(Inspect(), Serialize()) GunMagazine m_magazineStored;

        RFKField(Inspect(), Serialize()) float m_reloadingDuration  = 0.f; // In second
        RFKField(Inspect(), Serialize()) float m_reloadingTimeCount = 0.f; // In second
        RFKField(Inspect(), Serialize()) bool  m_isReloading        = false;

    public:
        FireArme(const GunMagazine& magazineStored, float reloadingDuration) noexcept
            : m_magazineStored{magazineStored}, m_reloadingDuration{reloadingDuration}
        {
        }

        bool isMagazineEmpty()
        {
            return m_magazineStored.isEmpty();
        }

        void triggered()
        {
            m_magazineStored.triggeredBullet();
        }

        void reload(double deltaTime)
        {
            if (m_isReloading)
            {
                m_reloadingTimeCount += deltaTime;
                m_isReloading = m_reloadingTimeCount >= m_reloadingDuration;
            }
        }

        FireArme() noexcept                      = default;
        FireArme(const FireArme& other) noexcept = delete;
        FireArme(FireArme && other) noexcept     = default;
        virtual ~FireArme() noexcept             = default;
        FireArme& operator=(FireArme const& other) noexcept = delete;
        FireArme& operator=(FireArme&& other) noexcept = delete;

        FireArme_GENERATED
    };
} // namespace )
