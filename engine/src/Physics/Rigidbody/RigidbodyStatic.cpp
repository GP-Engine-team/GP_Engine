#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <GPM/Vector3.hpp>
#include <PxPhysics.h>

#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyStatic.hpp>

// Generated
#include <Generated/RigidbodyStatic.rfk.h>
File_GENERATED

    using namespace GPE;
using namespace physx;

RigidbodyStatic::RigidbodyStatic(GameObject& owner) noexcept : Component(owner), RigidBodyBase(owner)
{
    rigidbody = PxGetPhysics().createRigidStatic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getGlobalPosition()),
                    PhysXSystem::GPMQuatToPxQuat(getOwner().getTransform().getGlobalRotation())));

    rigidbody->userData = &getOwner();

    setType(type);

    updateToSystem();
    getOwner().getTransform().OnUpdate += Function::make(this, "updateTransform");
}

void RigidbodyStatic::onPostLoad() noexcept
{
    using namespace GPM;

    owner = &getOwner();
    {
        const Quat        rot{getOwner().getTransform().getGlobalRotation()};
        const Vec3        pos{getOwner().getTransform().getGlobalPosition()};
        const PxQuat      pxRot{PhysXSystem::GPMQuatToPxQuat(rot)};
        const PxVec3      pxPos{PhysXSystem::GPMVec3ToPxVec3(pos)};
        const PxTransform transform{pxPos, pxRot};

        rigidbody = PxGetPhysics().createRigidStatic(transform);

        rigidbody->userData = &getOwner();
    }

    if (!collider)
    {
        setType(type);
    }
    else
    {
        rigidbody->attachShape(*collider->shape);
    }

    collider->updateTransform();
    collider->updateShape();

    Component::onPostLoad();

    getOwner().getTransform().OnUpdate += Function::make(this, "updateTransform");
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
    rigidbody->detachShape(oldShape);
    rigidbody->attachShape(*collider->shape);
}

void RigidbodyStatic::updateTransform()
{
    if (!rigidbody)
        return;

    PxTransform newTransform;

    newTransform.p = PhysXSystem::GPMVec3ToPxVec3(owner->getTransform().getGlobalPosition());
    newTransform.q = PhysXSystem::GPMQuatToPxQuat(owner->getTransform().getGlobalRotation());

    PxTransform oldTransform;
    oldTransform = rigidbody->getGlobalPose();

    if (newTransform.p == oldTransform.p && newTransform.q == oldTransform.q)
    {
        return;
    }

    rigidbody->setGlobalPose(newTransform);
}

RigidbodyStatic::~RigidbodyStatic() noexcept
{
    setActive(false);
    getOwner().getTransform().OnUpdate -= Function::make(this, "updateTransform");
}
