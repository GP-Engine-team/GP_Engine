/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/ParticleSystem/ParticleData.hpp>
#include <GPM/Vector4.hpp>

namespace GPE
{

/**
 * @brief : Particle generator manage generation of particle data. It is stored into particle emitter than call it
 * during update
 * @see : Article of Bartłomiej Filipek : https://www.bfilipek.com/2014/04/flexible-particle-system-start.html
 */
class ParticleGenerator
{
public:
    ParticleGenerator()
    {
    }
    virtual ~ParticleGenerator()
    {
    }

    virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) = 0;
};

class BoxPosGen : public ParticleGenerator
{
public:
    GPM::Vec4 m_pos{0.0};
    GPM::Vec4 m_maxStartPosOffset{0.0};

public:
    BoxPosGen()
    {
    }

    virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;
};

class RoundPosGen : public ParticleGenerator
{
public:
    GPM::Vec4 m_center{0.0};
    float     m_radX{0.0};
    float     m_radY{0.0};

public:
    RoundPosGen()
    {
    }
    RoundPosGen(const GPM::Vec4& center, double radX, double radY)
        : m_center(center), m_radX((float)radX), m_radY((float)radY)
    {
    }

    virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;
};

class BasicColorGen : public ParticleGenerator
{
public:
    GPM::Vec4 m_minStartCol{0.0};
    GPM::Vec4 m_maxStartCol{0.0};
    GPM::Vec4 m_minEndCol{0.0};
    GPM::Vec4 m_maxEndCol{0.0};

public:
    BasicColorGen()
    {
    }

    virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;
};

class BasicVelGen : public ParticleGenerator
{
public:
    GPM::Vec4 m_minStartVel{0.0};
    GPM::Vec4 m_maxStartVel{0.0};

public:
    BasicVelGen()
    {
    }

    virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;
};

class SphereVelGen : public ParticleGenerator
{
public:
    float m_minVel{0.0f};
    float m_maxVel{0.0f};

public:
    SphereVelGen()
    {
    }

    virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;
};

class BasicTimeGen : public ParticleGenerator
{
public:
    float m_minTime{0.0};
    float m_maxTime{0.0};

public:
    BasicTimeGen()
    {
    }

    virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;
};

} // namespace GPE