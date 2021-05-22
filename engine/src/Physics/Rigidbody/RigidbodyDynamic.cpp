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

RigidbodyDynamic::RigidbodyDynamic(GameObject& owner) noexcept : Component(owner)
{
    rigidbody = PxGetPhysics().createRigidDynamic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getGlobalPosition()),
                    PhysXSystem::GPMQuatToPxQuat(getOwner().getTransform().getGlobalRotation())));

    collider = owner.getComponent<Collider>();

    rigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
    rigidbody->setMass(1);
    rigidbody->userData = &getOwner();

    if (!collider)
    {
        FUNCT_ERROR("No collider assigned to the game object!");
        m_isActivated = false;
    }

    else
    {
        rigidbody->attachShape(*collider->shape);
        collider->shape->release();
    }

    updateToSystem();
}

void RigidbodyDynamic::onPostLoad() noexcept
{
    //rigidbody = PxGetPhysics().createRigidDynamic(
    //    PxTransform(PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getGlobalPosition()),
    //                PhysXSystem::GPMQuatToPxQuat(getOwner().getTransform().getGlobalRotation())));
     rigidbody = PxGetPhysics().createRigidDynamic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(GPM::Vec3::zero()),
                    PhysXSystem::GPMQuatToPxQuat(GPM::Quat::identity())));

    rigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
    rigidbody->setMass(1);
    rigidbody->userData = &getOwner();

    if (!collider)
    {
        FUNCT_ERROR("No collider assigned to the game object!");
        m_isActivated = false;
    }

    else
    {
        rigidbody->attachShape(*collider->shape);
        collider->shape->release();
    }

    Component::onPostLoad();
}

RigidbodyDynamic::~RigidbodyDynamic() noexcept
{
    setActive(false);
}

void RigidbodyDynamic::update() noexcept
{
    getOwner().getTransform().setTranslation(PhysXSystem::PxVec3ToGPMVec3(rigidbody->getGlobalPose().p));
}

void RigidbodyDynamic::updatePosition() noexcept
{
    // getOwner().getTransform().
    // getOwner().getTransform().setTranslation(PhysXSystem::PxVec3ToGPMVec3(rigidbody->getGlobalPose().p));
    rigidbody->setGlobalPose(PhysXSystem::GPETransformComponentToPxTransform(getOwner().getTransform()));
    // rigidbody->setKinematicTarget(physx::PxTransform::transform(physx::PxPlane::));
    // collider->shape
}

void RigidbodyDynamic::setKinematic(bool state) noexcept
{
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