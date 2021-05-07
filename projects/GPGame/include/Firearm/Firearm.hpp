﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>

#include <Firearm/GunMagazine.hpp>

#include <Generated/Firearm.rfk.h>

namespace GPE
{
class AudioComponent;
class ParticleComponent;
class GameObject;
}

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) Firearm : public GPE::BehaviourComponent
    {
    protected:
        RFKField(Inspect()) GunMagazine m_magazineStored;

        RFKField(Serialize()) GPE::AudioComponent*    m_shootSound  = nullptr;
        RFKField(Serialize()) GPE::ParticleComponent* m_muzzleFlash = nullptr;

        RFKField(Inspect()) float m_rateOfFire               = 0.f; // In second
        RFKField(Inspect()) float m_reloadingBulletTimeCount = 0.f; // In second

        RFKField(Inspect()) float m_reloadingDuration  = 0.f; // In second
        RFKField(Inspect()) float m_reloadingTimeCount = 0.f; // In second

        RFKField(Inspect()) bool m_isReloadingNextBullet = false;
        RFKField(Inspect()) bool m_isReloading           = false;


    public:
        Firearm()                                     noexcept = default;
        Firearm(GPE::GameObject& owner)               noexcept;
        Firearm(GPE::GameObject&   owner,
                const GunMagazine& magazineStored,
                float              reloadingDuration,
                float              rateOfFire)        noexcept;
        Firearm(const Firearm& other)                 noexcept = delete;
        Firearm(Firearm&& other)                      noexcept = delete;
        virtual ~Firearm()                            noexcept = default;
        
        Firearm& operator=(Firearm const& other) noexcept = delete;
        Firearm& operator=(Firearm&& other)      noexcept = delete;

        bool isMagazineEmpty() const;
        void triggered      ();
        void reload         ();
        void update         (double deltaTime) final;

        const GunMagazine& getMagazine() const;

        Firearm_GENERATED
    };
} // namespace )