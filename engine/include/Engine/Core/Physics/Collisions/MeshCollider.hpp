/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */
#pragma once

#include <Engine/Core/Physics/Collisions/Collider.hpp>

 // Generated
#include <Generated/MeshCollider.rfk.h>

namespace physx
{
	class PxConvexMesh;
}

namespace GPE RFKNamespace()
{
	class RFKClass(Inspect()) MeshCollider : public Collider
	{
	public:
		MeshCollider() noexcept;

		virtual ~MeshCollider() noexcept = default;
		MeshCollider(const MeshCollider & other) noexcept = delete;
		MeshCollider(MeshCollider && other) noexcept = default;
		MeshCollider& operator=(MeshCollider const& other) noexcept = delete;
		MeshCollider& operator=(MeshCollider && other) noexcept = delete;

	public:
		physx::PxConvexMesh* convexMesh = nullptr;

		MeshCollider_GENERATED
	};
} // namespace )

File_GENERATED