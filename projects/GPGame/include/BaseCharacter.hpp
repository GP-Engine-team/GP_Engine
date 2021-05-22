/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

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
        RFKField(Serialize()) GPE::CharacterController* controller = nullptr;

    public:
        BaseCharacter(GPE::GameObject & owner);
        BaseCharacter() noexcept = default;
        virtual ~BaseCharacter() = default;

        void start() override;
        void fixedUpdate(double deltaTime) override;
        void onPostLoad() override;

        RFKMethod() void rotateYToward(const GPM::Vec3& target, float deltaTime);
        RFKMethod() void jump();
        RFKMethod() void forward();
        RFKMethod() void backward();
        RFKMethod() void left();
        RFKMethod() void right();
        RFKMethod() void sprintStart();
        RFKMethod() void sprintEnd();

        BaseCharacter_GENERATED
    };

} // namespace )