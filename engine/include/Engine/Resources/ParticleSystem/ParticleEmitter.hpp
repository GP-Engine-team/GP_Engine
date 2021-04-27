/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/ParticleSystem/ParticleData.hpp>
#include <Engine/Resources/ParticleSystem/ParticleGenerator.hpp>
#include <Engine/Serialization/DataInspector.hpp>

#include <memory>
#include <vector>

namespace GPE
{

class ParticleEmitter
{
public:
    std::vector<std::unique_ptr<ParticleGenerator>> m_generators;
    float                                           m_emitRate{0.0};

public:
    ParticleEmitter()
    {
    }
    virtual ~ParticleEmitter()
    {
    }

    // calls all the generators and at the end it activates (wakes) particle
    virtual void emit(double dt, ParticleData* p);

    template <typename T, typename... TArg>
    void addGenerator(TArg... arg)
    {
        for (auto&& generator : m_generators)
        {
            if (dynamic_cast<T>(generator)) // Already exist
                return;
        }
        m_generators.emplace_back(arg...);
    }

    template <typename T>
    void removeGenerator()
    {
        for (auto&& it = m_generators.begin(); it != m_generators.end(); ++it)
        {
            if (dynamic_cast<T>(*it)) // Already exist
            {
                m_generators.erase(it);
                return;
            }
        }
    }
};
/*
template <>
void DataInspector::inspect(GPE::InspectContext& context, ParticleEmitter& inspected);
*/
} // namespace GPE