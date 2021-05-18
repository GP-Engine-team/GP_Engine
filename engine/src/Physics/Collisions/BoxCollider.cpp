#include <Engine/Core/Physics/Collisions/BoxCollider.hpp>

#include <Engine/Engine.hpp>
#include <PhysX/PxPhysics.h>
#include <Engine/Intermediate/GameObject.hpp>

#include <Generated/BoxCollider.rfk.h>

File_GENERATED

using namespace GPE;
using namespace GPM;
using namespace physx;

BoxCollider::BoxCollider() noexcept : Collider(), m_offset(10.f)
{
	material = Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, 0.f);
	shape = Engine::getInstance()->physXSystem.physics->createShape(
		PxBoxGeometry(m_offset.x * .5f, m_offset.y * .5f, m_offset.z * .5f), *material, true);

	material->release();
}

void BoxCollider::setScale() noexcept
{
	m_scale = owner->getTransform().getGlobalScale();
	updateShape();
}

void BoxCollider::setOffset(const Vec3& newOffset) noexcept
{
	m_offset = newOffset;
	updateShape();
}

void BoxCollider::updateShape() noexcept
{
	//shape->setGeometry(PxBoxGeometry(m_offset.x * .5f, m_offset.y * .5f, m_offset.z * .5f));
	shape->setGeometry(PxBoxGeometry(PhysXSystem::GPMVec3ToPxVec3((m_offset + m_scale) * 0.5f)));
}
