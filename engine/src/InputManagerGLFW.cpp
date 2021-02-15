#include "Engine/Core/Input/InputManagerGLFW.hpp"

using namespace std;
using namespace Engine::Core::Input;
using namespace Engine::Core;

InputManager* InputManager::m_inputManager = nullptr;

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
	InputManager* input = InputManager::GetInstance();
	if (!action.empty())
	{
		for (size_t i = 0; i < input->inputComponents.size(); i++)
		{
			input->inputComponents[i].fireAction(action);
		}
	}
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputManager* input = InputManager::GetInstance();
	auto it = input->actionMap.equal_range(key);

	for(auto i = it.first; i != it.second; i++)
	{
		input->fireInputComponents(i->second);
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