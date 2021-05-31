#include <Engine/Core/Tools/Raycast.hpp>
#include <Engine/ECS/Component/ParticleComponent.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/ParticleSystem/ParticleGenerator.hpp>
#include <Engine/Resources/Wave.hpp>

#include <BaseCharacter.hpp>
#include <PhysX/PxRigidActor.h>

#include <Firearm/Firearm.hpp>
File_GENERATED

    using namespace GPG;
using namespace GPE;

Firearm::Firearm(GPE::GameObject& owner) noexcept : GPE::BehaviourComponent(owner)
{
    onPostLoad();
}

Firearm::Firearm(GPE::GameObject& owner, const GunMagazine& magazineStored, float reloadingDuration,
                 float rateOfFire) noexcept
    : GPE::BehaviourComponent(owner), m_magazineStored{magazineStored}, m_rateOfFire{rateOfFire},
      m_reloadingBulletTimeCount{rateOfFire}, m_reloadingDuration{reloadingDuration}
{
    onPostLoad();
}

void Firearm::onPostLoad()
{
    BehaviourComponent::onPostLoad();

    enableUpdate(true);

    m_shootSound = &getOwner().getOrCreateComponent<GPE::AudioComponent>();

    GPE::Wave           sound("./resources/sounds/Firearm/machinegun.wav", "Shoot");
    GPE::SourceSettings sourceSettings;
    sourceSettings.pitch = 1.f;
    sourceSettings.loop  = AL_FALSE;

    m_shootSound->setSound("Shoot", "Shoot", sourceSettings);
}

void Firearm::start()
{
    m_recoileAnimtationDuration = std::clamp(m_recoileAnimtationDuration, 0.f, m_rateOfFire);

    GAME_ASSERT(m_muzzleFlashEffect.pData, "Missing component");
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
        ray.fire(getOwner().getTransform().getGlobalPosition() + getOwner().getTransform().getVectorForward() * 10.f,
                 getOwner().getTransform().getVectorForward(), 100000.f);

        if (ray.hit.hasBlock)
        {
            const GPM::Vec3 rayPos  = GPE::PhysXSystem::PxVec3ToGPMVec3(ray.hit.block.position);
            const GPM::Vec3 rayNorm = GPE::PhysXSystem::PxVec3ToGPMVec3(ray.hit.block.normal);

            if (GPE::GameObject* pOwner = static_cast<GPE::GameObject*>(ray.hit.block.actor->userData))
            {

                if (m_decalePrefab)
                {
                    GPE::GameObject& decaleGO = *m_decalePrefab->clone(*pOwner);
                    decaleGO.getTransform().setTranslation(rayPos);

                    decaleGO.getTransform().setVecForward(rayNorm, (GPM::Vec3::right().cross(rayNorm).normalized()));
                }

                if (pOwner->getTag() == "Character")
                {
                    BaseCharacter* const bc = pOwner->getComponent<BaseCharacter>();

                    GAME_ASSERT(bc, "null");

                    bc->takeDamage(m_magazineStored.bulletData.getDammage());
                    // Here vvv
                }
                else
                {
                    if (m_groundShootEffect.pData)
                    {
                        m_groundShootEffect.pData->emitAt(
                            GPM::Transform::lookAt(rayPos, rayPos + rayNorm),
                            static_cast<unsigned int>(m_groundShootEffect.pData->getCount() /
                                                      m_magazineStored.getCapacity()));
                    }
                }
            }

            // TODO: place here ^^^
            if (m_bloodEffect.pData)
            {
                m_bloodEffect.pData->emitAt(
                    GPM::Transform::lookAt(rayPos, rayPos + getOwner().getTransform().getVectorForward()),
                    static_cast<unsigned int>(m_bloodEffect.pData->getCount() / m_magazineStored.getCapacity()));
            }

            getOwner().pOwnerScene->sceneRenderer.drawDebugSphere(rayPos, 1.f, GPE::ColorRGBA::red(), 3.f);
        }

        m_isRecoileAnimate = true;
        onShoot();
        m_muzzleFlashEffect.pData->emit(
            static_cast<unsigned int>(m_muzzleFlashEffect.pData->getCount() / m_magazineStored.getCapacity()));

        m_shootSound->playSound("Shoot", true);
        m_isReloadingNextBullet    = true;
        m_reloadingBulletTimeCount = 0.f;
    }
}

void Firearm::update(double deltaTime)
{
    m_isReloadingNextBullet =
        (m_reloadingBulletTimeCount += (float(deltaTime) * m_isReloadingNextBullet)) < m_rateOfFire;

    if (m_isRecoileAnimate)
    {
        m_recoileAnimtationDurationCount += float(deltaTime);

        const float t = std::clamp(m_recoileAnimtationDurationCount / m_recoileAnimtationDuration, 0.f, 1.f);
        animateRecoil(t);

        if (m_recoileAnimtationDurationCount >= m_recoileAnimtationDuration)
        {
            m_isRecoileAnimate               = false;
            m_recoileAnimtationDurationCount = 0.f;
        }
    }

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

void Firearm::onShoot()
{
}

void Firearm::animateRecoil(float t)
{
}

void Firearm::reload()
{
    m_isReloading = true;
}

const GunMagazine& Firearm::getMagazine() const
{
    return m_magazineStored;
}