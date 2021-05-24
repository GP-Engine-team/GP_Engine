#include <Engine/Core/Physics/Collisions/Collider.hpp>
#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Intermediate/GameObject.hpp>

// Generated
#include "Generated/Collider.rfk.h"

File_GENERATED

    using namespace GPE;
using namespace physx;

Collider::~Collider() noexcept
{
    // if (material && material->isReleasable())
    //{
    //    material->release();
    //}
}

void Collider::setCenter(const GPM::Vec3& newCenter) noexcept
{
    center = newCenter;
    updateTransform();
}

void Collider::setLocalRotation(const GPM::Vec3& newLocalRotation) noexcept
{
    localRotation = newLocalRotation;
    updateTransform();
}

void Collider::updateTransform() noexcept
{
    PxVec3 newPos = PhysXSystem::GPMVec3ToPxVec3(owner->getTransform().getGlobalPosition() + center);
    PxQuat newQuat =
        PhysXSystem::GPMQuatToPxQuat(owner->getTransform().getRotation() + GPM::Quat::fromEuler(localRotation));
    // shape->setLocalPose(PxTransform(newPos, newQuat));
}
