#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <GPM/Vector3.hpp>
#include <PxPhysics.h>

#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyDynamic.hpp>

// Generated
#include "Generated/RigidbodyDynamic.rfk.h"

File_GENERATED

    using namespace GPE;
using namespace physx;

RigidbodyDynamic::RigidbodyDynamic(GameObject& owner) noexcept : Component(owner), RigidBodyBase(owner)
{
    rigidbody = PxGetPhysics().createRigidDynamic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(owner.getTransform().getGlobalPosition()),
                    PhysXSystem::GPMQuatToPxQuat(owner.getTransform().getGlobalRotation())));

    rigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
    rigidbody->setMass(1);
    rigidbody->userData = &getOwner();

    setType(type);

    updateToSystem();
    getOwner().getTransform().OnUpdate += Function::make(this, "updateTransform");
}

void RigidbodyDynamic::onPostLoad() noexcept
{
    owner = &getOwner();

    rigidbody = PxGetPhysics().createRigidDynamic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(owner->getTransform().getGlobalPosition()),
                    PhysXSystem::GPMQuatToPxQuat(owner->getTransform().getGlobalRotation())));

    rigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
    rigidbody->setMass(1);

    rigidbody->userData = &getOwner();

    if (!collider)
    {
        setType(type);
    }
    else
    {
        rigidbody->attachShape(*collider->shape);
    }

    collider->updateTransform();

    Component::onPostLoad();

    getOwner().getTransform().OnUpdate += Function::make(this, "updateTransform");
}

void RigidbodyDynamic::update() noexcept
{
    getOwner().getTransform().setTranslation(PhysXSystem::PxVec3ToGPMVec3(rigidbody->getGlobalPose().p));
}

void RigidbodyDynamic::updatePosition() noexcept
{
    rigidbody->setGlobalPose(PhysXSystem::GPETransformComponentToPxTransform(getOwner().getTransform()));
}

void RigidbodyDynamic::setKinematic(bool state) noexcept
{
    if (m_isKinematic == state)
        return;

    m_isKinematic = state;
    rigidbody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, state);
}

void RigidbodyDynamic::updateToSystem() noexcept
{
    if (m_isActivated)
        GPE::Engine::getInstance()->physXSystem.addComponent(this);
    else
        GPE::Engine::getInstance()->physXSystem.removeComponent(this);
}

void RigidbodyDynamic::updateShape(physx::PxShape& oldShape)
{
    if (&oldShape)
    {
        rigidbody->detachShape(oldShape);
    }

    rigidbody->attachShape(*collider->shape);
}

void RigidbodyDynamic::updateTransform()
{
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

RigidbodyDynamic::~RigidbodyDynamic() noexcept
{
    setActive(false);
    getOwner().getTransform().OnUpdate -= Function::make(this, "updateTransform");
}
