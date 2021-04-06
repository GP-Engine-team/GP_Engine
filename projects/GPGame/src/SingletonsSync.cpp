#include "SingletonsSync.hpp"

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Serialization/DataInspector.hpp"
#include "Engine/Serialization/InspectContext.hpp"
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

extern "C" GAME_API void saveScene(XmlSaver& context, GPE::GameObject* const& inspected, const XmlSaver::SaveInfo& info)
{
    GPE::save(context, inspected, info);
}

extern "C" GAME_API void loadScene(XmlLoader& context, GPE::GameObject*& inspected, const XmlLoader::LoadInfo& info)
{
    GPE::load(context, inspected, info);
}