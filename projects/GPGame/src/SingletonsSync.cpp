#include "SingletonsSync.hpp"

#include "GLFW/glfw3.h"

extern "C" void setLogInstance(GPE::Log & log)
{
	GPE::Log::setInstance(log);
}

extern "C" GPE::Log & getLogInstance()
{
	return *GPE::Log::getInstance();
}

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
