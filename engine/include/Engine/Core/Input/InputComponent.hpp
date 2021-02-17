/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include "Engine/Intermediate/Component.hpp"

namespace GPE
{
	class InputComponent : public Component
	{
	public:
		//InputComponent() = delete;
		~InputComponent() = default;
		InputComponent(GameObject& owner);

	private:
		std::unordered_map<std::string, void*> m_functionMap;
		int									   m_key;

	public:

		/**
		 * @brief Bind a function to an action
		 * @param action
		 * @param function
		*/
		void bindAction(const std::string& action, void* function);

		/**
		 * @brief launch an action
		 * @param action
		*/
		void fireAction(const std::string& action);

	};
} // namespace Engine::Core::Input