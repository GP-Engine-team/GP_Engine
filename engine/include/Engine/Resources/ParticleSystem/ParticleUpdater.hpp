/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/ParticleSystem/ParticleData.hpp"
#include <vector>

namespace GPE
{
class ParticleUpdater
{
public:
    ParticleUpdater()
    {
    }
    virtual ~ParticleUpdater()
    {
    }

    virtual void    update(double dt, ParticleData* p) = 0;
    virtual uint8_t getRequiereConfig() const          = 0;
};

class EulerUpdater : public ParticleUpdater
{
public:
    GPM::Vec4 m_globalAcceleration;

public:
    EulerUpdater() : m_globalAcceleration(0.0)
    {
    }

    virtual void update(double dt, ParticleData* p) override;

    uint8_t getRequiereConfig() const override;
};

// collision with the floor :) todo: implement a collision model
class FloorUpdater : public ParticleUpdater
{
public:
    float m_floorY;
    float m_bounceFactor;

public:
    FloorUpdater() : m_floorY(0.0), m_bounceFactor(0.5f)
    {
    }

    virtual void update(double dt, ParticleData* p) override;

    uint8_t getRequiereConfig() const override;
};

class AttractorUpdater : public ParticleUpdater
{
protected:
    std::vector<GPM::Vec4> m_attractors; // .w is force
public:
    virtual void update(double dt, ParticleData* p) override;

    uint8_t getRequiereConfig() const override;

    size_t collectionSize() const
    {
        return m_attractors.size();
    }
    void add(const GPM::Vec4& attr)
    {
        m_attractors.push_back(attr);
    }
    GPM::Vec4& get(size_t id)
    {
        return m_attractors[id];
    }
};

class BasicColorUpdater : public ParticleUpdater
{
public:
    virtual void update(double dt, ParticleData* p) override;

    uint8_t getRequiereConfig() const override;
};

class PosColorUpdater : public ParticleUpdater
{
public:
    GPM::Vec4 m_minPos;
    GPM::Vec4 m_maxPos;

public:
    PosColorUpdater() : m_minPos(0.0), m_maxPos(1.0)
    {
    }

    virtual void update(double dt, ParticleData* p) override;

    uint8_t getRequiereConfig() const override;
};

class VelColorUpdater : public ParticleUpdater
{
public:
    GPM::Vec4 m_minVel;
    GPM::Vec4 m_maxVel;

public:
    VelColorUpdater() : m_minVel(0.0), m_maxVel(1.0)
    {
    }

    virtual void update(double dt, ParticleData* p) override;

    uint8_t getRequiereConfig() const override;
};

class BasicTimeUpdater : public ParticleUpdater
{
public:
    virtual void update(double dt, ParticleData* p) override;

    uint8_t getRequiereConfig() const override;
};

} // namespace GPE