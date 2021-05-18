#pragma once
#include <Engine/Core/Physics/Collisions/Collider.hpp>
#include <Refureku/Refureku.h>
#include <memory>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>
#include <Engine/Intermediate/GameObject.hpp>

// Generated
#include <Generated/RigidBodyBase.rfk.h>

namespace GPE RFKNamespace()
{
	enum class EShapeType
	{
		E_SPHERE,
		E_BOX,
	};

	class RFKClass(Inspect(false), Serialize(false)) RigidBodyBase : rfk::Object
	{
	public:
		RigidBodyBase(GameObject & owner, EShapeType _type) noexcept;

		RigidBodyBase() noexcept = default;
		RigidBodyBase(const RigidBodyBase & other) noexcept = delete;
		RigidBodyBase(RigidBodyBase && other) noexcept = default;
		RigidBodyBase& operator=(RigidBodyBase const& other) noexcept = delete;
		RigidBodyBase& operator=(RigidBodyBase && other) noexcept = delete;

		// virtual void updateTransform() = 0;
		virtual ~RigidBodyBase() noexcept = default;

	public:
		EShapeType                type;
		std::unique_ptr<Collider> collider;

		RigidBodyBase_GENERATED
	};
} // namespace )
