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
#include <memory>

#include "Generated/WorldGenerator.rfk.h"

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) WorldGenerator : public GPE::BehaviourComponent
    {
    private:
        RFKField(Inspect(), Serialize()) GPE::Prefab      m_treePrefab;
        RFKField(Inspect(), Serialize()) GPE::GameObject* m_treeContainer = nullptr;

    public:
        WorldGenerator(GPE::GameObject & owner) noexcept : GPE::BehaviourComponent(owner)
        {
        }

        WorldGenerator() noexcept                            = default;
        WorldGenerator(const WorldGenerator& other) noexcept = delete;
        WorldGenerator(WorldGenerator && other) noexcept     = delete;
        virtual ~WorldGenerator() noexcept                   = default;

        WorldGenerator& operator=(WorldGenerator const& other) noexcept = delete;
        WorldGenerator& operator=(WorldGenerator&& other) noexcept = delete;

    public:
        void loadTree(class GPE::GameObject & parent, unsigned int number);

        virtual void start()
        {
            if (!m_treeContainer)
            {
                GPE::Log::getInstance()->logError("Missing tree container GO");
                return;
            }

            loadTree(*m_treeContainer, 100);
        }

        WorldGenerator_GENERATED
    };

} // namespace )
