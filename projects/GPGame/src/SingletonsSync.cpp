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
#include <Windows.h>
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

extern "C" void setImguiCurrentContext(ImGuiContext* ctx)
{
    ImGui::SetCurrentContext(ctx);
}

extern "C" ImGuiContext* getGameUIContext()
{
    return ImGui::GetCurrentContext();
}

extern "C" void saveScene(XmlSaver& context, GPE::Scene* scene)
{
    //GPE::Engine::getInstance()->sceneManager.getCurrentScene()->save(context);
    context.addWeakPtr(scene);
    scene->save(context);
}

extern "C" void loadScene(XmlLoader& context, GPE::Scene* scene)
{
    context.addPersistentPtr(scene);

    //GPE::Engine::getInstance()->sceneManager.getCurrentScene()->load(context);
    scene->load(context);
    context.updateLazyPtrs();


}