﻿#include <Engine/ECS/Component/Physics/Collisions/SphereCollider.hpp>

#include <Engine/Engine.hpp>
#include <PhysX/PxPhysics.h>

#include <Generated/SphereCollider.rfk.h>

File_GENERATED

using namespace GPE;
using namespace physx;
using namespace std;

SphereCollider::SphereCollider(GameObject& owner) noexcept
    : Collider(owner), m_center(.0f, .0f, .0f), m_radius(10.f)
{
    material = Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, .0f);
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
