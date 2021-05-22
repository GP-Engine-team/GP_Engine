#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <GPM/Vector3.hpp>
#include <PxPhysics.h>

#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyStatic.hpp>

// Generated
#include "Generated/RigidbodyStatic.rfk.h"
File_GENERATED

    using namespace GPE;
using namespace physx;

RigidbodyStatic::RigidbodyStatic(GameObject& owner, EShapeType _type) noexcept
    : Component(owner), RigidBodyBase(owner, _type)
{
    rigidbody = PxGetPhysics().createRigidStatic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getGlobalPosition()),
                    PhysXSystem::GPMQuatToPxQuat(getOwner().getTransform().getGlobalRotation())));

    rigidbody->userData = &getOwner();

    setType(_type);

    updateToSystem();
}

void RigidbodyStatic::onPostLoad() noexcept
{

    rigidbody = PxGetPhysics().createRigidStatic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(GPM::Vec3::zero()),
            PhysXSystem::GPMQuatToPxQuat(GPM::Quat::identity())));

    rigidbody->userData = &getOwner();

    setType(type);

    updateToSystem();
}

void RigidbodyStatic::updateToSystem() noexcept
{
    if (m_isActivated)
    {
        Engine::getInstance()->physXSystem.addComponent(this);
    }
    else
    {
        Engine::getInstance()->physXSystem.removeComponent(this);
    }
}

void RigidbodyStatic::updateShape(physx::PxShape& oldShape)
{
    if (&oldShape)
    {
        rigidbody->detachShape(oldShape);
    }

    rigidbody->attachShape(*collider->shape);
}

RigidbodyStatic::~RigidbodyStatic() noexcept
{
    if (collider && collider->shape)
    {
        rigidbody->detachShape(*collider->shape);
    }

    setActive(false);

    if (rigidbody != nullptr && rigidbody->isReleasable())
    {
        rigidbody->release();
    }
}
