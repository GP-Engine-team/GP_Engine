/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Serialization/Inspect.hpp"
#include <Engine/Serialization/DataInspector.hpp>

#include "Engine/Serialization/Serialize.hpp"
#include "Engine/Serialization/xml/xmlLoader.hpp"
#include "Engine/Serialization/xml/xmlSaver.hpp"

#include <FireArme/GunMagazine.hpp>

#include "Generated/FireArme.rfk.h"

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), Serialize()) FireArme
    {
    protected:
        RFKField(Inspect()) GunMagazine m_magazineStored;

        RFKField(Inspect()) float m_rateOfFire               = 0.f; // In second
        RFKField(Inspect()) float m_reloadingBulletTimeCount = 0.f; // In second

        RFKField(Inspect()) float m_reloadingDuration  = 0.f; // In second
        RFKField(Inspect()) float m_reloadingTimeCount = 0.f; // In second

        RFKField(Inspect()) bool m_isReloadingNextBullet = false;
        RFKField(Inspect()) bool m_isReloading           = false;

    public:
        FireArme(const GunMagazine& magazineStored, float reloadingDuration, float rateOfFire) noexcept
            : m_magazineStored{magazineStored}, m_reloadingDuration{reloadingDuration}, m_rateOfFire{rateOfFire}
        {
            m_reloadingBulletTimeCount = m_rateOfFire;
        }

        bool isMagazineEmpty()
        {
            return m_magazineStored.isEmpty();
        }

        void triggered()
        {
            if (!m_isReloadingNextBullet && !m_isReloading)
            {
                m_magazineStored.triggeredBullet();
                m_isReloadingNextBullet    = true;
                m_reloadingBulletTimeCount = 0.f;
            }
        }

        void update(double deltaTime)
        {
            m_isReloadingNextBullet =
                (m_reloadingBulletTimeCount += (deltaTime * m_isReloadingNextBullet)) < m_rateOfFire;

            if (m_isReloading)
            {
                m_reloadingTimeCount += deltaTime * m_isReloading;
                if (m_reloadingTimeCount >= m_reloadingDuration)
                {
                    m_isReloading        = false;
                    m_reloadingTimeCount = 0.f;
                    m_magazineStored.reload();
                }
            }
        }

        void reload()
        {
            m_isReloading = true;
        }

        const GunMagazine& getMagazine()
        {
            return m_magazineStored;
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