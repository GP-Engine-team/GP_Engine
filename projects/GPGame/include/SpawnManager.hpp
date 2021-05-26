/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Prefab.hpp>

// Generated
#include <Generated/SpawnManager.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) SpawnManager : public GPE::BehaviourComponent
    {
        struct RFKStruct(Inspect(), Serialize()) EntityPrefab
        {
            RFKField(Inspect(), Serialize()) GPE::Prefab* prefab = nullptr;

            EntityPrefab_GENERATED
        };

        struct RFKStruct(Inspect(), Serialize()) Spawner
        {
            RFKField(Inspect(), Serialize()) GPE::GameObjectLinker go;

            Spawner_GENERATED
        };

    private:
        RFKField(Inspect(), Serialize()) std::vector<Spawner>      m_spawners{};
        RFKField(Inspect(), Serialize()) std::vector<EntityPrefab> m_entitiesToSpawnInfo{};

        RFKField(Inspect(), Serialize()) float                 m_zoneRadius{3.f};
        RFKField(Inspect(), Serialize()) float                 m_spawnDelay{1.f};         /*in sec*/
        RFKField(Inspect(), Serialize()) float                 m_spawnDelayInterval{0.f}; /*in sec*/
        RFKField(Inspect(), Serialize()) float                 m_delayCount{0.f};
        RFKField(Inspect(), Serialize()) float                 m_nextDelay{m_spawnDelay};
        RFKField(Inspect(), Serialize()) GPE::GameObjectLinker m_enemiesContainer;
        RFKField() GPE::GameObject*                            m_player;

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
        SpawnManager(GPE::GameObject & gameObject);
        SpawnManager()          = default;
        virtual ~SpawnManager() = default;

        void start() final;
        void onPostLoad() final;

        void update(double deltaTime) final;

        SpawnManager_GENERATED
    };

} // namespace )