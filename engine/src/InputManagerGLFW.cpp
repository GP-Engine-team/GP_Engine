#include "Engine/Core/Input/InputManagerGLFW.hpp"
#include "GLFW/glfw3.h"

using namespace std;
using namespace Engine::Core::Input;
using namespace Engine::Core;

InputManager* InputManager::GetInstance()
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (m_inputManager == nullptr)
	{
		m_inputManager = new InputManager();
	}
	return m_inputManager;
}

void InputManager::fireInputComponents(const std::string& action)
{
	if (!action.empty())
	{
		for (size_t i = 0; i < inputComponents.size(); i++)
		{
			inputComponents[i].fireAction(action);
		}
	}
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto it = actionMap.equal_range(key);

	for(auto i = it.first; i != it.second; i++)
	{
		fireInputComponents(i->second);
	}
}

void Input::setKeycallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputManager::GetInstance()->keyCallback(window,key,scancode,action,mods);
}

void InputManager::setupCallbacks(GLFWwindow* window)
{
	glfwSetKeyCallback(window, setKeycallback);
}