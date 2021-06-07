#include <LevitationMovement.hpp>

#include <Generated/LevitationMovement.rfk.h>
File_GENERATED

#include <Engine/Core/Debug/assert.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <algorithm>
#include <gpm/Vector3.hpp>
#include <gpm/Vector4.hpp>

    using namespace GPE;
using namespace GPG;
using namespace GPM;

LevitationMovement::LevitationMovement(GameObject& owner) : BehaviourComponent(owner)
{
}

void LevitationMovement::onPostLoad()
{
    enableUpdate(true);

    BehaviourComponent::onPostLoad();
}

void LevitationMovement::start()
{
}

void LevitationMovement::update(double deltaTime)
{
    // Add levitatio effect
    m_delay += deltaTime * m_speed;

    if (m_delay > TWO_PI)
    {
        m_delay -= TWO_PI;
    }

    transform().setTranslation(
        {transform().getPosition().x, m_initialY + cosf(m_delay) * m_heightIntensity, transform().getPosition().z});

    m_delayRot += deltaTime * m_rotationSpeed;

    if (m_delayRot > TWO_PI)
    {
        m_delayRot -= TWO_PI;
    }

    transform().setRotation(Quaternion::fromEuler({0.f, m_delayRot, 0.f}));
}