/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/InspectContext.hpp>
#include <Engine/Serialization/Serialize.hpp>
#include <Engine/Serialization/xml/xmlLoader.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>
#include <GPM/Vector3.hpp>
#include <PxMaterial.h>
#include <PxShape.h>
#include <Refureku/Refureku.h>

// Generated
#include <Generated/Collider.rfk.h>

namespace GPE RFKNamespace()
{
    class RFKClass(Inspect(false), Serialize(false)) Collider : rfk::Object
    {
    public:
        Collider() noexcept                      = default;
        Collider(const Collider& other) noexcept = delete;
        Collider(Collider && other) noexcept     = default;
        Collider& operator=(Collider const& other) noexcept = delete;
        Collider& operator=(Collider&& other) noexcept = delete;

        virtual ~Collider() noexcept;

    public:
        physx::PxShape*                                                 shape         = nullptr;
        physx::PxMaterial*                                              material      = nullptr;
        RFKField(Serialize()) bool                                      isTrigger     = false;
        RFKField(Serialize()) bool                                      isVisible     = false;
        RFKField(Serialize(), Inspect("setCenter")) GPM::Vector3        center        = {0.f, 0.f, 0.f};
        RFKField(Serialize(), Inspect("setLocalRotation")) GPM::Vector3 localRotation = {0.f, 0.f, 0.f};
        RFKField(Serialize()) GameObject*                               owner         = nullptr;

        RFKMethod() void setCenter(const GPM::Vec3& newCenter) noexcept;
        RFKMethod() void setLocalRotation(const GPM::Vec3& newLocalRotation) noexcept;
        void             updateTransform() noexcept;
        virtual void     onPostLoad() noexcept
        {
        }

        virtual void updateShape() = 0;

        Collider_GENERATED
    };
} // namespace )
