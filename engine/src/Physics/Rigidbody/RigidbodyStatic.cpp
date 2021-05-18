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

RigidbodyStatic::RigidbodyStatic(GameObject& owner, EShapeType _type) noexcept : Component(owner), RigidBodyBase(_type)
{
    rigidbody = PxGetPhysics().createRigidStatic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getGlobalPosition()),
                    PhysXSystem::GPMQuatToPxQuat(getOwner().getTransform().getGlobalRotation())));

    rigidbody->userData = &getOwner();

    rigidbody->attachShape(*collider->shape);
    collider->shape->release();

    Engine::getInstance()->physXSystem.addComponent(this);
}

void RigidbodyStatic::setActive(bool newState) noexcept
{
    if (m_isActivated == newState)
        return;

    m_isActivated = newState;
    updateToSystem();
}

void RigidbodyStatic::onPostLoad()
{
    setType(EShapeType::E_BOX);

    GPE::TransformComponent& transform = getOwner().getTransform();
    transform.update();

    rigidbody = PxGetPhysics().createRigidStatic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(transform.getGlobalPosition()),
                    PhysXSystem::GPMQuatToPxQuat(transform.getGlobalRotation())));

    rigidbody->userData = &getOwner();

    rigidbody->attachShape(*collider->shape);
    collider->shape->release();
    updateToSystem();
}

void RigidbodyStatic::updateToSystem()
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

RigidbodyStatic::~RigidbodyStatic() noexcept
{
    //if (rigidbody != nullptr && rigidbody->isReleasable())
    //{
    //    rigidbody->release();
    //}

    //setActive(false);
}