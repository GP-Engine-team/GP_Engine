#include <Engine/Core/Physics/Collisions/BoxCollider.hpp>

#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <PhysX/PxPhysics.h>

#include <Generated/BoxCollider.rfk.h>

File_GENERATED

using namespace GPE;
using namespace GPM;
using namespace physx;

BoxCollider::BoxCollider() noexcept : Collider(), m_sizeOffset(10.f)
{
    material = Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, 0.f);
    shape    = Engine::getInstance()->physXSystem.physics->createShape(
        PxBoxGeometry(m_sizeOffset.x * .5f, m_sizeOffset.y * .5f, m_sizeOffset.z * .5f), *material, true);

    material->release();
}

void BoxCollider::setSizeOffset(const Vec3& newOffset) noexcept
{
    m_sizeOffset = newOffset;
    updateShape();
}

void BoxCollider::updateShape() noexcept
{
    shape->setGeometry(
        PxBoxGeometry(PhysXSystem::GPMVec3ToPxVec3((owner->getTransform().getGlobalScale() + m_sizeOffset) * 0.5f)));
}

