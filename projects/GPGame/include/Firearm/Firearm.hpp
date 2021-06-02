/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>

#include <Engine/ECS/Component/ParticleComponent.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Resources/Prefab.hpp>
#include <Firearm/GunMagazine.hpp>

#include <Generated/Firearm.rfk.h>

namespace GPE
{
class AudioComponent;
class GameObject;
} // namespace GPE

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen(), Serialize()) Firearm : public GPE::BehaviourComponent
    {
    protected:
        RFKField(Inspect(), Serialize()) GunMagazine m_magazineStored;

        RFKField(Serialize()) GPE::AudioComponent*                           m_shootSound = nullptr;
        RFKField(Inspect(), Serialize()) GPE::Linker<GPE::ParticleComponent> m_muzzleFlashEffect;
        RFKField(Inspect(), Serialize()) GPE::Linker<GPE::ParticleComponent> m_bloodEffect;
        RFKField(Inspect(), Serialize()) GPE::Linker<GPE::ParticleComponent> m_groundShootEffect;

        RFKField(Inspect(), Serialize()) float m_rateOfFire               = 0.f; // In second
        RFKField(Serialize()) float            m_reloadingBulletTimeCount = 0.f; // In second

        RFKField(Inspect(), Serialize()) float m_reloadingDuration              = 0.f; // In second
        RFKField(Serialize()) float            m_reloadingTimeCount             = 0.f; // In second
        RFKField(Serialize()) float            m_recoileAnimtationDurationCount = 0.f; // In second
        RFKField(Inspect(), Serialize()) float m_recoileAnimtationDuration      = 0.f; // In second

        RFKField(Serialize()) bool m_isReloadingNextBullet = false;
        RFKField(Serialize()) bool m_isReloading           = false;
        RFKField(Serialize()) bool m_isRecoileAnimate      = false;

        /**
         * @brief Position and rotation without aiming
         */
        RFKField() GPM::Vec3       m_basePosition;
        RFKField() GPM::Quaternion m_baseRotation;

        /**
         * @brief Position and rotation with aiming
         */
        RFKField(Inspect(), Serialize()) GPM::Vec3 m_aimPosition;
        RFKField(Inspect(), Serialize()) GPM::Vec3 m_aimEulerRotation;
        RFKField() GPM::Quaternion                 m_aimRotation;
        RFKField(Inspect(), Serialize()) float     m_aimDuration        = 1.f; // In second
        RFKField(Serialize()) float                m_aimTimeCount       = 0.f; // In second
        RFKField(Serialize()) bool                 m_isAiming           = false;
        RFKField(Serialize()) bool                 m_isAimAnimationDone = true;

        RFKField(Inspect()) GPE::Prefab* m_decalePrefab;

    public:
        Firearm() noexcept = default;
        Firearm(GPE::GameObject & owner) noexcept;
        Firearm(GPE::GameObject & owner, const GunMagazine& magazineStored, float reloadingDuration,
                float rateOfFire) noexcept;
        Firearm(const Firearm& other) noexcept = delete;
        Firearm(Firearm && other) noexcept     = delete;
        virtual ~Firearm() noexcept            = default;

        Firearm& operator=(Firearm const& other) noexcept = delete;
        Firearm& operator=(Firearm&& other) noexcept = delete;

        virtual void onPostLoad() override;

        bool isMagazineEmpty() const;
        void triggered();
        void start() override;
        void reload();
        void update(double deltaTime) override;

        virtual void onShoot();
        virtual void animateRecoil(float t);
        virtual void animateAimIn(float t);
        virtual void animateAimOut(float t);
        virtual void setAim(bool flag);

        const GunMagazine& getMagazine() const;

        Firearm_GENERATED
    };
} // namespace )