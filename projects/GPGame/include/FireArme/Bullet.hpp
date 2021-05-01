/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Serialization/Inspect.hpp"
#include "Engine/Serialization/Serialize.hpp"

#include "Generated/Bullet.rfk.h"

namespace GPG RFKNamespace()
{

    class RFKClass(Inspect(), Serialize()) Bullet
    {
    protected:
        RFKField(Inspect(), Serialize()) float m_dammage = 0.f;

    public:
        Bullet(float dammage) noexcept : m_dammage{dammage}
        {
        }

        void triggered()
        {
        }

        Bullet() noexcept                    = default;
        Bullet(const Bullet& other) noexcept = default;
        Bullet(Bullet && other) noexcept     = default;
        virtual ~Bullet() noexcept           = default;
        Bullet& operator=(Bullet const& other) noexcept = default;
        Bullet& operator=(Bullet&& other) noexcept = default;

        Bullet_GENERATED
    };
} // namespace )
