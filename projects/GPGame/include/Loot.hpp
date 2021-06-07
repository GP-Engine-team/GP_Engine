/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <BasePlayer.hpp>
#include <SpawnManager.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Resources/Prefab.hpp>

// Generated
#include <Generated/Loot.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) Loot : public GPE::BehaviourComponent
    {
    private:
        RFKField() BasePlayer* m_player       = nullptr;
        RFKField() SpawnManager* m_spawnerManager = nullptr; 
        float                  sqrTotalRadius = 0;

        RFKField(Inspect(), Serialize()) float              m_radius = 0.5f;
        RFKField(Inspect(), Serialize(), Separator()) float m_speed{1.f};
        RFKField(Inspect(), Serialize()) float              m_heightIntensity{1.f};
        RFKField(Inspect(), Serialize()) float              m_delay{0.f};
        RFKField(Inspect(), Serialize()) float              m_initialY{0.f};

    public:
        Loot(GPE::GameObject & owner);
        Loot() noexcept = default;
        virtual ~Loot() = default;

        void start() override;
        void update(double deltaTime) override;
        void onPostLoad() override;

        Loot_GENERATED
    };

} // namespace )