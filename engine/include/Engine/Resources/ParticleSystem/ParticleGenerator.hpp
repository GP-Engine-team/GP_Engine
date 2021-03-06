﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/ParticleSystem/ParticleData.hpp>
#include <GPM/Vector4.hpp>

#include <Engine/Core/Tools/BinaryMask.hpp>
#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/GPMDataInspector.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/STDDataInspector.hpp>

#include <Engine/Serialization/Serialize.hpp>
#include <Refureku/Object.h>

// Generated
#include <Generated/ParticleGenerator.rfk.h>

namespace GPE RFKNamespace()
{

    /**
     * @brief : Particle generator manage generation of particle data. It is stored into particle emitter than call it
     * during update
     * @see : Article of Bartłomiej Filipek : https://www.bfilipek.com/2014/04/flexible-particle-system-start.html
     */
    class RFKClass(Serialize(false), Inspect(false)) ParticleGenerator : public rfk::Object
    {
    public:
        ParticleGenerator()
        {
        }
        virtual ~ParticleGenerator()
        {
        }

        virtual void generate(ParticleData * p, size_t startId, size_t endId) = 0;

        virtual U16BMask getRequiereConfig() const = 0;

        ParticleGenerator_GENERATED
    };

    class RFKClass(Serialize(), Inspect()) BoxPosGen : public ParticleGenerator
    {
    public:
        RFKField(Serialize(), Inspect()) GPM::Vec3 m_pos{0.0};
        RFKField(Serialize(), Inspect()) GPM::Vec3 m_maxStartPosOffset{0.0};

    public:
        BoxPosGen()
        {
        }

        virtual void generate(ParticleData * p, size_t startId, size_t endId) override;

        U16BMask getRequiereConfig() const override;

        BoxPosGen_GENERATED
    };

    class RFKClass(Serialize(), Inspect()) SizeGen : public ParticleGenerator
    {
    public:
        RFKField(Serialize(), Inspect()) float m_minStartSize{1.0};
        RFKField(Serialize(), Inspect()) float m_maxStartSize{20.0};
        RFKField(Serialize(), Inspect()) float m_minEndSize{2.0};
        RFKField(Serialize(), Inspect()) float m_maxEndSize{22.0};

    public:
        SizeGen()
        {
        }

        virtual void generate(ParticleData * p, size_t startId, size_t endId) override;

        U16BMask getRequiereConfig() const override;

        SizeGen_GENERATED
    };

    class RFKClass(Serialize(), Inspect()) RoundPosGen : public ParticleGenerator
    {
    public:
        RFKField(Serialize(), Inspect()) GPM::Vec3 m_center{0.0};
        RFKField(Serialize(), Inspect()) float     m_radX{0.0};
        RFKField(Serialize(), Inspect()) float     m_radY{0.0};

    public:
        RoundPosGen()
        {
        }
        RoundPosGen(const GPM::Vec3& center, double radX, double radY)
            : m_center(center), m_radX((float)radX), m_radY((float)radY)
        {
        }

        virtual void generate(ParticleData * p, size_t startId, size_t endId) override;

        U16BMask getRequiereConfig() const override;

        RoundPosGen_GENERATED
    };

    class RFKClass(Serialize(), Inspect()) BasicColorGen : public ParticleGenerator
    {
    public:
        struct CreateArg
        {
            RGBA minStartCol{0.0};
            RGBA maxStartCol{0.0};
            RGBA minEndCol{0.0};
            RGBA maxEndCol{0.0};
        };

    public:
        RFKField(Serialize(), Inspect()) RGBA m_minStartCol{0.0};
        RFKField(Serialize(), Inspect()) RGBA m_maxStartCol{0.0};
        RFKField(Serialize(), Inspect()) RGBA m_minEndCol{0.0};
        RFKField(Serialize(), Inspect()) RGBA m_maxEndCol{0.0};

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

        virtual void generate(ParticleData * p, size_t startId, size_t endId) override;

        U16BMask getRequiereConfig() const override;

        BasicColorGen_GENERATED
    };

    class RFKClass(Serialize(), Inspect()) BasicVelGen : public ParticleGenerator
    {
    public:
        RFKField(Serialize(), Inspect()) GPM::Vec4 m_minStartVel{0.0};
        RFKField(Serialize(), Inspect()) GPM::Vec4 m_maxStartVel{0.0};

    public:
        BasicVelGen()
        {
        }

        virtual void generate(ParticleData * p, size_t startId, size_t endId) override;

        U16BMask getRequiereConfig() const override;

        BasicVelGen_GENERATED
    };

    //|Generate velocity with more change to obtain vector in the center
    class RFKClass(Serialize(), Inspect()) ConeVelGen : public ParticleGenerator
    {
        // Normalized
        RFKField(Serialize(), Inspect()) GPM::Vec3 m_direction{0.0};

        // radius after firection
        RFKField(Serialize(), Inspect()) float m_radiusVel{0.0};

        // velocity factor
        RFKField(Serialize(), Inspect()) float m_velStrength{0.0};

    public:
        ConeVelGen()
        {
        }

        virtual void generate(ParticleData * p, size_t startId, size_t endId) override;

        U16BMask getRequiereConfig() const override;

        ConeVelGen_GENERATED
    };

    class RFKClass(Serialize(), Inspect()) SphereVelGen : public ParticleGenerator
    {
    public:
        RFKField(Serialize(), Inspect()) float m_minVel{0.0f};
        RFKField(Serialize(), Inspect()) float m_maxVel{0.0f};

    public:
        SphereVelGen()
        {
        }

        virtual void generate(ParticleData * p, size_t startId, size_t endId) override;

        U16BMask getRequiereConfig() const override;

        SphereVelGen_GENERATED
    };

    class RFKClass(Serialize(), Inspect()) VelFromPosGen : public ParticleGenerator
    {
    public:
        RFKField(Serialize(), Inspect()) GPM::Vec4 m_offset;
        RFKField(Serialize(), Inspect()) float     m_minScale;
        RFKField(Serialize(), Inspect()) float     m_maxScale;

    public:
        VelFromPosGen() : m_offset(0.0), m_minScale(0.0), m_maxScale(0.0)
        {
        }
        VelFromPosGen(const GPM::Vec4& off, double minS, double maxS)
            : m_offset(off), m_minScale((float)minS), m_maxScale((float)maxS)
        {
        }

        virtual void generate(ParticleData * p, size_t startId, size_t endId) override;

        U16BMask getRequiereConfig() const override;

        VelFromPosGen_GENERATED
    };

    class RFKClass(Serialize(), Inspect()) BasicTimeGen : public ParticleGenerator
    {
    public:
        RFKField(Serialize(), Inspect()) float m_minTime{0.0};
        RFKField(Serialize(), Inspect()) float m_maxTime{0.0};

    public:
        BasicTimeGen()
        {
        }

        virtual void generate(ParticleData * p, size_t startId, size_t endId) override;

        U16BMask getRequiereConfig() const override;

        BasicTimeGen_GENERATED
    };

} // namespace )