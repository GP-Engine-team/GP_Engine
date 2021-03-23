#include <Engine/ECS/Component/Physics/Collisions/SphereCollider.hpp>
#include <Engine/Engine.hpp>

using namespace GPE;
using namespace physx;

SphereCollider::SphereCollider(GameObject& owner) noexcept : Collider(owner), m_center(0, 0, 0), m_radius(10)
{
    material = Engine::getInstance()->physXSystem.physics->createMaterial(1, 1, 1);
    shape    = Engine::getInstance()->physXSystem.physics->createShape(PxSphereGeometry(m_radius), *material, true);
}

void SphereCollider::setRadius(float newRadius) noexcept
{
    m_radius                             = newRadius;
    shape->getGeometry().sphere().radius = m_radius;
}
