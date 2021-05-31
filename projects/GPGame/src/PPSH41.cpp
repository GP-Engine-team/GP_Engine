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

    if (m_muzzleFlashPlane.pData)
    {
        m_muzzleFlashPlane.pData->setActive(false);
    }
}

void PPSH41::onShoot()
{
    if (m_muzzleFlashPlane.pData)
    {
        m_muzzleFlashPlane.pData->setActive(true);

        m_muzzleFlashPlane.pData->getOwner().getTransform().setRotationZ(Random::ranged(0.f, TWO_PI));
        m_muzzleFlashPlane.pData->getOwner().getTransform().setScale(
            Vec3{Random::ranged(m_muzzleFlashMinScale, m_muzzleFlashMaxScale),
            Random::ranged(m_muzzleFlashMinScale, m_muzzleFlashMaxScale),
            Random::ranged(m_muzzleFlashMinScale, m_muzzleFlashMaxScale)});
    }

    m_basePosition  = transform().getPosition();
    m_finalPosition = m_basePosition + transform().getLocalForward() * m_knowbackStrength;

    m_baseRotation  = transform().getRotation();
    m_finalRotation = m_baseRotation * Quaternion::angleAxis(m_knowbackMaxAngle, transform().getLocalRight());
}

void PPSH41::animateRecoil(float t)
{
    if (t < m_knowbackDuration)
    {
        const float intRatio = easeOutElastic(t / m_knowbackDuration);
        transform().setTranslation(lerp(m_basePosition, m_finalPosition, intRatio));
        transform().setRotation(m_baseRotation.nlerp(m_finalRotation, intRatio));
    }
    else
    {
        const float intRatio = easeInBounce((t - m_knowbackDuration) / (1.f - m_knowbackDuration));
        transform().setTranslation(lerp(m_finalPosition, m_basePosition, intRatio));
        transform().setRotation(m_finalRotation.nlerp(m_baseRotation, intRatio));
    }
}

void PPSH41::update(double deltaTime)
{
    Firearm::update(deltaTime);

    if (m_muzzleFlashPlane.pData && m_muzzleFlashPlane.pData->isActivated())
    {
        m_muzzleFlashCount += float(deltaTime);

        if (m_muzzleFlashCount >= m_muzzleFlashDuration)
        {
            m_muzzleFlashPlane.pData->setActive(false);
            m_muzzleFlashCount = 0.f;
        }
    }
}
