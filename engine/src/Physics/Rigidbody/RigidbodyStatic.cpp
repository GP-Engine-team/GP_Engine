#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyStatic.hpp>
#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <GPM/Vector3.hpp>
#include <PxPhysics.h>

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

    if (!collider)
    {
        FUNCT_ERROR("No collider assigned to the game object!");
    }

    else
    {
        rigidbody->attachShape(*collider->shape);
        collider->shape->release();

        Engine::getInstance()->physXSystem.addComponent(this);
    }
}
