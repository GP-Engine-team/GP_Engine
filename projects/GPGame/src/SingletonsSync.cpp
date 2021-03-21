#include "SingletonsSync.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

extern "C" void setGameSystemsManagerInstance(GPE::SystemsManager & systemManager)
{
	GPE::SystemsManager::setInstance(systemManager);
}

extern "C" GPE::SystemsManager & getGameSystemsManagerInstance()
{
	return *GPE::SystemsManager::getInstance();
}

extern "C" void setContextCurrent(GLFWwindow * window)
{
	glfwMakeContextCurrent(window);
}

extern "C" void setImguiCurrentContext(ImGuiContext* ctx)
{
	ImGui::SetCurrentContext(ctx);
}
