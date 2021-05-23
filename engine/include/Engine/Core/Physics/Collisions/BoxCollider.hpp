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
#include <Generated/BoxCollider.rfk.h>

namespace GPE RFKNamespace()
{
    class RFKClass(Inspect(), Serialize()) BoxCollider : public Collider
    {
    public:
        BoxCollider() noexcept;
        BoxCollider(GameObject & owner) noexcept;
        BoxCollider(const BoxCollider& other) noexcept = delete;
        BoxCollider(BoxCollider && other) noexcept     = default;
        virtual ~BoxCollider() noexcept;

        BoxCollider& operator=(BoxCollider const& other) noexcept = delete;
        BoxCollider& operator=(BoxCollider&& other) noexcept = delete;

    private:
        RFKField(Serialize(), Inspect("setSizeOffset")) GPM::Vector3 m_sizeOffset;

    public:
        GETTER_BY_VALUE(Offset, m_sizeOffset);
        void             setSizeOffset(const GPM::Vec3& newSizeOffset) noexcept;
        void             onPostLoad() noexcept override;
        RFKMethod() void updateShape() noexcept;

        BoxCollider_GENERATED
    };
} // namespace )
