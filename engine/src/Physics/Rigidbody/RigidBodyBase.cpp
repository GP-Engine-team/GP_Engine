﻿#include <Engine/Core/Physics/Collisions/BoxCollider.hpp>
#include <Engine/Core/Physics/Collisions/SphereCollider.hpp>
#include <Engine/Core/Physics/RigidBodyBase.hpp>

// Generated
#include "Generated/RigidbodyBase.rfk.h"

File_GENERATED

using namespace GPE;

RigidBodyBase::RigidBodyBase(GameObject& owner, EShapeType _type) noexcept : type(_type)
{
	switch (_type)
	{
	case EShapeType::E_SPHERE:
		collider = std::make_unique<SphereCollider>();
		//owner.getTransform().OnUpdate += Function::make(static_cast<SphereCollider*>(collider.get()), "updateView");
		break;
	case EShapeType::E_BOX:
		collider = std::make_unique<BoxCollider>();
		owner.getTransform().OnUpdate += Function::make(static_cast<BoxCollider*>(collider.get()), "setScale");
		break;
	default:
		break;
	}

	collider->owner = &owner;
}
