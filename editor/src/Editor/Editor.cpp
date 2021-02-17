#include "Editor/Editor.hpp"

#include "Engine/Core/Debug/Log.hpp"
#include "imgui/imgui.h"

#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

using namespace Engine::Core::Debug;

// Hint to use GPU if available
extern "C"
{
    _declspec(dllexport) int NvOptimusEnablement = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


namespace Editor
{

/* ========================== Constructor & destructor ========================== */
Editor::Editor()
    : window{{"GP engine editor", 100, 100}}, renderer{window}
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}


Editor::~Editor()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void Editor::update(double unscaledDeltaTime, double deltaTime)
{
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hey");
        ImGui::Text("Hello there");
    ImGui::End();

    render();
    renderer.swapBuffer();
}


void Editor::fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime)
{

}


void Editor::render()
{
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window.getGLFWWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


bool Editor::isRunning()
{
    return !glfwWindowShouldClose(window.getGLFWWindow());
}

} // End of namespace Editor