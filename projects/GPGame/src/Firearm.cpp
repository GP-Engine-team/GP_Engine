#include <Engine/Core/Tools/Interpolation.hpp>
#include <Engine/Core/Tools/Raycast.hpp>
#include <Engine/ECS/Component/ParticleComponent.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/ParticleSystem/ParticleGenerator.hpp>
#include <Engine/Resources/Wave.hpp>
#include <gpm/Calc.hpp>

#include <BaseCharacter.hpp>
#include <PhysX/PxRigidActor.h>

#include <Firearm/Firearm.hpp>
File_GENERATED

    using namespace GPG;
using namespace GPE;
using namespace GPM;

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
    enableUpdate(true);

    m_shootSound = &getOwner().getOrCreateComponent<GPE::AudioComponent>();

    GPE::Wave           sound("./resources/sounds/Firearm/machinegun.wav", "Shoot");
    GPE::SourceSettings sourceSettings;
    sourceSettings.pitch    = 1.f;
    sourceSettings.loop     = AL_FALSE;
    sourceSettings.relative = AL_TRUE;

    m_shootSound->setSound("Shoot", "Shoot", sourceSettings);

    BehaviourComponent::onPostLoad();
}

void Firearm::start()
{
    m_staticPosition = m_dynamicPosition = transform().getPosition();
    m_staticRotation = m_dynamicRotation = transform().getRotation();

    m_recoileAnimtationDuration = std::clamp(m_recoileAnimtationDuration, 0.f, m_rateOfFire);

    GameObject* playerGO = getOwner().getParent();
    GAME_ASSERT(playerGO, "FireArm must be child of player");
    m_player = playerGO->getComponent<BasePlayer>();
    GAME_ASSERT(m_player, "Missing baseChracter in parent gamObject");

    GAME_ASSERT(m_muzzleFlashEffect.pData, "Missing component");

    m_aimRotation = GPM::Quaternion::fromEuler(m_aimEulerRotation * PI / 180.f);
    // m_aimPosition += m_basePosition;
}

bool Firearm::isMagazineEmpty() const
{
    return m_magazineStored.isEmpty();
}

