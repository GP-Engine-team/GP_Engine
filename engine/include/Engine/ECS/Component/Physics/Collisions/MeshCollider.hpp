/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */
#pragma once
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/ECS/Component/Physics/Collisions/collider.hpp>
#include <GPM/Vector3.hpp>
#include <PhysX/geometry/PxConvexMesh.h>

// Generated
#include "Generated/MeshCollider.rfk.h"

namespace GPE RFKNamespace()
{
    class RFKClass(ComponentGen(), Inspect()) MeshCollider : public Collider
    {
    public:
        MeshCollider(GameObject & owner) noexcept;

        virtual ~MeshCollider() noexcept = default;

        MeshCollider() noexcept                          = delete;
        MeshCollider(const MeshCollider& other) noexcept = delete;
        MeshCollider(MeshCollider && other) noexcept     = default;
        MeshCollider& operator=(MeshCollider const& other) noexcept = delete;
        MeshCollider& operator=(MeshCollider&& other) noexcept = delete;

        virtual void awake() override;

    public:
        physx::PxConvexMesh* convexMesh = nullptr;

        MeshCollider_GENERATED
    };
} // namespace )