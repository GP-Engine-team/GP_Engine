/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>

namespace rfk
{
	class NativeProperties
	{
		public:
			/**
			*	Method property used to register a method as a class instantiator (used by makeInstance()).
			*	The method must have the prototype static void* (*)(AnyParameterYouWant);
			*/
			inline static std::string const customInstantiatorProperty		= "CustomInstantiator";

			/**
			*	Struct/Class property used to define settings of a property.
			*	The struct or class must be a child class of rfk::Property.
			*/
			inline static std::string const	propertySettingsProperty		= "PropertySettings";

			NativeProperties()							= delete;
			NativeProperties(NativeProperties const&)	= delete;
			NativeProperties(NativeProperties&&)		= delete;
			~NativeProperties()							= delete;
	};
}