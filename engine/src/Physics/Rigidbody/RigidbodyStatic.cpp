#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyStatic.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <GPM/Vector3.hpp>
#include <PxPhysics.h>
//#include <foundation/PxTransform.h>

using namespace GPE;
using namespace physx;

RigidbodyStatic::RigidbodyStatic(GameObject& owner) noexcept : Component(owner)
{
    GPM::Vec3 vector = owner.getTransform().getGlobalPosition();
    PxQuat    quat   = PxQuat(owner.getTransform().getGlobalRotation().x, owner.getTransform().getGlobalRotation().y,
                         owner.getTransform().getGlobalRotation().z, owner.getTransform().getGlobalRotation().s);

    rigidbody = PxGetPhysics().createRigidStatic(PxTransform(vector.x, vector.y, vector.z, quat));
    collider  = owner.getComponent<Collider>();
    if (!collider)
    {
        FUNCT_ERROR("No collider assigned to the game object!");
    }

    else
    {
        Engine::getInstance()->physXSystem.addComponent(this);
    }
}
