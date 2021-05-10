#include <Engine/ECS/Component/Physics/Collisions/SphereCollider.hpp>
#include <Engine/Engine.hpp>

// Generated
#include "Generated/SphereCollider.rfk.h"

File_GENERATED

    using namespace GPE;
using namespace physx;
using namespace std;

SphereCollider::SphereCollider(GameObject& owner) noexcept : Collider(owner)
{
    material = Engine::getInstance()->physXSystem.physics->createMaterial(1, 1, 0);
    shape    = Engine::getInstance()->physXSystem.physics->createShape(PxSphereGeometry(m_radius), *material, true);
}

void SphereCollider::setRadius(float newRadius) noexcept
{
    m_radius = newRadius;
    shape->setGeometry(PxSphereGeometry(m_radius));
}

/*void SphereCollider::updateTransform()
{

}*/

void SphereCollider::awake()
{

}