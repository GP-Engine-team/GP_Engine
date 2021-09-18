#include <Engine/Core/Debug/assert.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <algorithm>
#include <gpm/Vector3.hpp>
#include <gpm/Vector4.hpp>

#include <Generated/CameraOscilation.cpp.h>

    using namespace GPE;
using namespace GPG;
using namespace GPM;

CameraOscilation::CameraOscilation(GameObject& owner) : BehaviourComponent(owner)
{
}

void CameraOscilation::onPostLoad()
{
    enableUpdate(true);

    BehaviourComponent::onPostLoad();
}

void CameraOscilation::start()
{
    m_baseEulerRotation = transform().getRotation().eulerAngles();
}

void CameraOscilation::update(double deltaTime)
{
    // Add levitatio effect
    m_delay += deltaTime * m_speed;

    if (m_delay > TWO_PI)
    {
        m_delay -= TWO_PI;
    }

    transform().setRotation(
        Quaternion::fromEuler({0.f + m_baseEulerRotation.x, 0.f + m_baseEulerRotation.y,
                               PI / 180.f * m_angle * cosf(m_delay) + m_baseEulerRotation.z}));
}