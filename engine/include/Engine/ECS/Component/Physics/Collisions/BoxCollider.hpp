/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/ECS/Component/Physics/Collisions/Collider.hpp>
#include <GPM/Vector3.hpp>

// Generated
#include <Generated/BoxCollider.rfk.h>

namespace GPE RFKNamespace()
{
    class RFKClass(ComponentGen(), Inspect(), Serialize()) BoxCollider : public Collider
    {
    public:
        BoxCollider(GameObject& owner)                   noexcept;
        BoxCollider()                                    noexcept = default;
        virtual ~BoxCollider()                           noexcept = default;

    private:
        RFKField(Inspect(), Serialize()) GPM::Vector3 m_center = {0,0,0};
        RFKField(Inspect(), Serialize()) GPM::Vector3 m_dimensions = {10,10,10};

    public:
        DEFAULT_GETTER_SETTER_BY_REF(Center, m_center);
        GETTER_BY_VALUE(Dimensions, m_dimensions);
        void setDimensions(const GPM::Vec3& newDimensions) noexcept;

        BoxCollider_GENERATED
    };
} // namespace )
