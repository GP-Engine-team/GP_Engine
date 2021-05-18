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
	class RFKClass(Inspect(), Serialize()) BoxCollider : public Collider
	{
	public:
		BoxCollider() noexcept;
		BoxCollider(const BoxCollider & other) noexcept = delete;
		BoxCollider(BoxCollider && other) noexcept = default;
		virtual ~BoxCollider() noexcept = default;

		BoxCollider& operator=(BoxCollider const& other) noexcept = delete;
		BoxCollider& operator=(BoxCollider && other) noexcept = delete;

	private:
		GPM::Vector3 m_scale;
		GPM::Vector3 m_offset;

	public:
		GETTER_BY_VALUE(Scale, m_scale);
		RFKMethod()void setScale() noexcept;
		GETTER_BY_VALUE(Offset, m_offset);
		void setOffset(const GPM::Vec3 & newOffset) noexcept;
		void onPostLoad() noexcept;
		void updateShape() noexcept;

		BoxCollider_GENERATED
	};
} // namespace )
