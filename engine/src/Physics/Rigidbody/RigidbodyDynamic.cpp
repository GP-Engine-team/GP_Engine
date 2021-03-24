#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyDynamic.hpp>
#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <GPM/Vector3.hpp>
#include <PxPhysics.h>

using namespace GPE;
using namespace physx;

RigidbodyDynamic::RigidbodyDynamic(GameObject& owner) noexcept : Component(owner)
{
    rigidbody = PxGetPhysics().createRigidDynamic(
        PxTransform(PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getGlobalPosition()),
                    PhysXSystem::GPMQuatToPxQuat(getOwner().getTransform().getGlobalRotation())));

    collider = owner.getComponent<Collider>();

    rigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
    // rigidbody->setAngularVelocity(PxVec3(0.f, 0.f, 5.f));
    // rigidbody->setAngularDamping(0.f);

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

void RigidbodyDynamic::update() noexcept
{
    getOwner().getTransform().setTranslation(PhysXSystem::PxVec3ToGPMVec3(rigidbody->getGlobalPose().p));
}
