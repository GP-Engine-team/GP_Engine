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
#include "Engine/Serialization/Serialize.hpp"
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/Resources/ParticleSystem/ParticleData.hpp>
#include <Engine/Resources/ParticleSystem/ParticleGenerator.hpp>
#include <Engine/Resources/ParticleSystem/ParticleRenderer.hpp>
#include <Engine/Resources/ParticleSystem/ParticleUpdater.hpp>
#include <gpm/Matrix4.hpp>
#include <gpm/Vector4.hpp>

// Generated
#include "Generated/ParticleComponent.rfk.h"

namespace GPE RFKNamespace()
{
    class GameObject;
    class Shader;

    class RFKClass(ComponentGen, Serialize()) ParticleComponent : public Component
    {
    public:
        struct CreateArg
        {
        };

    protected:
        RFKField(Serialize()) Shader* m_shader = nullptr;

        // TODO : Line hardcoded : used material with texture instead
        RFKField(Serialize()) Texture* m_diffuseTexture = nullptr;

        ParticleData                 m_particles;
        RFKField(Serialize()) size_t m_count                  = 0;
        RFKField(Serialize()) float  m_emitRate               = 0.0;
        RFKField(Serialize()) float  m_duration               = std::numeric_limits<float>::infinity();
        RFKField(Serialize()) float  m_durationCount          = 0.f;
        RFKField(Serialize()) bool   m_canEmit                = false;
        RFKField(Serialize()) bool   m_useGlobalPosition      = true;
        RFKField(Serialize()) bool   m_useGameObjectTransform = false;

        /**
         * @brief Is used to define how the particle must be generated (color ? velocity ? Position ?)
         */
        RFKField(Serialize()) std::vector<std::unique_ptr<ParticleGenerator>> m_generators;

        /**
         * @brief Us used to define how the particle must be update (life time ? acceleration ? color changement ?)
         */
        RFKField(Serialize()) std::vector<std::unique_ptr<ParticleUpdater>> m_updaters;
        std::unique_ptr<GPE::ParticleRenderer>                              m_renderer = nullptr;

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

        /**
         * @brief Init render buffer
         */
        void generate();

        virtual void updateToSystem() noexcept override;

    public:
        ParticleComponent(GameObject & owner);
        ParticleComponent(GameObject & owner, const CreateArg& arg);

        virtual ~ParticleComponent();

        ParticleComponent();

        void         moveTowardScene(class Scene & newOwner) override;
        virtual void inspect(InspectContext & context);

        DEFAULT_GETTER_SETTER_BY_VALUE(Duration, m_duration)
        DEFAULT_GETTER_SETTER_BY_VALUE(Count, m_count)
        DEFAULT_GETTER_SETTER_BY_VALUE(EmitRate, m_emitRate)

        /**
         * @brief Kill all alive particle
         */
        void reset();

        /**
         * @brief Start the particle effect
         */
        void start();

        /**
         * @brief Emite a specific number of particle
         * @param count
         */
        void emit(unsigned int count);

        /**
         * @brief Emit particle at position
         * @param count
         */
        void emitAt(const GPM::Vec3& position, unsigned int count);

        /**
         * @brief Emit particle thank to model matrix (use transform model of gameObject)
         * @param model
         * @param count
         */
        void emitAt(const GPM::Mat4& model, unsigned int count);

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

        inline Shader*  getShader();
        inline Texture* getTexture();

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

        ParticleComponent_GENERATED
    };
} // namespace )

#include <Engine/Resources/ParticleSystem/ParticleComponent.inl>
