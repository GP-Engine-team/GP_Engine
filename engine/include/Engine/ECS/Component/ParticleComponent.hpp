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
        float        m_emitRate{0.0};

        std::vector<std::unique_ptr<ParticleGenerator>> m_generators;
        std::vector<std::shared_ptr<ParticleUpdater>>   m_updaters;
        std::shared_ptr<GPE::IParticleRenderer>         m_renderer;

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

        /**
         * @brief Init fontaine effect by default. Work when user use start function
         */
        void initializeDefaultSetting();
        bool initialize(size_t numParticles);
        bool initializeRenderer();
        void reset();
        void clean();

        /**
         * @brief Start the particle effect
         */
        void start();

        /**
         * @brief Update the particle effect and all it's updater
         * @param dt
         */
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

        template <typename T>
        T* getUpdater()
        {
            T* rst = nullptr;
            for (auto&& up : m_updaters)
            {
                if (rst = dynamic_cast<T*>(up.get()))
                    return rst;
            }
            return rst;
        }

        template <typename T>
        void removeUpdater(T & updaterToRemove)
        {
            for (auto&& it = m_updaters.begin(); it != m_updaters.end(); ++it)
            {
                if (it->get() == &updaterToRemove)
                {
                    m_updaters.erase(it);
                    return;
                }
            }
        }

        // calls all the generators and at the end it activates (wakes) particle
        void emit(double dt);

        template <typename T, typename... TArg>
        void addGenerator(TArg... arg)
        {
            for (auto&& generator : m_generators)
            {
                if (dynamic_cast<T*>(generator.get())) // Already exist
                    return;
            }
            m_generators.emplace_back(arg...);
        }

        template <typename T>
        void removeGenerator(T & genToRemove)
        {
            for (auto&& it = m_generators.begin(); it != m_generators.end(); ++it)
            {
                if (it->get() == &genToRemove)
                {
                    m_generators.erase(it);
                    return;
                }
            }
        }

        template <typename T>
        T* getGenerator()
        {
            T* rst = nullptr;
            for (auto&& up : m_generators)
            {
                if (rst = dynamic_cast<T*>(up.get()))
                    return rst;
            }
            return rst;
        }

        void sendDataToShader();

        ParticleComponent_GENERATED
    };
} // namespace )
