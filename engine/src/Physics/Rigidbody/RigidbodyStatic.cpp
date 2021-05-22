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

RigidbodyStatic::RigidbodyStatic(GameObject& owner) noexcept : Component(owner)
{
    rigidbody = PxGetPhysics().createRigidStatic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getGlobalPosition()),
                    PhysXSystem::GPMQuatToPxQuat(getOwner().getTransform().getGlobalRotation())));

    collider = owner.getComponent<Collider>();

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

void RigidbodyStatic::onPostLoad() noexcept
{
    //rigidbody = PxGetPhysics().createRigidStatic(
    //    PxTransform(PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getGlobalPosition()),
    //                PhysXSystem::GPMQuatToPxQuat(getOwner().getTransform().getGlobalRotation())));
    rigidbody = PxGetPhysics().createRigidStatic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(GPM::Vec3::zero()),
                    PhysXSystem::GPMQuatToPxQuat(GPM::Quat::identity())));

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

RigidbodyStatic::~RigidbodyStatic() noexcept
{
    setActive(false);
}

void RigidbodyStatic::updateToSystem() noexcept
{
    if (m_isActivated)
        GPE::Engine::getInstance()->physXSystem.addComponent(this);
    else
        GPE::Engine::getInstance()->physXSystem.removeComponent(this);
}