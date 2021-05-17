#include <Engine/Core/Physics/Collisions/BoxCollider.hpp>
#include <Engine/Core/Physics/Collisions/SphereCollider.hpp>
#include <Engine/Core/Physics/RigidBodyBase.hpp>

// Generated
#include "Generated/RigidbodyBase.rfk.h"

File_GENERATED

    using namespace GPE;

RigidBodyBase::RigidBodyBase(EShapeType _type) noexcept : type(_type)
{
    if (_type == EShapeType::E_BOX)
    {
        std::unique_ptr<Collider> p1(new BoxCollider);
        collider = std::move(p1);
    }

    else if (_type == EShapeType::E_SPHERE)
    {
        std::unique_ptr<Collider> p1(new SphereCollider);
        collider = std::move(p1);
    }
}