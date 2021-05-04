/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/ParticleSystem/ParticleData.hpp>
#include <vector>

#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/GPMDataInspector.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/STDDataInspector.hpp>

#include <Engine/Core/Tools/BinaryMask.hpp>

// Generated
#include <Generated/ParticleUpdater.rfk.h>

namespace GPE RFKNamespace()
{
    class RFKClass(Inspect()) ParticleUpdater
    {
    public:
        ParticleUpdater()
        {
        }

        virtual ~ParticleUpdater()
        {
        }

        virtual void     update(double dt, ParticleData* p) = 0;
        virtual U16BMask getRequiereConfig() const          = 0;

        ParticleUpdater_GENERATED
    };

    class RFKClass(Inspect()) EulerUpdater : public ParticleUpdater
    {
    public:
        RFKField(Inspect()) GPM::Vec4 m_globalAcceleration;

    public:
        EulerUpdater() : m_globalAcceleration(0.0)
        {
        }

        virtual void update(double dt, ParticleData* p) override;

        U16BMask getRequiereConfig() const override;

        EulerUpdater_GENERATED
    };

    // collision with the floor :) todo: implement a collision model
    class RFKClass(Inspect()) FloorUpdater : public ParticleUpdater
    {
    public:
        RFKField(Inspect()) float m_floorY;
        RFKField(Inspect()) float m_bounceFactor;

    public:
        FloorUpdater() : m_floorY(0.0), m_bounceFactor(0.5f)
        {
        }

        virtual void update(double dt, ParticleData* p) override;

        U16BMask getRequiereConfig() const override;

        FloorUpdater_GENERATED
    };

    class RFKClass(Inspect()) AttractorUpdater : public ParticleUpdater
    {
    protected:
        RFKField(Inspect()) std::vector<GPM::Vec4> m_attractors; // .w is force
    public:
        virtual void update(double dt, ParticleData* p) override;

        U16BMask getRequiereConfig() const override;

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

        AttractorUpdater_GENERATED
    };

    class RFKClass(Inspect()) BasicColorUpdater : public ParticleUpdater
    {
    public:
        virtual void update(double dt, ParticleData* p) override;

        U16BMask getRequiereConfig() const override;

        BasicColorUpdater_GENERATED
    };

    class RFKClass(Inspect()) PosColorUpdater : public ParticleUpdater
    {
    public:
        RFKField(Inspect()) GPM::Vec4 m_minPos;
        RFKField(Inspect()) GPM::Vec4 m_maxPos;

    public:
        PosColorUpdater() : m_minPos(0.0), m_maxPos(1.0)
        {
        }

        virtual void update(double dt, ParticleData* p) override;

        U16BMask getRequiereConfig() const override;

        PosColorUpdater_GENERATED
    };

    class RFKClass(Inspect()) VelColorUpdater : public ParticleUpdater
    {
    public:
        RFKField(Inspect()) GPM::Vec4 m_minVel;
        RFKField(Inspect()) GPM::Vec4 m_maxVel;

    public:
        VelColorUpdater() : m_minVel(0.0), m_maxVel(1.0)
        {
        }

        virtual void update(double dt, ParticleData* p) override;

        U16BMask getRequiereConfig() const override;

        VelColorUpdater_GENERATED
    };

    class RFKClass(Inspect()) BasicTimeUpdater : public ParticleUpdater
    {
    public:
        virtual void update(double dt, ParticleData* p) override;

        U16BMask getRequiereConfig() const override;

        BasicTimeUpdater_GENERATED
    };

} // namespace )