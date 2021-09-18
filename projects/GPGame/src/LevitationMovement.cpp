#include <Engine/Core/Debug/assert.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <algorithm>
#include <gpm/Vector3.hpp>
#include <gpm/Vector4.hpp>

#include <Generated/LevitationMovement.cpp.h>

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

    transform().setRotation(Quaternion::fromEuler({0.f, m_rotationSpeed * m_delay, 0.f}));
}