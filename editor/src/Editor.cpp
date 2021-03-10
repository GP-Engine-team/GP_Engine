#include "Editor/Editor.hpp"

#include "imgui/imgui.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#include <cstdio>
#include <cstdlib>

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

    editor->m_framebufferWidth  = newWidth;
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

    m_window = glfwCreateWindow(1024, 720, "GP engine editor", nullptr, nullptr);

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
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}


void Editor::renderMenuBar() const
{
    if (ImGui::BeginMainMenuBar())
    {
        // File
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New");
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Save");
            ImGui::EndMenu();
        }

        // Edit
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::MenuItem("Edit something");
            ImGui::EndMenu();
        }

        // View
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::BeginMenu("Add window"))
            {
                ImGui::MenuItem("Viewport");
                ImGui::MenuItem("Scene graph");
                ImGui::MenuItem("Project browser");
                ImGui::MenuItem("Inspector");
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        // Options
        if (ImGui::BeginMenu("Options"))
        {
            // Menu content
            ImGui::MenuItem("Preferences");
            ImGui::EndMenu();
        }

        // Help
        if (ImGui::BeginMenu("Help"))
        {
            // Menu content
            ImGui::MenuItem("Useful links");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}


void Editor::renderLevelEditor() const
{
    ImGui::Begin("Level editor");
        ImGui::Text("Level editor");

        //sceneEditor.render();

    ImGui::End();
}


void Editor::renderInspector() const
{
    ImGui::Begin("Inspector");
        ImGui::Text("Inspector content");
    ImGui::End();
}


void Editor::renderSceneGraph() const
{
    ImGui::Begin("Scene graph");
        ImGui::Text("Scene graph hierarchy");
    ImGui::End();
}


void Editor::renderExplorer() const
{
    ImGui::Begin("Explorer");
        if (ImGui::BeginTabBar("Explorer"))
        {
            if (ImGui::BeginTabItem("Project"))
            {
                ImGui::Text("Project's content");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Logs"))
            {
                ImGui::Text("This is a log");
                ImGui::Text("And another log");
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Profiler"))
            {
                ImGui::Text("Plots, and graphs, and numbers...");
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    ImGui::End();
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


void Editor::update()
{
    // Listen to keyboard and mouse
    glfwPollEvents();

    // Initialize a new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Start drawing
    renderMenuBar();

    ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport());

    renderLevelEditor();
    renderSceneGraph();
    renderExplorer();
    renderInspector();
}


void Editor::render()
{
    ImGui::Render();

    glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);
    glClearColor(1.f, 1.f, 1.f, .0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    glfwSwapBuffers(m_window);
}


bool Editor::isRunning()
{
    return !glfwWindowShouldClose(m_window);
}

} // End of namespace Editor