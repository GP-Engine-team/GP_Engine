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
    if (m_isActivated)
        GPE::Engine::getInstance()->physXSystem.addComponent(this);
    else
        GPE::Engine::getInstance()->physXSystem.removeComponent(this);
}

RigidbodyStatic::~RigidbodyStatic() noexcept
{
    if (rigidbody != nullptr && rigidbody->isReleasable())
    {
        rigidbody->release();
    }
    Engine::getInstance()->physXSystem.removeComponent(this);
}