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

#define EARTH_GRAVITY 9.80665f

namespace physx
{
class PxController;
}

namespace GPE RFKNamespace()
{
    class RFKClass(ComponentGen(), Inspect(), Serialize()) CharacterController : public Component
    {
    public:
        CharacterController(GameObject & owner) noexcept;

        CharacterController() noexcept                                 = default;
        CharacterController(const CharacterController& other) noexcept = delete;
        CharacterController(CharacterController && other) noexcept     = default;
        CharacterController& operator=(CharacterController const& other) noexcept = delete;
        CharacterController& operator=(CharacterController&& other) noexcept = delete;

        void update(double deltaTime) noexcept;

        virtual ~CharacterController() noexcept;

    private:
        RFKField(Inspect(), Serialize()) float                   m_gravity       = EARTH_GRAVITY;
        RFKField(Inspect(), Serialize()) float                   m_mass          = 90.f;
        RFKField(Inspect(), Serialize()) float                   m_speed         = 1.f;
        RFKField(Inspect(), Serialize()) float                   m_angularSpeed  = 1.f; // in rad by second
        RFKField(Inspect()) float                                m_startJumpTime = 0.f;
        RFKField(Inspect(), Serialize()) float                   m_jumpTimeDelay = 1.f;
        RFKField(Inspect(), Serialize()) bool                    m_hasGravity    = true;
        RFKField(Inspect(), Serialize()) bool                    m_canJump       = false;
        RFKField(Inspect("setHeight"), Serialize()) float        m_height        = 25.f;
        RFKField(Inspect("setRadius"), Serialize()) float        m_radius        = 10.f;
        RFKField(Inspect("setContactOffset"), Serialize()) float m_contactOffset = 0.1f;
        RFKField(Inspect("setCenter"), Serialize()) GPM::Vec3    m_center        = {.0f};
        RFKField(Serialize()) GPM::Vec3                          m_parentScale   = {.0f};

        GPM::Vec3 m_force        = {.0f};
        bool      m_grounded     = false;
        GPM::Vec3 m_displacement = {.0f};

    protected:
        virtual void updateToSystem() noexcept override;
        virtual void onPostLoad() noexcept override;

    public:
        physx::PxController* controller = nullptr;
        DEFAULT_GETTER_SETTER_BY_VALUE(Speed, m_speed);
        DEFAULT_GETTER_SETTER_BY_VALUE(AngularSpeed, m_angularSpeed);
        DEFAULT_GETTER_SETTER_BY_VALUE(HasGravity, m_hasGravity);
        DEFAULT_GETTER_SETTER_BY_VALUE(Gravity, m_gravity);
        DEFAULT_GETTER_SETTER_BY_VALUE(Mass, m_mass);
        GETTER_BY_VALUE(CanJump, m_canJump);

        /**
         * @brief Start the jump timer and disable the ability to jump temporary;
         * @return
         */
        void startJumpTimer() noexcept;

        /**
         * @brief move the character controller of "displacement";
         * @param displacement
         * @return
         */
        void move(const GPM::Vec3& displacement) noexcept;

        /**
         * @brief move the character controller of "displacement" but with a custom speed;
         * @param displacement
         * @param customSpeed
         * @return
         */
        void move(const GPM::Vec3& displacement, float customSpeed) noexcept;

        /**
         * @brief Add a force to the character controller;
         * @param displacement
         * @return
         */
        void addForce(const GPM::Vec3& displacement) noexcept;

        /**
         * @brief Apply the current force on the displacement vector;
         * @return
         */
        void updateForce() noexcept;

        /**
         * @brief Check if the character controller touch the ground;
         * @return
         */
        void groundCheck() noexcept;

        /**
         * @brief update the shape of the CharacterController;
         * @return
         */
        void updateShape() noexcept;

        /**
         * @brief set the new local center of the collider;
         * @param newCenter
         * @return
         */
        void setCenter(const GPM::Vec3& newCenter) noexcept;

        /**
         * @brief set the new height of the collider;
         * @param newHeight
         * @return
         */
        void setHeight(float newHeight) noexcept;

        /**
         * @brief set the new radius of the collider;
         * @param newRadius
         * @return
         */
        void setRadius(float newRadius) noexcept;

        /**
         * @brief set the new contact offset of the collider;
         * @param newOffset
         * @return
         */
        void setContactOffset(float newOffset) noexcept;

        /**
         * @brief Update the scale of the collider from is owner;
         * @return
         */
        void updateScale() noexcept;

        void updatePosition();

        RFKMethod() void updateTransform();

        CharacterController_GENERATED
    };
} // namespace )
