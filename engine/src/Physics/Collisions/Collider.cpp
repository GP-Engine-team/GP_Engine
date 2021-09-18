#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Intermediate/GameObject.hpp>

#include <Generated/Collider.cpp.h>

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
    const PxVec3 newPos  = PhysXSystem::GPMVec3ToPxVec3(center);
    const PxQuat newQuat = PhysXSystem::GPMQuatToPxQuat(GPM::Quat::fromEuler(localRotation));
    shape->setLocalPose(PxTransform(newPos, newQuat));
}
