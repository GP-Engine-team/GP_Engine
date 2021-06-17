/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *  found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>
#include <Refureku/Object.h>

// Generated
#include "Generated/Uniform.rfk.h"

#define UNIFORM_CLASS(type)                                                                                            \
    class RFKClass(Serialize(), Inspect()) type##Uniform : public IUniform                                             \
    {                                                                                                                  \
                                                                                                                       \
    public:                                                                                                            \
        type##Uniform(type newData) : data{data}                                                                       \
        {                                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        RFKField(Serialize(), Inspect()) type data;                                                                    \
        type##Uniform_GENERATED                                                                                        \
    };

namespace GPE RFKNamespace()
{
    class RFKClass(Serialize(false), Inspect(false)) IUniform{public : IUniform_GENERATED};

    UNIFORM_CLASS(float)
    UNIFORM_CLASS(int)

} // namespace )