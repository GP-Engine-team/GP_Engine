/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

namespace GPE
{
class ParticleRenderer
{
protected:
    class ParticleComponent* m_system{nullptr};

    unsigned int m_bufPos{0};
    unsigned int m_bufCol{0};
    unsigned int m_vao{0};

public:
    ParticleRenderer()
    {
    }

    ~ParticleRenderer()
    {
        destroy();
    }

    void generate(class ParticleComponent* sys, bool useQuads);
    void destroy();
    void update();

    unsigned int getID()
    {
        return m_vao;
    }
};
} // namespace GPE