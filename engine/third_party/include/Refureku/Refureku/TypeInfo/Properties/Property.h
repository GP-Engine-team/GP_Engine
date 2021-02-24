/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Refureku/Object.h"
#include "Refureku/TypeInfo/Entity/EEntityKind.h"

namespace rfk
{
	/**
	*	@brief Base struct to inherit from to define new properties.
	*/
	struct Property : public Object
	{
		protected:
			Property()	= default;

		public:
			/** Kind of entity this property can be attached to. By default, a property can be attached to any kind of entity. */
			static constexpr rfk::EEntityKind	targetEntityKind	=	rfk::EEntityKind::Class | rfk::EEntityKind::Enum | rfk::EEntityKind::EnumValue |
																		rfk::EEntityKind::Field |rfk::EEntityKind::Function | rfk::EEntityKind::Method |
																		rfk::EEntityKind::Namespace | rfk::EEntityKind::Struct | rfk::EEntityKind::Variable;

			/** Should this property be inherited in child struct/class? Relevant only for properties targeting structs/classes. */
			static constexpr bool				shouldInherit		= true;
			
			/** Is this property allowed to be attached multiple times to the same entity? */
			static constexpr bool				allowMultiple		= false;

			/**
			*	@brief Getter for targetEntityKind of the concrete Property type.
			*	
			*	@return targetEntityKind.
			*/
			virtual rfk::EEntityKind	getTargetEntityKind()	const noexcept;

			/**
			*	@brief Getter for shouldInherit of the concrete Property type.
			*	
			*	@return shouldInherit.
			*/
			virtual bool				getShouldInherit()		const noexcept;

			/**
			*	@brief Getter for allowMultiple of the concrete Property type.
			*	
			*	@return allowMultiple.
			*/
			virtual bool				getAllowMultiple()		const noexcept;

			Property(Property const&)	= delete;
			Property(Property&&)		= delete;
	};
}