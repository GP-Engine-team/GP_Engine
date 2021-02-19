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

/* ========================== Friend function ========================== */
void windowFramebufferResized(GLFWwindow* window, int newWidth, int newHeight)
{
    Editor* editor{(Editor*)glfwGetWindowUserPointer(window)};

    editor->m_framebufferWidth = newWidth;
    editor->m_framebufferHeight = newHeight;
}




/* ========================== Private methods ========================== */
void Editor::initGLFW()
{
    // Initialization
    if (!glfwInit())
    {
        fprintf(stderr, "GLFW failed to initialize\n");
        exit(EXIT_FAILURE);
    }

    // Error callback
#ifdef _DEBUG
    glfwSetErrorCallback([](int error, const char* desc) {
        fprintf(stderr, "GLFW error %d: %s\n", error, desc);
    });
#endif

    // OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window appearance
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    m_window = glfwCreateWindow(100, 100, "GP engine editor", nullptr, nullptr);

    if (!m_window)
    {
        fprintf(stderr, "GLFW window could not be initialized\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, windowFramebufferResized);
    glfwGetFramebufferSize(m_window, &m_framebufferWidth, &m_framebufferHeight);

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
}


void Editor::initGlad()
{
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
    {
        fprintf(stderr, "Failed to initialize OpenGL context\n");
        glfwDestroyWindow(m_window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}


void Editor::initDearImGui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}




/* ========================== Constructor & destructor ========================== */
Editor::Editor()
    : m_window{nullptr}, m_framebufferWidth{0}, m_framebufferHeight{0}
{
    initGLFW();
    initGlad();
    initDearImGui();

    // Wait for the full context to be initialized to show the window
    glfwShowWindow(m_window);
}


Editor::~Editor()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}


void Editor::update(double unscaledDeltaTime, double deltaTime)
{
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Anther one");
        ImGui::Text("bites the dust");
    ImGui::End();
    ImGui::Begin("Never gonna leave you up");
        ImGui::Text("Never gonna let you down");
    ImGui::End();
}


void Editor::fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime)
{

}


void Editor::render()
{
    // Rendering
    ImGui::Render();

    glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    glfwSwapBuffers(m_window);
}


bool Editor::isRunning()
{
    return !glfwWindowShouldClose(m_window);
}

} // End of namespace Editor