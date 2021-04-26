/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <memory> //std::shared_ptr
#include <vector> //std::vector

#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Serialization/ComponentGen.h"
#include <Engine/Resources/ParticleSystem/ParticleData.hpp>
#include <Engine/Resources/ParticleSystem/ParticleEmitter.hpp>
#include <Engine/Resources/ParticleSystem/ParticleGenerator.hpp>
#include <Engine/Resources/ParticleSystem/ParticleRenderer.hpp>
#include <Engine/Resources/ParticleSystem/ParticleUpdater.hpp>

// Generated
#include "Generated/ParticleComponent.rfk.h"

namespace GPE RFKNamespace()
{
    class RFKClass(ComponentGen) ParticleComponent : public Component
    {
    public:
        struct CreateArg
        {
        };

    protected:
        class Shader* m_shader;

        ParticleData m_particles;
        size_t       m_count;

        std::vector<std::shared_ptr<ParticleEmitter>> m_emitters;
        std::vector<std::shared_ptr<ParticleUpdater>> m_updaters;
        std::shared_ptr<GPE::IParticleRenderer>       m_renderer;

    public:
        ParticleComponent(GameObject & owner);
        ParticleComponent(GameObject & owner, const CreateArg& arg);

        ParticleComponent(const ParticleComponent& other) noexcept = delete;
        ParticleComponent(ParticleComponent && other) noexcept     = default;
        virtual ~ParticleComponent();

        ParticleComponent()        = default;
        ParticleComponent& operator=(ParticleComponent const& other) = delete;
        ParticleComponent& operator=(ParticleComponent&& other) noexcept = default;

        void         moveTowardScene(class Scene & newOwner) override;
        virtual void inspect(InspectContext & context);

        bool initialize(size_t numParticles);
        bool initializeRenderer();
        void reset();
        void clean();

        void update(double dt);

        size_t numAllParticles() const
        {
            return m_particles.m_count;
        }

        size_t numAliveParticles() const
        {
            return m_particles.m_countAlive;
        }

        ParticleData* getData()
        {
            return &m_particles;
        }

        unsigned int getMeshID()
        {
            return m_renderer->getID();
        }

        Shader* getShader()
        {
            return m_shader;
        }

        void sendDataToShader();

        ParticleComponent_GENERATED
    };
} // namespace )
