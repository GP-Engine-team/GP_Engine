﻿#include <Engine/Core/Physics/Collisions/BoxCollider.hpp>

#include <Engine/Engine.hpp>
#include <PhysX/PxPhysics.h>

#include <Generated/BoxCollider.rfk.h>

File_GENERATED

using namespace GPE;
using namespace GPM;
using namespace physx;

BoxCollider::BoxCollider() noexcept : Collider(), m_dimensions(10.f)
{
	material = Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, 0.f);
	shape = Engine::getInstance()->physXSystem.physics->createShape(
		PxBoxGeometry(m_dimensions.x * .5f, m_dimensions.y * .5f, m_dimensions.z * .5f), *material, true);

	material->release();
}

void BoxCollider::setDimensions(const Vec3& newDimensions) noexcept
{
	m_dimensions = newDimensions;
	shape->setGeometry(PxBoxGeometry(m_dimensions.x * .5f, m_dimensions.y * .5f, m_dimensions.z * .5f));
}

void BoxCollider::onPostLoad() noexcept
{

}
