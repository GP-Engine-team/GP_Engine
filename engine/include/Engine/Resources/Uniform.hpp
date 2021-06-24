/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *  found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/Shader.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>
#include <Refureku/Object.h>
#include <gpm/Vector2.hpp>
#include <gpm/Vector3.hpp>

// Generated
#include "Generated/Uniform.rfk.h"

#define UNIFORM_CLASS(type, name)                                                                                      \
    class RFKClass(Serialize(), Inspect()) name##Uniform : public IUniform                                             \
    {                                                                                                                  \
                                                                                                                       \
    public:                                                                                                            \
        RFKField(Serialize(), Inspect()) type data;                                                                    \
                                                                                                                       \
        name##Uniform() = default;                                                                                     \
                                                                                                                       \
        name##Uniform(type newData) : data{data}                                                                       \
        {                                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        void sendToShader(const char* nameUniform, Shader& shader) final                                               \
        {                                                                                                              \
            shader.sendData(nameUniform, data);                                                                        \
        }                                                                                                              \
                                                                                                                       \
        name##Uniform_GENERATED                                                                                        \
    };

namespace GPE RFKNamespace()
{
    class RFKClass(Serialize(false), Inspect(false)) IUniform : public rfk::Object
    {
    public:
        IUniform() = default;

        virtual void sendToShader(const char* nameUniform, Shader& shader) = 0;
        IUniform_GENERATED
    };

    UNIFORM_CLASS(float, float)
    UNIFORM_CLASS(int, int)
    UNIFORM_CLASS(GPM::vec2, vec2)
    UNIFORM_CLASS(GPM::vec3, vec3)

} // namespace )