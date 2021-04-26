/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <memory>

namespace GPE
{
class ParticleComponent;

class IParticleRenderer
{
public:
    IParticleRenderer()
    {
    }
    virtual ~IParticleRenderer()
    {
    }

    virtual void generate(ParticleComponent* sys, bool useQuads) = 0;
    virtual void destroy()                                       = 0;
    virtual void update()                                        = 0;

    virtual unsigned int getID() = 0;
};

class RendererFactory
{
public:
    static std::shared_ptr<IParticleRenderer> create(const char* name);
};

} // namespace GPE