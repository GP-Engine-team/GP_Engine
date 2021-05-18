/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Physics/Collisions/Collider.hpp>
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <GPM/Vector3.hpp>

 // Generated
#include <Generated/BoxCollider.rfk.h>

namespace GPE RFKNamespace()
{
	class RFKClass(Inspect()) BoxCollider : public Collider
	{
	public:
		BoxCollider() noexcept;
		BoxCollider(const BoxCollider & other) noexcept = delete;
		BoxCollider(BoxCollider && other) noexcept = default;
		virtual ~BoxCollider() noexcept = default;

		BoxCollider& operator=(BoxCollider const& other) noexcept = delete;
		BoxCollider& operator=(BoxCollider && other) noexcept = delete;

	private:
		GPM::Vector3 m_dimensions;

	public:
		GETTER_BY_VALUE(Dimensions, m_dimensions);
		void setDimensions(const GPM::Vec3 & newDimensions) noexcept;

		BoxCollider_GENERATED
	};
} // namespace )
