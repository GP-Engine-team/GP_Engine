/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Serialization/InspectContext.hpp>
#include <Engine/Serialization/xml/xmlLoader.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <GPM/Vector3.hpp>
#include <PxMaterial.h>
#include <PxShape.h>
#include <Refureku/Refureku.h>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>

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

        virtual ~Collider() noexcept = default;

    public:
        physx::PxShape*    shape     = nullptr;
        physx::PxMaterial* material  = nullptr;
        bool               isTrigger = false;
        bool               isVisible = false;
        GPM::Vector3       center    = {0.f, 0.f, 0.f};

        Collider_GENERATED
    };
} // namespace )
