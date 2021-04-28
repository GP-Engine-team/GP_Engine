/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/ParticleSystem/ParticleData.hpp>
#include <GPM/Vector4.hpp>

#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/GPMDataInspector.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/STDDataInspector.hpp>

// Generated
#include <Generated/ParticleGenerator.rfk.h>

namespace GPE RFKNamespace()
{

    /**
     * @brief : Particle generator manage generation of particle data. It is stored into particle emitter than call it
     * during update
     * @see : Article of Bartłomiej Filipek : https://www.bfilipek.com/2014/04/flexible-particle-system-start.html
     */
    class RFKClass(Inspect()) ParticleGenerator
    {
    public:
        ParticleGenerator()
        {
        }
        virtual ~ParticleGenerator()
        {
        }

        virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) = 0;

        virtual uint16_t getRequiereConfig() const = 0;

        ParticleGenerator_GENERATED
    };

    class RFKClass(Inspect()) BoxPosGen : public ParticleGenerator
    {
    public:
        RFKField(Inspect()) GPM::Vec4 m_pos{0.0};
        RFKField(Inspect()) GPM::Vec4 m_maxStartPosOffset{0.0};

    public:
        BoxPosGen()
        {
        }

        virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;

        uint16_t getRequiereConfig() const override;

        BoxPosGen_GENERATED
    };

    class RFKClass(Inspect()) RoundPosGen : public ParticleGenerator
    {
    public:
        RFKField(Inspect()) GPM::Vec4 m_center{0.0};
        RFKField(Inspect()) float     m_radX{0.0};
        RFKField(Inspect()) float     m_radY{0.0};

    public:
        RoundPosGen()
        {
        }
        RoundPosGen(const GPM::Vec4& center, double radX, double radY)
            : m_center(center), m_radX((float)radX), m_radY((float)radY)
        {
        }

        virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;

        uint16_t getRequiereConfig() const override;

        RoundPosGen_GENERATED
    };

    class RFKClass(Inspect()) BasicColorGen : public ParticleGenerator
    {
    public:
        struct CreateArg
        {
            GPM::Vec4 minStartCol{0.0};
            GPM::Vec4 maxStartCol{0.0};
            GPM::Vec4 minEndCol{0.0};
            GPM::Vec4 maxEndCol{0.0};
        };

    public:
        RFKField(Inspect()) GPM::Vec4 m_minStartCol{0.0};
        RFKField(Inspect()) GPM::Vec4 m_maxStartCol{0.0};
        RFKField(Inspect()) GPM::Vec4 m_minEndCol{0.0};
        RFKField(Inspect()) GPM::Vec4 m_maxEndCol{0.0};

    public:
        BasicColorGen()
        {
        }

        BasicColorGen(const CreateArg& arg)
        {
            m_minStartCol = arg.minEndCol;
            m_maxStartCol = arg.minStartCol;
            m_minEndCol   = arg.minEndCol;
            m_maxEndCol   = arg.maxEndCol;
        }

        virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;

        uint16_t getRequiereConfig() const override;

        BasicColorGen_GENERATED
    };

    class RFKClass(Inspect()) BasicVelGen : public ParticleGenerator
    {
    public:
        RFKField(Inspect()) GPM::Vec4 m_minStartVel{0.0};
        RFKField(Inspect()) GPM::Vec4 m_maxStartVel{0.0};

    public:
        BasicVelGen()
        {
        }

        virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;

        uint16_t getRequiereConfig() const override;

        BasicVelGen_GENERATED
    };

    class RFKClass(Inspect()) SphereVelGen : public ParticleGenerator
    {
    public:
        RFKField(Inspect()) float m_minVel{0.0f};
        RFKField(Inspect()) float m_maxVel{0.0f};

    public:
        SphereVelGen()
        {
        }

        virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;

        uint16_t getRequiereConfig() const override;

        SphereVelGen_GENERATED
    };

    class RFKClass(Inspect()) VelFromPosGen : public ParticleGenerator
    {
    public:
        RFKField(Inspect()) GPM::Vec4 m_offset;
        RFKField(Inspect()) float     m_minScale;
        RFKField(Inspect()) float     m_maxScale;

    public:
        VelFromPosGen() : m_offset(0.0), m_minScale(0.0), m_maxScale(0.0)
        {
        }
        VelFromPosGen(const GPM::Vec4& off, double minS, double maxS)
            : m_offset(off), m_minScale((float)minS), m_maxScale((float)maxS)
        {
        }

        virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;

        uint16_t getRequiereConfig() const override;

        VelFromPosGen_GENERATED
    };

    class RFKClass(Inspect()) BasicTimeGen : public ParticleGenerator
    {
    public:
        RFKField(Inspect()) float m_minTime{0.0};
        RFKField(Inspect()) float m_maxTime{0.0};

    public:
        BasicTimeGen()
        {
        }

        virtual void generate(double dt, ParticleData* p, size_t startId, size_t endId) override;

        uint16_t getRequiereConfig() const override;

        BasicTimeGen_GENERATED
    };

} // namespace )