void Firearm::triggered()
{
    if (!m_isReloadingNextBullet && !m_isReloading && m_isAimAnimationDone)
    {
        m_magazineStored.triggeredBullet();

        GPE::Raycast ray;
        ray.fire(getOwner().getTransform().getGlobalPosition() +
                     getOwner().getTransform().getVectorForward() * m_rayCastForwardOffset,
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

                std::string tag = pOwner->getTag();

                if (tag == "Character")
                {
                    BaseCharacter* const bc = pOwner->getComponent<BaseCharacter>();

                    GAME_ASSERT(bc, "null");

                    bc->takeDamage(m_magazineStored.bulletData.getDammage());

                    if (m_bloodEffect.pData)
                    {
                        m_bloodEffect.pData->emitAt(
                            GPM::Transform::lookAt(rayPos, rayPos + getOwner().getTransform().getVectorForward()),
                            static_cast<unsigned int>(m_bloodEffect.pData->getCount() /
                                                      m_magazineStored.getCapacity()));
                    }
                    GPE::SourceData* data   = m_player->source->findSource("zombieHurted");
                    data->settings.position = rayPos;
                    m_player->source->updateSource(data);
                    m_player->source->playSound("zombieHurted", true, true);
                }
                else if (tag == "Ground")
                {
                    if (m_groundShootEffect.pData)
                    {
                        m_groundShootEffect.pData->emitAt(
                            GPM::Transform::lookAt(rayPos, rayPos + rayNorm),
                            static_cast<unsigned int>(m_groundShootEffect.pData->getCount() /
                                                      m_magazineStored.getCapacity()));
                    }

                    GPE::SourceData* data   = m_player->source->findSource("groundHurted");
                    data->settings.position = rayPos;
                    m_player->source->updateSource(data);
                    m_player->source->playSound("groundHurted", true, true);
                }

                else if (tag == "Metal")
                {
                    GPE::SourceData* data   = m_player->source->findSource("metalHurted");
                    data->settings.position = rayPos;
                    m_player->source->updateSource(data);
                    m_player->source->playSound("metalHurted", true, true);
                }

                else if (tag == "Wall")
                {
                    GPE::SourceData* data   = m_player->source->findSource("wallHurted");
                    data->settings.position = rayPos;
                    m_player->source->updateSource(data);
                    m_player->source->playSound("wallHurted", true, true);
                }

                else if (tag == "Rock")
                {
                    GPE::SourceData* data   = m_player->source->findSource("rockHurted");
                    data->settings.position = rayPos;
                    m_player->source->updateSource(data);
                    m_player->source->playSound("rockHurted", true, true);
                }

                else if (tag == "Wood")
                {
                    GPE::SourceData* data   = m_player->source->findSource("woodHurted");
                    data->settings.position = rayPos;
                    m_player->source->updateSource(data);
                    m_player->source->playSound("woodHurted", true, true);
                }
            }

            getOwner().pOwnerScene->sceneRenderer.drawDebugSphere(rayPos, 1.f, GPE::ColorRGBA::red(), 3.f);
        }

        m_isRecoileAnimate = true;
        onShoot();
        m_muzzleFlashEffect.pData->emit(
            static_cast<unsigned int>(m_muzzleFlashEffect.pData->getCount() / m_magazineStored.getCapacity()));

        m_shootSound->playSound("Shoot", true, true);
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

    if (m_isAiming)
    {
        if (m_aimTimeCount < m_aimDuration)
        {
            m_aimTimeCount += float(deltaTime);

            const float t = std::clamp(m_aimTimeCount / m_aimDuration, 0.f, 1.f);
            animateAimIn(t);
        }
        else
        {
            m_isAimAnimationDone = true;
        }
    }
    else
    {
        if (m_aimTimeCount > 0.f)
        {
            m_aimTimeCount -= float(deltaTime);

            const float t = std::clamp(m_aimTimeCount / m_aimDuration, 0.f, 1.f);
            animateAimOut(1.0f - t);
        }
        else
        {
            m_isAimAnimationDone = true;
        }
    }

    transform().setTranslation(m_dynamicPosition + m_translationMovement);
    transform().setRotation(m_dynamicRotation * m_rotationMovement);

    const float aimAmplification    = (!m_isAiming + 1) / 2.f;           // 0.5 if aiming else 1
    const float sprintAmplification = m_player->getIsSprint() * 4.f + 1; // 2.f if sprint else 1
    m_translationMovement =
        m_player->getBodyBalancing() * sprintAmplification * m_balancingStrength * aimAmplification * Vec3::forward() +
        m_player->getBodyBalancing() * sprintAmplification * m_balancingStrength * aimAmplification * Vec3::right();

    m_rotationMovement = Quaternion::identity();
}

void Firearm::onShoot()
{
}

void Firearm::animateRecoil(float t)
{
}

void Firearm::animateAimIn(float t)
{
    const float intRatio = easeOutElastic(t);
    m_dynamicPosition    = m_staticPosition.lerp(m_aimPosition, intRatio);
    m_dynamicRotation    = m_staticRotation.nlerp(m_aimRotation, intRatio);
}

void Firearm::animateAimOut(float t)
{
    const float intRatio = easeOutElastic(t);
    m_dynamicPosition    = m_aimPosition.lerp(m_staticPosition, intRatio);
    m_dynamicRotation    = m_aimRotation.nlerp(m_staticRotation, intRatio);
}

void Firearm::reload()
{
    if (!m_isReloading)
    {
        m_isReloading = true;
        m_player->source->playSound("reload", true, true);
    }
}

const GunMagazine& Firearm::getMagazine() const
{
    return m_magazineStored;
}

void Firearm::setAim(bool flag)
{
    m_isAiming = flag;
}