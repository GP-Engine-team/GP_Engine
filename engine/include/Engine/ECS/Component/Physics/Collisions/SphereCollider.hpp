/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/ECS/Component/Physics/Collisions/Collider.hpp>
#include <GPM/Vector3.hpp>

// Generated
#include <Generated/SphereCollider.rfk.h>

namespace GPE RFKNamespace()
{
    class RFKClass(ComponentGen(), Inspect(), Serialize()) SphereCollider : public Collider
    {
    public:
        SphereCollider(GameObject & owner)                     noexcept;
        SphereCollider()                                       noexcept = default;
        SphereCollider(const SphereCollider& other)            noexcept = delete;
        SphereCollider(SphereCollider&& other)                 noexcept = default;
        virtual ~SphereCollider()                              noexcept = default;
        SphereCollider& operator=(SphereCollider const& other) noexcept = delete;
        SphereCollider& operator=(SphereCollider&& other)      noexcept = delete;

        // void updateTransform() final;

    private:
        RFKField(Inspect(), Serialize()) GPM::Vector3 m_center = {0, 0, 0};
        RFKField(Inspect("setRadius"), Serialize()) float m_radius = 10.f;

    public:
        DEFAULT_GETTER_SETTER_BY_REF(Center, m_center);
        GETTER_BY_VALUE(Radius, m_radius);
        void setRadius(float newRadius) noexcept;

        virtual void onPostLoad() override;

        SphereCollider_GENERATED
    };
} // namespace GPE
