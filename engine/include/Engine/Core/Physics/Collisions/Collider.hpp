﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
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
    class GameObject;

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
        physx::PxShape*                                                     shape         = nullptr;
        physx::PxMaterial*                                                  material      = nullptr;
        /*RFKField(Inspect(), Serialize())*/ bool                           isTrigger     = false;
        /*RFKField(Inspect(), Serialize())*/ bool                           isVisible     = false;
        /* RFKField(Inspect("setCenter"), Serialize())*/ GPM::Vector3       center        = {0.f, 0.f, 0.f};
        /*RFKField(Inspect("setLocalRotation"), Serialize())*/ GPM::Vector3 localRotation = {0.f, 0.f, 0.f};

        void        setCenter(const GPM::Vec3& newCenter) noexcept;
        void        setLocalRotation(const GPM::Vec3& newLocalRotation) noexcept;
        void        updateTransform() noexcept;
        GameObject* owner = nullptr;

        Collider_GENERATED
    };
} // namespace )
