﻿#include <Engine/Core/Physics/Collisions/BoxCollider.hpp>

#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <PhysX/PxPhysics.h>

#include <Generated/BoxCollider.rfk.h>

File_GENERATED

    using namespace GPE;
using namespace GPM;
using namespace physx;

BoxCollider::BoxCollider() noexcept : m_sizeOffset(1.f)
{
    material = Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, 0.f);
    shape    = Engine::getInstance()->physXSystem.physics->createShape(
        PxBoxGeometry(PhysXSystem::GPMVec3ToPxVec3((GPM::Vec3(1000, 1, 1000) + m_sizeOffset) * 0.5f)),
        *material, true);
}

BoxCollider::BoxCollider(GameObject& _owner) noexcept : Collider(), m_sizeOffset(1.f)
{
    material      = Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, 0.f);
    shape         = Engine::getInstance()->physXSystem.physics->createShape(
        PxBoxGeometry(PhysXSystem::GPMVec3ToPxVec3((_owner.getTransform().getGlobalScale() + m_sizeOffset) * 0.5f)),
        *material, true);

    material->release();
}

void BoxCollider::onPostLoad() noexcept
{
    //material = Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, 0.f);
    //shape    = Engine::getInstance()->physXSystem.physics->createShape(
    //    PxBoxGeometry(PhysXSystem::GPMVec3ToPxVec3((owner->getTransform().getGlobalScale() + m_sizeOffset)*0.5f)), *material, true);

    updateTransform();
    updateShape();

    material->release();
}

void BoxCollider::setSizeOffset(const Vec3& newOffset) noexcept
{
    m_sizeOffset = newOffset;
    updateShape();
}

void BoxCollider::updateShape() noexcept
{
    GPM::Vec3 lol = m_sizeOffset;
    shape->setGeometry(PxBoxGeometry(
        PhysXSystem::GPMVec3ToPxVec3((owner->getTransform().getGlobalScale() + m_sizeOffset) * 0.5f)));
}

BoxCollider::~BoxCollider()
{
    owner->getTransform().OnUpdate -= Function::make(this, "updateShape");
}
