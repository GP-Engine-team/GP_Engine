/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>

#include "Firearm.hpp"

#include <Generated/PPSH41.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(ComponentGen(), Serialize(), Inspect()) PPSH41 : public Firearm
    {
    public:
        PPSH41(GPE::GameObject & owner) noexcept;

        PPSH41()                    noexcept = default;
        virtual ~PPSH41() noexcept = default;

        PPSH41_GENERATED
    };
} // namespace )
