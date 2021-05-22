#include <Engine/Core/Tools/Raycast.hpp>
#include <Engine/ECS/Component/ParticleComponent.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>

#include <PhysX/PxRigidActor.h>

#include <gpm/DebugOutput.hpp>

#include <Firearm/Firearm.hpp>
File_GENERATED

    namespace GPG
{

    Firearm::Firearm(GPE::GameObject & owner) noexcept : GPE::BehaviourComponent(owner)
    {
    }

    Firearm::Firearm(GPE::GameObject & owner, const GunMagazine& magazineStored, float reloadingDuration,
                     float rateOfFire) noexcept
        : GPE::BehaviourComponent(owner), m_magazineStored{magazineStored},
          m_shootSound{&owner.addComponent<GPE::AudioComponent>()},
          m_muzzleFlash{&owner.addComponent<GPE::ParticleComponent>()}, m_rateOfFire{rateOfFire},
          m_reloadingBulletTimeCount{rateOfFire}, m_reloadingDuration{reloadingDuration}
    {
        enableUpdate(true);

        m_muzzleFlash->setDuration(.5f);

        GPE::Wave           sound("./resources/sounds/Firearm/machinegun.wav", "Shoot");
        GPE::SourceSettings sourceSettings;
        sourceSettings.pitch = 1.f;
        sourceSettings.loop  = AL_FALSE;

        m_shootSound->setSound("Shoot", "Shoot", sourceSettings);
    }

    void Firearm::start()
    {
        // GAME_ASSERT(m_decalePrefab, "Missing prefab");
    }

    bool Firearm::isMagazineEmpty() const
    {
        return m_magazineStored.isEmpty();
    }

    void Firearm::triggered()
    {
        if (!m_isReloadingNextBullet && !m_isReloading)
        {
            m_magazineStored.triggeredBullet();

            GPE::Raycast ray;
            ray.fire(getOwner().getTransform().getGlobalPosition() +
                         getOwner().getTransform().getVectorForward() * 10.f,
                     getOwner().getTransform().getVectorForward(), 100000.f);

            if (ray.hit.hasBlock)
            {
                if (GPE::GameObject* pOwner = static_cast<GPE::GameObject*>(ray.hit.block.actor->userData))
                {
                    if (m_decalePrefab)
                    {
                        GPE::GameObject& decaleGO = *m_decalePrefab->clone(*pOwner);
                        decaleGO.getTransform().setTranslation(
                            GPE::PhysXSystem::PxVec3ToGPMVec3(ray.hit.block.position));

                        decaleGO.getTransform().setVecForward(
                            GPE::PhysXSystem::PxVec3ToGPMVec3(ray.hit.block.normal),
                            (GPM::Vec3::right()
                                 .cross(GPE::PhysXSystem::PxVec3ToGPMVec3(ray.hit.block.normal))
                                 .normalized()));
                    }

                    getOwner().pOwnerScene->sceneRenderer.drawDebugSphere(
                        GPE::PhysXSystem::PxVec3ToGPMVec3(ray.hit.block.position), 1.f, GPE::ColorRGBA::red(), 3.f);
                }
            }

            m_muzzleFlash->emit(static_cast<unsigned int>(m_muzzleFlash->getCount() / m_magazineStored.getCapacity()));

            m_shootSound->playSound("Shoot", true);
            m_isReloadingNextBullet    = true;
            m_reloadingBulletTimeCount = 0.f;
        }
    }

    void Firearm::update(double deltaTime)
    {
        m_isReloadingNextBullet =
            (m_reloadingBulletTimeCount += (float(deltaTime) * m_isReloadingNextBullet)) < m_rateOfFire;

        if (m_isReloading)
        {
            m_reloadingTimeCount += float(deltaTime) * m_isReloading;
            if (m_reloadingTimeCount >= m_reloadingDuration)
            {
                m_isReloading        = false;
                m_reloadingTimeCount = 0.f;
                m_magazineStored.reload();
            }
        }
    }

    void Firearm::reload()
    {
        m_isReloading = true;
    }

    const GunMagazine& Firearm::getMagazine() const
    {
        return m_magazineStored;
    }

    void Firearm::onPostLoad()
    {
        BehaviourComponent::onPostLoad();

        GPE::Wave           sound("./resources/sounds/Firearm/machinegun.wav", "Shoot");
        GPE::SourceSettings sourceSettings;
        sourceSettings.pitch = 1.f;
        sourceSettings.loop  = AL_FALSE;

        m_shootSound->setSound("Shoot", "Shoot", sourceSettings);
    }

} // End of namespace GPG