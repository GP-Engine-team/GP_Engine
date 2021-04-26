/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/ParticleSystem/ParticleData.hpp>
#include <Engine/Resources/ParticleSystem/ParticleEmitter.hpp>
#include <Engine/Resources/ParticleSystem/ParticleUpdater.hpp>
#include <memory>
#include <vector>

namespace GPE
{
class ParticleSystem
{
protected:
    ParticleData m_particles;

    size_t m_count;

    std::vector<std::shared_ptr<ParticleEmitter>> m_emitters;
    std::vector<std::shared_ptr<ParticleUpdater>> m_updaters;

public:
    explicit ParticleSystem(size_t maxCount);
    virtual ~ParticleSystem()
    {
    }

    ParticleSystem(const ParticleSystem&) = delete;
    ParticleSystem& operator=(const ParticleSystem&) = delete;

    virtual void update(double dt);
    virtual void reset();

    virtual size_t numAllParticles() const
    {
        return m_particles.m_count;
    }
    virtual size_t numAliveParticles() const
    {
        return m_particles.m_countAlive;
    }

    void addEmitter(std::shared_ptr<ParticleEmitter> em)
    {
        m_emitters.push_back(em);
    }
    void addUpdater(std::shared_ptr<ParticleUpdater> up)
    {
        m_updaters.push_back(up);
    }

    ParticleData* getData()
    {
        return &m_particles;
    }
};
} // namespace GPE