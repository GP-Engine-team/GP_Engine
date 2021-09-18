/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Physics/Collisions/Collider.hpp>
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <GPM/Vector3.hpp>

// Generated
#include <Generated/SphereCollider.rfk.h>

namespace GPE RFKNamespace()
{
    class RFKClass(Inspect(), Serialize()) SphereCollider : public Collider
    {
    public:
        SphereCollider() noexcept;
        SphereCollider(GameObject & owner) noexcept;
        SphereCollider(const SphereCollider& other) noexcept = delete;
        SphereCollider(SphereCollider && other) noexcept     = default;
        virtual ~SphereCollider() noexcept;
        SphereCollider& operator=(SphereCollider const& other) noexcept = delete;
        SphereCollider& operator=(SphereCollider&& other) noexcept = delete;

    private:
        RFKField(Serialize(), Inspect("setRadius")) float m_radius;

    public:
        GETTER_BY_VALUE(Radius, m_radius);
        void             setRadius(float newRadius) noexcept;
        RFKMethod() void updateShape() noexcept;
        virtual void     onPostLoad() noexcept override;

        SphereCollider_GENERATED
    };
} // namespace )

File_GENERATED