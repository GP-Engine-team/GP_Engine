/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/Prefab.hpp>
#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/Inspect.hpp>

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <memory>

#include "Generated/WorldGenerator.rfk.h"

namespace GPG RFKNamespace()
{
    class RFKClass(ComponentGen, Serialize()) WorldGenerator : public GPE::BehaviourComponent
    {
    private:
        RFKField(Serialize()) GPE::Prefab*      m_prefab = nullptr;
        RFKField(Serialize()) GPE::Linker<GPE::GameObject> m_container;

        RFKField(Serialize()) float     m_radius   = 10.f;
        RFKField(Serialize()) GPM::Vec3 m_minScale = GPM::Vec3::one();
        RFKField(Serialize()) GPM::Vec3 m_maxScale = GPM::Vec3::one();

        RFKField(Serialize()) GPM::Vec3 m_minRot = GPM::Vec3{0.f, 360.f, 0.f};
        RFKField(Serialize()) GPM::Vec3 m_maxRot = GPM::Vec3{0.f, 360.f, 0.f};

        RFKField(Serialize()) int  m_number = 15;
        RFKField(Serialize()) bool m_debug  = true;

    public:
        WorldGenerator(GPE::GameObject & owner) noexcept : GPE::BehaviourComponent(owner)
        {
            onPostLoad();
        }

        WorldGenerator() noexcept = default;

        WorldGenerator(const WorldGenerator& other) noexcept = delete;
        WorldGenerator(WorldGenerator && other) noexcept     = delete;
        virtual ~WorldGenerator() noexcept                   = default;

        WorldGenerator& operator=(WorldGenerator const& other) noexcept = delete;
        WorldGenerator& operator=(WorldGenerator&& other) noexcept = delete;

    public:
        void loadCircularCoordinate();

        virtual void start()
        {
        }

        void onPostLoad() final;
        void updateEditor(double deltaTime) final;

        void inspect(GPE::InspectContext & context) override;

        WorldGenerator_GENERATED
    };

} // namespace )
