#include "SingletonsSync.hpp"

#include "GLFW/glfw3.h"

extern "C" void setGameEngineInstance(GPE::Engine & engine)
{
	GPE::Engine::setInstance(engine);
}

extern "C" GPE::Engine & getGameEngineInstance()
{
	return *GPE::Engine::getInstance();
}

extern "C" void setContextCurrent(GLFWwindow * window)
{
	glfwMakeContextCurrent(window);
}
