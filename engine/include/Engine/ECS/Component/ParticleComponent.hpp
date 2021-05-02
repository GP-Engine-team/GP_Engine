/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <memory> //std::unique_ptr
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
        class Shader* m_shader = nullptr;

        ParticleData m_particles;
        size_t       m_count    = 0;
        float        m_emitRate = 0.0;

        /**
         * @brief Is used to define how the particle must be generated (color ? velocity ? Position ?)
         */
        std::vector<std::unique_ptr<ParticleGenerator>> m_generators;

        /**
         * @brief Us used to define how the particle must be update (life time ? acceleration ? color changement ?)
         */
        std::vector<std::unique_ptr<ParticleUpdater>> m_updaters;
        std::unique_ptr<GPE::ParticleRenderer>        m_renderer = nullptr;

    protected:
        /**
         * @brief Init fontaine effect by default. Work when user use start function
         */
        void initializeDefaultSetting();

        /**
         * @brief Init render buffer and GPU data
         */
        void initializeRenderer();

        /**
         * @brief calls all the generators and at the end it activates (wakes) particle
         * @param dt
         */
        void emit(double dt);

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
         * @brief Kill all alive particle
         */
        void reset();

        /**
         * @brief Start the particle effect
         */
        void start();

        /**
         * @brief Update the particle effect and all it's updater
         * @param dt
         */
        void update(double dt);

        /**
         * @brief Get total particule count allocated. Alive and dead particule
         * @return
         */
        inline size_t numAllParticles() const;

        /**
         * @brief Get alive particule count only
         * @return
         */
        inline size_t numAliveParticles() const;

        /**
         * @brief Get Particle data that contain buffer of data used to compute
         * @return
         */
        inline const ParticleData* getData() const;

        inline unsigned int getMeshID();

        inline Shader* getShader();

        /**
         * @brief Try to add specific updater if its type doesn't exist
         * @tparam T
         * @param updaterToRemove
         */
        template <typename T, typename... TArg>
        void addUpdater(TArg && ... arg);

        /**
         * @brief Try to find specific updater
         * @tparam T
         * @return
         */
        template <typename T>
        T* getUpdater();

        /**
         * @brief Try to remove specific updater
         * @tparam T
         * @param updaterToRemove
         */
        template <typename T>
        void removeUpdater(T & updaterToRemove);

        /**
         * @brief Try to add specific generator if its type doesn't exist
         * @tparam T
         * @param updaterToRemove
         */
        template <typename T, typename... TArg>
        void addGenerator(TArg && ... arg);

        /**
         * @brief Try to find specific generator
         * @tparam T
         * @return
         */
        template <typename T>
        T* getGenerator();

        /**
         * @brief Try to remove specific generator
         * @tparam T
         * @param genToRemove
         */
        template <typename T>
        void removeGenerator(T & genToRemove);

        /**
         * @brief Update GPU data with CPU buffer
         */
        void sendDataToShader();

        /**
         * @brief Add or remove current component from it's system which have for effect to enable or disable it
         * @param newState
         * @return
         */
        void setActive(bool newState) noexcept final;

        ParticleComponent_GENERATED
    };
} // namespace )

#include <Engine/Resources/ParticleSystem/ParticleComponent.inl>