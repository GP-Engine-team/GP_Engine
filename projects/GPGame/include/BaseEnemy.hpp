/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

//#include <Engine/ECS/Component/AudioComponent.hpp>

#include <BaseCharacter.hpp>

#include <Generated/BaseEnemy.rfk.h>

namespace GPE
{
class GameObject;
class AudioComponent;
} // namespace GPE

namespace GPG RFKNamespace()
{
    class Firearm;

    class RFKClass(Inspect(), ComponentGen, Serialize()) BaseEnemy : public BaseCharacter
    {
    protected:
        RFKField() BaseCharacter*              m_target              = nullptr;
        RFKField(Serialize()) float            m_animDeathCounter    = 0.f;
        RFKField(Inspect(), Serialize()) float m_animDeathCounterMax = 3.f;

        RFKField(Serialize(), Inspect(), ReadOnly(), Separator(true, false)) float m_attackCounter    = 0.f;
        RFKField(Inspect(), Serialize()) float                                     m_attackCounterMax = 3.f;
        RFKField(Inspect(), Serialize()) float                                     m_radiusAttack     = 3.f;
        RFKField(Inspect(), Serialize(), Separator(false, true)) float             m_dammage          = 3.f;
        RFKField(Serialize()) GPE::AudioComponent*                                 m_source           = nullptr;
        RFKField() GPE::AnimationComponent*                                        m_animComp         = nullptr;
        RFKField() GPE::CharacterController*                                       m_controller       = nullptr;

        RFKField(Inspect(), Serialize()) float m_disappearanceSpeed       = 10.f;
        RFKField(Inspect(), Serialize()) float m_maxHeightBeforDestroying = 50.f;

    public:
        BaseEnemy() noexcept = default;
        BaseEnemy(GPE::GameObject & owner) noexcept;
        BaseEnemy(const BaseEnemy& other) noexcept = delete;
        BaseEnemy(BaseEnemy && other) noexcept     = delete;
        virtual ~BaseEnemy() noexcept              = default;

        BaseEnemy& operator=(BaseEnemy const& other) noexcept = delete;
        BaseEnemy& operator=(BaseEnemy&& other) noexcept = delete;

    public:
        void start() final;
        void update(double deltaTime) final;
        void onPostLoad() final;
        void onDeath() final;

        BaseEnemy_GENERATED
    };

} // namespace )
