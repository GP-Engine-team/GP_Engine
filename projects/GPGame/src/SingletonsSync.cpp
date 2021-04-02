#include "SingletonsSync.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <memory>

extern "C" void setLogInstance(GPE::Log& log)
{
    GPE::Log::setInstance(log);
}

extern "C" GPE::Log& getLogInstance()
{
    return *GPE::Log::getInstance();
}

extern "C" void setGameEngineInstance(GPE::Engine& engine)
{
    GPE::Engine::setInstance(engine);
}

extern "C" GPE::Engine& getGameEngineInstance()
{
    return *GPE::Engine::getInstance();
}

extern "C" void setContextCurrent(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);
}

extern "C" void setImguiCurrentContext(ImGuiContext* ctx)
{
    ImGui::SetCurrentContext(ctx);
}
