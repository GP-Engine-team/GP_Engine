/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <Refureku/Object.h>

#include "Engine/Serialization/InspectContext.hpp"

namespace GPE
{

class IInspectable : public rfk::Object
{
public:
    virtual ~IInspectable()                            = default;
    virtual void inspect(GPE::InspectContext& context) = 0;
};
} // namespace GPE