/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/ECS/Component/Component.hpp>
#include <GPM/Vector3.hpp>

#include <Generated/CharacterController.rfk.h>

#define EARTH_GRAVITY 0.980665f

namespace physx
{
class PxController;
}

namespace GPE RFKNamespace()
{
    class RFKClass(ComponentGen(), Inspect(), Serialize()) CharacterController : public Component
    {
    public:
        CharacterController(GameObject& owner) noexcept;

        CharacterController() noexcept;
        CharacterController(const CharacterController& other) noexcept = delete;
        CharacterController(CharacterController&& other) noexcept     = default;
        CharacterController& operator=(CharacterController const& other) noexcept = delete;
        CharacterController& operator=(CharacterController&& other) noexcept = delete;

        void update(double deltaTime) noexcept;

        virtual ~CharacterController() noexcept;

    private:
        RFKField(Inspect(), Serialize()) GPM::Vec3 m_displacement  = {.0f};
        RFKField(Inspect(), Serialize()) GPM::Vec3 m_force         = {.0f};
        RFKField(Inspect(), Serialize()) float     m_gravity       = EARTH_GRAVITY;
        RFKField(Inspect(), Serialize()) float     m_speed         = 1.f;
        RFKField(Inspect(), Serialize()) float     m_mouseSpeed    = 1.f;
        RFKField(Inspect(), Serialize()) float     m_startJumpTime = 0.f;
        RFKField(Inspect(), Serialize()) float     m_jumpTimeDelay = 1.f;
        RFKField(Inspect(), Serialize()) bool      m_hasGravity    = false;
        RFKField(Inspect(), Serialize()) bool      m_jumping       = false;

    protected:
        virtual void updateToSystem() noexcept override;

    public:
        physx::PxController* controller = nullptr;
        DEFAULT_GETTER_SETTER_BY_VALUE(Speed, m_speed);
        DEFAULT_GETTER_SETTER_BY_VALUE(MouseSpeed, m_mouseSpeed);
        DEFAULT_GETTER_SETTER_BY_VALUE(HasGravity, m_hasGravity);
        DEFAULT_GETTER_SETTER_BY_VALUE(Gravity, m_gravity);
        GETTER_BY_VALUE(Jumping, m_jumping);

        void setJumping(float jumping) noexcept;
        void move(const GPM::Vec3& displacement) noexcept;
        void move(const GPM::Vec3& displacement, float customSpeed) noexcept;
        void addForce(const GPM::Vec3& displacement) noexcept;
        void updateForce() noexcept;

        bool canJump() noexcept;

        CharacterController_GENERATED
    };
} // namespace GPE
