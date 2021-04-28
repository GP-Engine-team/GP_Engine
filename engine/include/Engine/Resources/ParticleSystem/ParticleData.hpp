/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Core/Tools/BinaryMask.hpp>
#include <Engine/Resources/Color.hpp>
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
    enum EParam : uint16_t
    {
        POSITION     = (1u << 1),
        COLOR        = (1u << 2),
        START_COLOR  = (1u << 3),
        END_COLOR    = (1u << 4),
        VELOCITY     = (1u << 5),
        ACCELERATION = (1u << 6),
        TIME         = (1u << 7),
    };

public:
    U16BMask                     m_maskType;
    std::unique_ptr<GPM::Vec4[]> m_pos      = nullptr;
    std::unique_ptr<RGBA[]>      m_col      = nullptr;
    std::unique_ptr<RGBA[]>      m_startCol = nullptr;
    std::unique_ptr<RGBA[]>      m_endCol   = nullptr;
    std::unique_ptr<GPM::Vec4[]> m_vel      = nullptr;
    std::unique_ptr<GPM::Vec4[]> m_acc      = nullptr;
    std::unique_ptr<GPM::Vec4[]> m_time     = nullptr;
    std::unique_ptr<bool[]>      m_alive    = nullptr;

    size_t m_count{0};
    size_t m_countAlive{0};

public:
    ParticleData()
    {
    }

    explicit ParticleData(size_t maxCount, U16BMask maskType)
    {
        generate(maxCount, maskType);
    }

    ~ParticleData()
    {
    }

    ParticleData(const ParticleData&) = delete;
    ParticleData& operator=(const ParticleData&) = delete;

    void generate(size_t maxSize, U16BMask maskType);
    void kill(size_t id);
    void wake(size_t id);

    /**
     * @brief Set data b in data a
     * @param a
     * @param b
     */
    void swapData(size_t a, size_t b);

    static void copyOnlyAlive(const ParticleData* source, ParticleData* destination);
};
} // namespace GPE