/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <type_traits>

#include "Refureku/TypeInfo/Archetypes/Enum.h"
#include "Refureku/TypeInfo/Archetypes/ArchetypeRegisterer.h"
#include "Refureku/Utility/TypeTraits.h"

namespace rfk
{
	/**
	*	@brief	Get the archetype of any type if it exists.
	*			If a cv-qualified, pointer/reference, array type is passed, they are all ignored and the raw type archetype is returned.
	*			Example:	int* -> int,
	*						int[2] -> int,
	*						const volatile int -> int,
	*						int& -> int
	*
	*			This templated method can be specialized to handle unreflected types.
	*
	*	@return The archetype of the provided type if it exists, else nullptr.
	*/
	template <typename T>
	Archetype const* getArchetype() noexcept;

	/**
	*	rfk::getArchetype specialization for all fundamental types.
	*/
	template <>
	rfk::Archetype const* getArchetype<void>()					noexcept;

	template <>
	rfk::Archetype const* getArchetype<std::nullptr_t>()		noexcept;

	template <>
	rfk::Archetype const* getArchetype<bool>()					noexcept;

	template <>
	rfk::Archetype const* getArchetype<char>()					noexcept;

	template <>
	rfk::Archetype const* getArchetype<signed char>()			noexcept;

	template <>
	rfk::Archetype const* getArchetype<unsigned char>()			noexcept;

	template <>
	rfk::Archetype const* getArchetype<wchar_t>()				noexcept;

	template <>
	rfk::Archetype const* getArchetype<char16_t>()				noexcept;

	template <>
	rfk::Archetype const* getArchetype<char32_t>()				noexcept;

	template <>
	rfk::Archetype const* getArchetype<short>()					noexcept;

	template <>
	rfk::Archetype const* getArchetype<unsigned short>()		noexcept;

	template <>
	rfk::Archetype const* getArchetype<int>()					noexcept;

	template <>
	rfk::Archetype const* getArchetype<unsigned int>()			noexcept;

	template <>
	rfk::Archetype const* getArchetype<long>()					noexcept;

	template <>
	rfk::Archetype const* getArchetype<unsigned long>()			noexcept;

	template <>
	rfk::Archetype const* getArchetype<long long>()				noexcept;

	template <>
	rfk::Archetype const* getArchetype<unsigned long long>()	noexcept;

	template <>
	rfk::Archetype const* getArchetype<float>()					noexcept;

	template <>
	rfk::Archetype const* getArchetype<double>()				noexcept;

	template <>
	rfk::Archetype const* getArchetype<long double>()			noexcept;

	//Register all of these archetypes to database
	namespace internal
	{
		inline ArchetypeRegisterer voidRegisterer		= getArchetype<void>();
		inline ArchetypeRegisterer nullptrRegisterer	= getArchetype<std::nullptr_t>();
		inline ArchetypeRegisterer boolRegisterer		= getArchetype<bool>();
		inline ArchetypeRegisterer charRegisterer		= getArchetype<char>();
		inline ArchetypeRegisterer signedCharRegisterer	= getArchetype<signed char>();
		inline ArchetypeRegisterer ucharRegisterer		= getArchetype<unsigned char>();
		inline ArchetypeRegisterer wcharRegisterer		= getArchetype<wchar_t>();
		inline ArchetypeRegisterer char16Registerer		= getArchetype<char16_t>();
		inline ArchetypeRegisterer char32Registerer		= getArchetype<char32_t>();
		inline ArchetypeRegisterer shortRegisterer		= getArchetype<short>();
		inline ArchetypeRegisterer ushortRegisterer		= getArchetype<unsigned short>();
		inline ArchetypeRegisterer intRegisterer		= getArchetype<int>();
		inline ArchetypeRegisterer uintRegisterer		= getArchetype<unsigned int>();
		inline ArchetypeRegisterer longRegisterer		= getArchetype<long>();
		inline ArchetypeRegisterer ulongRegisterer		= getArchetype<unsigned long>();
		inline ArchetypeRegisterer longLongRegisterer	= getArchetype<long long>();
		inline ArchetypeRegisterer ulongLongRegisterer	= getArchetype<unsigned long long>();
		inline ArchetypeRegisterer floatRegisterer		= getArchetype<float>();
		inline ArchetypeRegisterer doubleRegisterer		= getArchetype<double>();
		inline ArchetypeRegisterer longDoubleRegisterer	= getArchetype<long double>();
	}

	#include "Refureku/TypeInfo/Archetypes/GetArchetype.inl"
}