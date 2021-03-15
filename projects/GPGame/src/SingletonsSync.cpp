#include "SingletonsSync.hpp"

#include "GLFW/glfw3.h"

extern "C" void setGameSystemsManagerInstance(GPE::SystemsManager& systemManager)
{
	GPE::SystemsManager::setInstance(systemManager);
}

extern "C" GPE::SystemsManager & getGameSystemsManagerInstance()
{
	return *GPE::SystemsManager::getInstance();
}

extern "C" void setContextCurrent(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);
}
