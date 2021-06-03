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
    const Vec3       basePosition = m_isAiming ? m_aimPosition : m_basePosition;
    const Quaternion baseRotation = m_isAiming ? m_aimRotation : m_baseRotation;

    m_finalPosition = basePosition + transform().getLocalForward() * m_knowbackStrength;
    m_finalRotation = baseRotation * Quaternion::angleAxis(m_knowbackMaxAngle, transform().getLocalRight());

    m_smokeEffect.pData->emit(
        static_cast<unsigned int>(m_muzzleFlashEffect.pData->getCount() / m_magazineStored.getCapacity()));

    m_muzzleFlashCount = 0.f;
}

void PPSH41::animateRecoil(float t)
{
    const Vec3       basePosition = m_isAiming ? m_aimPosition : m_basePosition;
    const Quaternion baseRotation = m_isAiming ? m_aimRotation : m_baseRotation;

    if (t < m_knowbackDuration)
    {
        const float intRatio = easeOutElastic(t / m_knowbackDuration);
        transform().setTranslation(lerp(basePosition, m_finalPosition, intRatio));
        transform().setRotation(baseRotation.nlerp(m_finalRotation, intRatio));
    }
    else
    {
        const float intRatio = easeInBounce((t - m_knowbackDuration) / (1.f - m_knowbackDuration));
        transform().setTranslation(lerp(m_finalPosition, basePosition, intRatio));
        transform().setRotation(m_finalRotation.nlerp(baseRotation, intRatio));
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
