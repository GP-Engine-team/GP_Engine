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
            enableOnGUI(true);
        }

        WorldGenerator() noexcept = default;

        WorldGenerator(const WorldGenerator& other) noexcept = delete;
        WorldGenerator(WorldGenerator && other) noexcept     = delete;
        virtual ~WorldGenerator() noexcept                   = default;

        WorldGenerator& operator=(WorldGenerator const& other) noexcept = delete;
        WorldGenerator& operator=(WorldGenerator&& other) noexcept = delete;

    public:
        void loadTree(class GPE::GameObject & parent, unsigned int number);

        virtual void start()
        {
        }

        void onGUI() final
        {
            if (ImGui::Button("Generate"))
            {
                if (!m_treeContainer || m_treePrefab.isEmpty())
                {
                    GPE::Log::getInstance()->logError("Missing tree container GO or tree prefab");
                    return;
                }
                for (auto&& child : m_treeContainer->children)
                {
                    delete child;
                }
                m_treeContainer->children.clear();

                loadTree(*m_treeContainer, 100);
            }
        }

        WorldGenerator_GENERATED
    };

} // namespace )
