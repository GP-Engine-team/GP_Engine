/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

// NOTE: shouldn't the following headers be in Bullet.rfk.h?
#include <Engine/Serialization/xml/xmlLoader.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>
#include <Engine/Serialization/InspectContext.hpp>
#include <Engine/Serialization/DataInspector.hpp>
// NOTE END
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>

#include <Generated/Bullet.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), Serialize()) Bullet
    {
    protected:
        RFKField(Inspect()) float m_dammage = 0.f;

    public:
        Bullet(float dammage) noexcept;

        void triggered();

        Bullet() noexcept                    = default;
        Bullet(const Bullet& other) noexcept = default;
        Bullet(Bullet&& other) noexcept      = default;
        virtual ~Bullet() noexcept           = default;
        Bullet& operator=(Bullet const& other) noexcept = default;
        Bullet& operator=(Bullet&& other) noexcept = default;

        Bullet_GENERATED
    };
} // namespace )