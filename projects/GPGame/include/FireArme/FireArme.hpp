/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Core/Tools/Raycast.hpp>
#include <Engine/ECS/Component/AudioComponent.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/ParticleComponent.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>
#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>
#include <Engine/Serialization/xml/xmlLoader.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>

#include <FireArme/GunMagazine.hpp>

#include "Generated/FireArme.rfk.h"

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) FireArme : public GPE::BehaviourComponent
    {
    protected:
        RFKField(Inspect()) GunMagazine m_magazineStored;

        RFKField(Inspect()) float m_rateOfFire               = 0.f; // In second
        RFKField(Inspect()) float m_reloadingBulletTimeCount = 0.f; // In second

        RFKField(Inspect()) float m_reloadingDuration  = 0.f; // In second
        RFKField(Inspect()) float m_reloadingTimeCount = 0.f; // In second

        RFKField(Inspect()) bool m_isReloadingNextBullet = false;
        RFKField(Inspect()) bool m_isReloading           = false;

        RFKField(Serialize()) GPE::AudioComponent*    m_shootSound  = nullptr;
        RFKField(Serialize()) GPE::ParticleComponent* m_muzzleFlash = nullptr;

    public:
        FireArme(GPE::GameObject & owner, const GunMagazine& magazineStored, float reloadingDuration,
                 float rateOfFire) noexcept
            : GPE::BehaviourComponent(owner), m_magazineStored{magazineStored}, m_reloadingDuration{reloadingDuration},
              m_rateOfFire{rateOfFire}
        {
            m_reloadingBulletTimeCount = m_rateOfFire;

            m_shootSound  = &owner.addComponent<GPE::AudioComponent>();
            m_muzzleFlash = &owner.addComponent<GPE::ParticleComponent>();
            m_muzzleFlash->setDuration(0.5);
            m_muzzleFlash->generate();

            GPE::Wave           sound("./resources/sounds/FireArme/machinegun.wav", "Shoot");
            GPE::SourceSettings sourceSettings;
            sourceSettings.pitch = 1.f;
            sourceSettings.loop  = AL_FALSE;

            m_shootSound->setSound("Shoot", "Shoot", sourceSettings);
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

                GPE::Raycast ray;
                ray.Fire(getOwner().getTransform().getGlobalPosition(), getOwner().getTransform().getVectorForward(),
                         10000.f);

                m_muzzleFlash->emit(
                    static_cast<unsigned int>(m_muzzleFlash->getCount() / m_magazineStored.getCapacity()));
                /*if (ray.hit.getNbAnyHits())
                    GPE::Log::getInstance()->log(ray.hit.getAnyHit(0).actor->getName());*/
                m_shootSound->playSound("Shoot");
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

        FireArme(GPE::GameObject & owner) noexcept : GPE::BehaviourComponent(owner)
        {
        }

        FireArme() noexcept                      = default;
        FireArme(const FireArme& other) noexcept = delete;
        FireArme(FireArme && other) noexcept     = delete;
        virtual ~FireArme() noexcept             = default;

        FireArme& operator=(FireArme const& other) noexcept = delete;
        FireArme& operator=(FireArme&& other) noexcept = delete;

        FireArme_GENERATED
    };
} // namespace )