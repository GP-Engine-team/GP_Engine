#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyDynamic.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <GPM/Vector3.hpp>
#include <PxPhysics.h>

using namespace GPE;
using namespace physx;

RigidbodyDynamic::RigidbodyDynamic(GameObject& owner) noexcept : Component(owner)
{
    GPM::Vec3 vector   = getOwner().getTransform().getGlobalPosition();
    GPM::Quat tempQuat = getOwner().getTransform().getGlobalRotation();
    PxQuat    quat     = PxQuat(tempQuat.x, tempQuat.y, tempQuat.z, tempQuat.s);
    rigidbody          = PxGetPhysics().createRigidDynamic(PxTransform(vector.x, vector.y, vector.z, quat));
    collider           = owner.getComponent<Collider>();

    // rigidbody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
    // rigidbody->setAngularVelocity(PxVec3(0.f, 0.f, 5.f));
    // rigidbody->setAngularDamping(0.f);

    rigidbody->attachShape(*collider->shape);
    collider->shape->release();

    if (!collider)
    {
        FUNCT_ERROR("No collider assigned to the game object!");
    }

    else
    {
        Engine::getInstance()->physXSystem.addComponent(this);
    }
}

GPM::Vec3 PxVec3FromVec3(PxVec3 v)
{
    return {v.x, v.y, v.z};
}

void RigidbodyDynamic::update() noexcept
{
    GPM::Vec3 vector   = getOwner().getTransform().getGlobalPosition();
    GPM::Quat tempQuat = getOwner().getTransform().getGlobalRotation();
    PxQuat    quat     = PxQuat(tempQuat.x, tempQuat.y, tempQuat.z, tempQuat.s);
    // rigidbody->setGlobalPose(PxTransform(vector.x, vector.y, vector.z, quat));
    // getOwner().getTransform().setTranslation(PxVec3FromVec3(rigidbody->getGlobalPose().p));
}
