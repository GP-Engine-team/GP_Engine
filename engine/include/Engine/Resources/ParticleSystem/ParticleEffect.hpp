/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <memory>

namespace GPE
{
class IEffect
{
public:
    static const size_t DEFAULT_PARTICLE_NUM_FLAG = 0; // for initialize method
                                                       // enum Name { };
public:
    IEffect()
    {
    }
    virtual ~IEffect()
    {
    }

    // creates the effect with desired num of particles, (0 means default for the effect)
    virtual bool initialize(size_t numParticles) = 0;
    virtual bool initializeRenderer()            = 0;
    virtual void reset()                         = 0;
    virtual void clean()                         = 0;

    virtual void update(double dt)    = 0;
    virtual void cpuUpdate(double dt) = 0;
    virtual void gpuUpdate(double dt) = 0;
    virtual void render()             = 0;

    virtual int numAllParticles()   = 0;
    virtual int numAliveParticles() = 0;
};
} // namespace GPE