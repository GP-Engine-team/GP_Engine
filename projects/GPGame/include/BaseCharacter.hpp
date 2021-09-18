/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>
#include <gpm/Vector3.hpp>

// Generated
#include <Generated/BaseCharacter.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) BaseCharacter : public GPE::BehaviourComponent
    {
    protected:
        RFKField(Serialize()) GPE::CharacterController* controller           = nullptr;
        RFKField(Inspect(), Serialize()) float          m_maxLife            = 0.f;
        RFKField(Inspect(), Serialize()) float          m_currentLife        = 0.f;
        RFKField(Inspect(), Serialize()) float          m_jumpStrength       = 1000.f;
        RFKField(Inspect(), Serialize()) float          m_baseSpeed          = 2.f;
        RFKField(Inspect(), Serialize()) float          m_sprintAcceleration = 2.f;
        RFKField(Inspect(), Serialize()) bool           m_isDead             = false;

        RFKField(Inspect(), Serialize(), ReadOnly()) float m_bodyBalancing      = 0.f; //[-1, 1]
        RFKField(Inspect(), Serialize()) float             m_bodyBalancingSpeed = 1.f;

        RFKField(Serialize()) bool m_isSprint = false;

    public:
        BaseCharacter(GPE::GameObject & owner);
        BaseCharacter() noexcept = default;
        virtual ~BaseCharacter() = default;

        void start() override;
        void fixedUpdate(double deltaTime) override;
        void onPostLoad() override;

        RFKMethod() void rotateYToward(const GPM::Vec3& target, float deltaTime);

        /**
         * @brief This funciton include only translation. Displayement is erase in Y to avoid fly
         * @param target
         * @param deltaTime
         * @return
         */
        RFKMethod() void moveToward(const GPM::Vec3& target);

        /**
         * @brief Rotate towar entity and move in its forward direction
         * @param target
         * @param deltaTime
         * @return
         */
        RFKMethod() void moveAndRotateToward(const GPM::Vec3& target, float deltaTime);

        RFKMethod() void jump();
        RFKMethod() void forward();
        RFKMethod() void backward();
        RFKMethod() void left();
        RFKMethod() void right();
        RFKMethod() void sprintStart();
        RFKMethod() void sprintEnd();

        RFKMethod() virtual void takeDamage(float damage);
        RFKMethod() bool isDead();
        RFKMethod() void takeLife(float damage);
        DEFAULT_GETTER_SETTER_BY_VALUE(BodyBalancing, m_bodyBalancing)
        DEFAULT_GETTER_SETTER_BY_VALUE(IsSprint, m_isSprint)

        RFKMethod() virtual void onDeath();

        BaseCharacter_GENERATED
    };

} // namespace )

File_GENERATED