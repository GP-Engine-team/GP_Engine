/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <characterkinematic/PxCapsuleController.h>
#include <GPM/Vector3.hpp>
#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/Serialization/Slider.hpp>

// Generated
#include "Generated/CharacterController.rfk.h"

#define EARTH_GRAVITY 0.980665f 

namespace GPE RFKNamespace()
{
    class RFKClass(Inspect(), Serialize()) CharacterController : public Component
	{
	public:
		CharacterController(GameObject& owner) noexcept;

		CharacterController() noexcept;
		CharacterController(const CharacterController& other) noexcept = delete;
		CharacterController(CharacterController&& other) noexcept = default;
		CharacterController& operator=(CharacterController const& other) noexcept = delete;
		CharacterController& operator=(CharacterController&& other) noexcept = delete;

		void update(float deltaTime) noexcept;

		virtual ~CharacterController() noexcept;

	private:
		GPM::Vec3				m_displacement	= { 0,0,0 };
		float					m_gravity		= EARTH_GRAVITY;
        RFKField(Serialize(), Inspect()) 
		bool					m_hasGravity	= false;
        RFKField(Serialize(), Inspect(), Slider(0, 10)) 
		float					m_speed			= 1.f;
		//GPM::Vec3				m_velocity		= { 0,0,0 };
		//GPM::Vec3				m_acceleration	= { 0,0,0 };

	public:
		physx::PxController* controller;
		DEFAULT_GETTER_SETTER_BY_VALUE(Speed,m_speed);
		DEFAULT_GETTER_SETTER_BY_VALUE(HasGravity, m_hasGravity);
		DEFAULT_GETTER_SETTER_BY_VALUE(Gravity, m_gravity);
		void move(const GPM::Vec3& displacement) noexcept;
		void move(const GPM::Vec3& displacement, float customSpeed) noexcept;

		CharacterController_GENERATED
	};
}