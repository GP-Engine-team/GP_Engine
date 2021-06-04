/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Resources/Prefab.hpp>

// Generated
#include <Generated/LootManager.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) LootManager : public GPE::BehaviourComponent
    {
        struct RFKStruct(Inspect(), Serialize()) LootSpawnPoint
        {
            RFKField(Inspect(), Serialize()) GPE::Linker<GPE::GameObject> go;

            LootSpawnPoint_GENERATED
        };

    private:
        RFKField(Inspect(), Serialize()) GPE::Prefab* lootPrefab = nullptr;

        RFKField(Inspect(), Serialize()) int m_lootNumbers = 4;

        RFKField(Inspect(), Serialize()) std::vector<LootSpawnPoint>  m_lootPoints{};
        RFKField(Inspect(), Serialize()) GPE::Linker<GPE::GameObject> m_lootsContainer;

        RFKField(Inspect(), Serialize()) bool m_debug = true;

    public:
        /**
         * @brief Construct a new Ranged Entitiess Spawner object
         *
         * @param gameObject
         * @param spawnPosition
         * @param zoneRadius
         * @param spawnDelay
         * @param spawnDelayInterval : spawnDelay will be compute this +/- this intervale.
         */
        LootManager(GPE::GameObject & gameObject);
        LootManager()          = default;
        virtual ~LootManager() = default;

        void start() final;
        void onPostLoad() final;
        void updateEditor(double deltaTime) final;

        LootManager_GENERATED
    };

} // namespace )