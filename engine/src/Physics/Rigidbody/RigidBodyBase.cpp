#include <Engine/Core/Physics/Collisions/BoxCollider.hpp>
#include <Engine/Core/Physics/Collisions/SphereCollider.hpp>

#include <Generated/RigidBodyBase.cpp.h>

    using namespace GPE;

void RigidBodyBase::setType(EShapeType& newType)
{
    physx::PxShape* oldShape = nullptr;

    if (collider && collider->shape)
    {
        if (newType == type)
        {
            return;
        }

        oldShape = collider->shape;
    }

    type = newType;

    switch (newType)
    {
    case EShapeType::E_SPHERE:
        collider = std::make_unique<SphereCollider>(*owner);
        owner->getTransform().OnUpdate += Function::make(static_cast<SphereCollider*>(collider.get()), "updateShape");
        break;
    case EShapeType::E_BOX:
        collider = std::make_unique<BoxCollider>(*owner);
        owner->getTransform().OnUpdate += Function::make(static_cast<BoxCollider*>(collider.get()), "updateShape");
        break;
    default:
        break;
    }

    updateShape(*oldShape);

    // rigidbody->attachShape(*collider->shape);
    // collider->shape->release();
    collider->owner = owner;
}

RigidBodyBase::RigidBodyBase(GameObject& _owner) noexcept
{
    owner = &_owner;
}
