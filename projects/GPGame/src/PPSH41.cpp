#include "Generated/PPSH41.rfk.h"
#include <Engine/ECS/Component/TransformComponent.hpp>
#include <Firearm/PPSH41.hpp>
#include <gpm/Random.hpp>

#include <Engine/Core/Tools/Interpolation.hpp>

File_GENERATED

    using namespace GPG;
using namespace GPM;
using namespace GPE;

PPSH41::PPSH41(GPE::GameObject& owner) noexcept : Firearm(owner, GunMagazine(Bullet{5.f}, 30, 30), 3.f, 0.1f)
{
}

void PPSH41::start()
{
    Firearm::start();

    if (m_muzzleFlashGO.pData)
    {
        m_muzzleFlashGO.pData->setActive(false);
    }

    GAME_ASSERT(m_smokeEffect.pData, "Missing component");
}

void PPSH41::onShoot()
{
    if (m_muzzleFlashGO.pData)
    {
        m_muzzleFlashGO.pData->setActive(true);

        m_muzzleFlashGO.pData->getTransform().setRotationZ(Random::ranged(0.f, TWO_PI));
        m_muzzleFlashGO.pData->getTransform().setScale(
            Vec3{Random::ranged(m_muzzleFlashMinScale, m_muzzleFlashMaxScale),
                 Random::ranged(m_muzzleFlashMinScale, m_muzzleFlashMaxScale),
                 Random::ranged(m_muzzleFlashMinScale, m_muzzleFlashMaxScale)});
    }

    m_smokeEffect.pData->emit(
        static_cast<unsigned int>(m_muzzleFlashEffect.pData->getCount() / m_magazineStored.getCapacity()));

    m_muzzleFlashCount = 0.f;
}

void PPSH41::animateRecoil(float t)
{
    const Vec3       basePosition = m_isAiming ? m_aimPosition : m_dynamicPosition;
    const Quaternion baseRotation = m_isAiming ? m_aimRotation : m_dynamicRotation;

    if (t < m_knowbackDuration)
    {
        const float newIntRatio = easeOutElastic(t / m_knowbackDuration);

        m_translationMovement += transform().getLocalForward() * m_knowbackStrength * (newIntRatio - recoilT);
        m_rotationMovement = m_rotationMovement *
                             Quaternion::angleAxis(m_knowbackMaxAngle, transform().getLocalRight()) *
                             (newIntRatio - recoilT);

        recoilT = newIntRatio;
    }
    else
    {
        const float newIntRatio = easeInBounce((t - m_knowbackDuration) / (1.f - m_knowbackDuration));
        m_translationMovement += transform().getLocalForward() * m_knowbackStrength * (1.f - (newIntRatio - recoilT));
        m_rotationMovement = m_rotationMovement *
                             Quaternion::angleAxis(m_knowbackMaxAngle, transform().getLocalRight()) *
                             (1.f - (newIntRatio - recoilT));

        recoilT = newIntRatio;
    }
}

void PPSH41::update(double deltaTime)
{
    Firearm::update(deltaTime);

    if (m_muzzleFlashGO.pData && m_muzzleFlashGO.pData->isActivated())
    {
        m_muzzleFlashCount += float(deltaTime);

        if (m_muzzleFlashCount >= m_muzzleFlashDuration)
        {
            m_muzzleFlashGO.pData->setActive(false);
            m_muzzleFlashCount = 0.f;
        }
    }
}
