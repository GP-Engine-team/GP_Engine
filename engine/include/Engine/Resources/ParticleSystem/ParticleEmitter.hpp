/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/ParticleSystem/ParticleData.hpp>
#include <Engine/Resources/ParticleSystem/ParticleGenerator.hpp>

#include <memory>
#include <vector>

namespace GPE
{

class ParticleEmitter
{
protected:
    std::vector<std::shared_ptr<ParticleGenerator>> m_generators;

public:
    float m_emitRate{0.0};

public:
    ParticleEmitter()
    {
    }
    virtual ~ParticleEmitter()
    {
    }

    // calls all the generators and at the end it activates (wakes) particle
    virtual void emit(double dt, ParticleData* p);

    void addGenerator(std::shared_ptr<ParticleGenerator> gen)
    {
        m_generators.push_back(gen);
    }
};

} // namespace GPE