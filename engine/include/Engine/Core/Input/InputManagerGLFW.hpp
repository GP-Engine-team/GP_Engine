/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include "GLFW/glfw3.h"
#include "Engine/Core/Input/InputComponent.hpp"

namespace Engine::Core::Input
{
	class InputManager
	{
	protected:
		InputManager(){};

		static InputManager* m_inputManager;
	public:

		/**
		* Singletons should not be cloneable.
		*/
		InputManager(InputManager& other) = delete;

		/**
		 * Singletons should not be assignable.
		 */
		void operator=(const InputManager&) = delete;

	private:
		std::unordered_multimap<int, std::string>	actionMap;
		std::vector<InputComponent>					inputComponents;

	public:
		static InputManager* GetInstance();

		_NODISCARD bool checkForAction(const std::string& action) const;
		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) const;
		void setupCallbacks(GLFWwindow* window);
		void fireInputComponents(const std::string& action) const;
		void bindInput(const int& key, const std::string& action);
	};
	void setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods);
} // namespace Engine::Core::Input