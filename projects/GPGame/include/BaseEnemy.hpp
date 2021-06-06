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
        enum class EState
        {
            RUNNING,
            DEAD,
            ATTACKING,
            DAMAGED
        };

    protected:
        RFKField() BaseCharacter*              m_target              = nullptr;
        RFKField(Serialize()) float            m_animDeathCounter    = 0.f;
        RFKField(Inspect(), Serialize()) float m_animDeathCounterMax = 3.f;

        RFKField(Serialize(), Inspect(), ReadOnly(), Separator(true, false)) float m_attackCounter    = 0.f;
        RFKField(Inspect(), Serialize()) float                                     m_attackCounterMax = 3.f;
        RFKField(Inspect(), Serialize()) float                                     m_radiusAttack     = 3.f;
        RFKField(Inspect(), Serialize()) float                                     m_dammage          = 3.f;
        RFKField(Serialize()) GPE::AudioComponent*                                 m_source           = nullptr;
        RFKField() std::vector<GPE::AnimationComponent*>                           m_animComps;
        RFKField() GPE::CharacterController*                                       m_controller       = nullptr;

        RFKField(Inspect(), Serialize(), Separator()) GPE::Animation* m_walkAnimation   = nullptr;
        RFKField(Inspect(), Serialize()) GPE::Animation* m_deathAnimation  = nullptr;
        RFKField(Inspect(), Serialize()) GPE::Animation* m_attackAnimation = nullptr;
        RFKField(Inspect(), Serialize()) GPE::Animation* m_onHitAnimation  = nullptr;

        RFKField(Inspect(), Serialize(), Separator()) float m_disappearanceSpeed       = 10.f;
        RFKField(Inspect(), Serialize())              float m_maxHeightBeforDestroying = 50.f;

        EState m_currentState = EState::RUNNING;
        RFKField(Inspect(), ReadOnly())  float m_nextAnimTime = -1.f;
        RFKField(Inspect(), ReadOnly())  float m_nextHitDelay = std::numeric_limits<float>::max();
        RFKField(Inspect(), Serialize()) float m_hitDelayRelativeToAnimLength = 0.36;

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
