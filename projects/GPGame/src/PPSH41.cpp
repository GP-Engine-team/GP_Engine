#include "Generated/PPSH41.rfk.h"
#include <Engine/ECS/Component/TransformComponent.hpp>
#include <Firearm/PPSH41.hpp>

#include <Engine/Core/Tools/Interpolation.hpp>

File_GENERATED

    using namespace GPG;
using namespace GPM;
using namespace GPE;

PPSH41::PPSH41(GPE::GameObject& owner) noexcept : Firearm(owner, GunMagazine(Bullet{5.f}, 30, 30), 3.f, 0.1f)
{
}

void PPSH41::onShoot()
{
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
