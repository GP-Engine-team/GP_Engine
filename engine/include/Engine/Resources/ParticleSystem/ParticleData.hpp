/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <GPM/Vector4.hpp>
#include <memory>

namespace GPE
{

/**
 * @brief : Data of Particle own all data relative to the particle
 * @see : Article of Bartłomiej Filipek : https://www.bfilipek.com/2014/04/flexible-particle-system-start.html
 */
class ParticleData
{
public:
    // Buffers (SoA)
    std::unique_ptr<GPM::Vec4[]> m_pos{nullptr};
    std::unique_ptr<GPM::Vec4[]> m_col{nullptr};
    std::unique_ptr<GPM::Vec4[]> m_startCol{nullptr};
    std::unique_ptr<GPM::Vec4[]> m_endCol{nullptr};
    std::unique_ptr<GPM::Vec4[]> m_vel{nullptr};
    std::unique_ptr<GPM::Vec4[]> m_acc{nullptr};
    std::unique_ptr<GPM::Vec4[]> m_time{nullptr};
    std::unique_ptr<bool[]>      m_alive{nullptr};

    size_t m_count{0};
    size_t m_countAlive{0};

public:
    ParticleData()                          = delete;
    ParticleData(const ParticleData& other) = delete;
    ParticleData& operator=(ParticleData const& other) = delete;
    ParticleData(ParticleData&& other)                 = delete;
    ParticleData& operator=(ParticleData&& other) = delete;
    ~ParticleData()                               = default;

    explicit ParticleData(size_t maxCount)
    {
        generate(maxCount);
    }

    ParticleData(const ParticleData&) = delete;
    ParticleData& operator=(const ParticleData&) = delete;

    void generate(size_t maxSize);
    void kill(size_t id);
    void wake(size_t id);
    void swapData(size_t a, size_t b);
};
} // namespace GPE