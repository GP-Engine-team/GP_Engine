#include <Engine/Core/Physics/Collisions/BoxCollider.hpp>
#include <Engine/Core/Physics/Collisions/SphereCollider.hpp>
#include <Engine/Core/Physics/RigidBodyBase.hpp>

// Generated
#include "Generated/RigidbodyBase.rfk.h"

File_GENERATED

    using namespace GPE;

void RigidBodyBase::setType(EShapeType& newType)
{
    if (newType == type)
        return;

    type = newType;

    physx::PxShape* oldShape = nullptr;

    if (collider && collider->shape)
    {
        oldShape = collider->shape;
    }

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

RigidBodyBase::RigidBodyBase(GameObject& _owner, EShapeType _type) noexcept
{
    owner = &_owner;
}
