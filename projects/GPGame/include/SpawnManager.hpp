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

        struct RFKStruct(Inspect(), Serialize()) DifficultyLevel
        {
            DifficultyLevel() = default;
            DifficultyLevel(float f, unsigned int mE) : m_spawnfrequency{f}, m_maxEntity{mE}
            {
            }

            RFKField(Inspect(), Serialize()) float        m_spawnfrequency = 0.f;
            RFKField(Inspect(), Serialize()) unsigned int m_maxEntity      = 0u;

            DifficultyLevel_GENERATED
        };

    private:
        RFKField(Inspect(), Serialize()) std::vector<Spawner>         m_spawners{};
        RFKField(Inspect(), Serialize()) std::vector<EntityPrefab>    m_entitiesToSpawnInfo{};
        RFKField(Inspect(), Serialize()) std::vector<DifficultyLevel> m_difficultyInfo{};

        RFKField(Inspect(), Serialize()) unsigned int m_currentDifficulty = 0;
        RFKField(Inspect(), Serialize()) float        m_spawnerZoneRadius{100.f};
        RFKField(Inspect(), Serialize()) float        m_playerZoneRadius{250.f};
        float                                         sqrTotalRadius = 0;
        // RFKField(Inspect(), Serialize()) float                 m_spawnDelay{1.f};         /*in sec*/
        // RFKField(Inspect(), Serialize()) float                 m_spawnDelayInterval{0.f}; /*in sec*/
        RFKField(Inspect(), Serialize()) float                 m_delayCount{0.f};
        RFKField(Inspect(), Serialize()) float                 m_nextDelay{0.f};
        RFKField(Inspect(), Serialize()) GPE::GameObjectLinker m_enemiesContainer;
        RFKField() GPE::GameObject*                            m_player;

        RFKField(Inspect()) bool m_debug = true;

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
        void updateEditor(double deltaTime) final;

        void setDifficulty(unsigned int level);
        void autoGenerateLinearDifficulty(unsigned int count, const DifficultyLevel& min, const DifficultyLevel& max);

        SpawnManager_GENERATED
    };

} // namespace )