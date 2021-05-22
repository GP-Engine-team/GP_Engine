/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Resources/Prefab.hpp>

// Generated
#include <Generated/CircularEntitiesSpawner.rfk.h>

namespace GPE
{
class GameObject;
}

namespace GPG RFKNamespace()
{
    struct RFKStruct(Inspect(), Serialize()) EntityPrefabCount
    {
        RFKField(Inspect(), Serialize()) unsigned int numberEntity;
        RFKField(Inspect(), Serialize()) GPE::Prefab* prefab = nullptr;

        EntityPrefabCount_GENERATED
    };

    class RFKClass(Inspect(), ComponentGen, Serialize()) CircularEntitiesSpawner : public GPE::BehaviourComponent
    {
    private:
        RFKField(Inspect(), Serialize()) std::vector<EntityPrefabCount> m_entitiesToSpawnInfo{};
        RFKField(Inspect(), Serialize()) float                          m_zoneRadius{3.f};         /*in sec*/
        RFKField(Inspect(), Serialize()) float                          m_spawnDelay{1.f};         /*in sec*/
        RFKField(Inspect(), Serialize()) float                          m_spawnDelayInterval{0.f}; /*in sec*/
        RFKField(Inspect(), Serialize()) float                          m_delayCount{0.f};
        RFKField(Inspect(), Serialize()) float                          m_nextDelay{m_spawnDelay};
        RFKField(Inspect(), Serialize()) GPE::GameObject*               m_container{nullptr};
        // Game::Checkpoint*              m_checkpoint{nullptr};

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
        CircularEntitiesSpawner(GPE::GameObject & gameObject);
        CircularEntitiesSpawner()          = default;
        virtual ~CircularEntitiesSpawner() = default;

        void start() final;
        void onPostLoad() final;

        /**
         * @brief Return true if the spawn stop to spawn entities
         *
         * @return true
         * @return false
         */
        inline bool isEmpty() const noexcept
        {
            return m_entitiesToSpawnInfo.empty();
        }

        void update(double deltaTime) final;

        CircularEntitiesSpawner_GENERATED
    };

} // namespace )