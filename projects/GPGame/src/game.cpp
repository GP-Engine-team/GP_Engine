#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#define GLFW_INCLUDE_NONE

#include <BasePlayer.hpp>
#include <Game.hpp>

//#include <Engine/Resources/Mesh.hpp>
//#include <Engine/ECS/Component/AnimationComponent.hpp>
//#include <Engine/ECS/Component/Physics/Collisions/BoxCollider.hpp>
//#include <Engine/ECS/Component/Physics/Collisions/SphereCollider.hpp>

#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyDynamic.hpp>
#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyStatic.hpp>
#include <Engine/ECS/System/RenderSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Script/FreeFly.hpp>
#include <SpatializedSoundPlayerScript.hpp>

#include <Sun.hpp>
#include <WorldGenerator.hpp>

#include <GPM/Random.hpp>

// Third_party
#include <glfw/glfw3.h>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Engine/Core/HotReload/SingletonsSync.hpp"

#include <Engine/Core/Physics/Collisions/BoxCollider.hpp>
#include <Engine/Core/Physics/Collisions/SphereCollider.hpp>

using namespace GPG;
using namespace GPE;
using namespace GPM;
using namespace GPM::Random;

void Game::update(double unscaledDeltaTime, double deltaTime)
{
    ImGui_ImplGlfw_NewFrame();
    ImGui::GetIO().MousePos -= {m_x, m_y};
}

void Game::fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime)
{
}

void Game::render()
{
    // TODO: Put in-game UI in a module
    // UI code can be easly move in update because it's not real render function. It however her for simplicity
    // Initialize a new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::GetIO().DisplaySize = {m_w, m_h};

    ImGui::NewFrame();

    ImGui::SetNextWindowSize({m_w, m_h});
    ImGui::SetNextWindowPos({.0f, .0f});
    ImGui::Begin("UI", nullptr,
                 ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                     ImGuiWindowFlags_NoScrollWithMouse);

    // Draw GUI
    Engine::getInstance()->sceneManager.getCurrentScene()->behaviourSystem.onGUI();
    ImGui::End();
    ImGui::Render();
    RenderSystem& sceneRS = Engine::getInstance()->sceneManager.getCurrentScene()->sceneRenderer;

    sceneRS.tryToResize(unsigned(m_w), unsigned(m_h));
    sceneRS.render(sceneRS.defaultRenderPipeline());

    // draw UI
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

extern "C" GPE::AbstractGame* createGameInstance()
{
    // Init glad
    if (!gladLoadGL())
    {
        FUNCT_ERROR("gladLoadGLLoader failed");
        exit(EXIT_FAILURE);
    }
    GPE::AbstractGame* const pGame = new Game();
    return pGame;
}

extern "C" void destroyGameInstance(GPE::AbstractGame* game)
{
    GPE_ASSERT(game != nullptr, "m_editor should be valid since we've just ran the editor.");
    delete game;
    GPE::Engine::getInstance()->animSystem.clearCache();
    GPE::Engine::getInstance()->sceneManager.removeScenes();
    GPE::Engine::getInstance()->resourceManager.clearAll();
}

// TODO: Put in-game UI in a module
void Game::initDearImGui(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void Game::setViewport(float x, float y, float w, float h)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
}

// TODO: Put in-game UI in a module
Game::~Game()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Game::Game()
{
    // ============= UI =============
    // TODO: Put in-game UI in a module
    initDearImGui(GPE::Engine::getInstance()->window.getGLFWWindow());
    if (ImGui::GetIO().Fonts->AddFontFromFileTTF("./resources/fonts/Roboto-Medium.ttf", 14))
    {
        ImGui_ImplOpenGL3_CreateFontsTexture();
    }

    if (ImGui::GetIO().Fonts->AddFontFromFileTTF("./resources/fonts/AovelSansRounded-rdDL.ttf", 60))
    {
        ImGui_ImplOpenGL3_CreateFontsTexture();
    }

    if (ImGui::GetIO().Fonts->AddFontFromFileTTF("./resources/fonts/Roboto-Medium.ttf", 30))
    {
        ImGui_ImplOpenGL3_CreateFontsTexture();
    }

    // ============ RNG =============
    initSeed();

    // =========== Inputs ===========
    { // Keys
        InputManager& io = Engine::getInstance()->inputManager;

        io.bindInput(GLFW_KEY_W, "forward");
        io.bindInput(GLFW_KEY_S, "backward");
        io.bindInput(GLFW_KEY_A, "left");
        io.bindInput(GLFW_KEY_D, "right");
        io.bindInput(GLFW_KEY_R, "reload");
        io.bindInput(GLFW_KEY_SPACE, "jump");
        io.bindInput(GLFW_KEY_ESCAPE, "exit");
        io.bindInput(GLFW_KEY_LEFT_SHIFT, "sprintStart");
        io.bindInput(GLFW_KEY_LEFT_SHIFT, "sprintEnd");
        io.bindInput(GLFW_MOUSE_BUTTON_LEFT, "RaycastExample");
        io.bindInput(GLFW_MOUSE_BUTTON_LEFT, "shoot");
        io.bindInput(GLFW_MOUSE_BUTTON_RIGHT, "aimBegin");
        io.bindInput(GLFW_MOUSE_BUTTON_RIGHT, "aimEnd");
        io.bindInput(GLFW_KEY_KP_1, "playAmbiantMusic");
        io.bindInput(GLFW_KEY_KP_2, "playAmbiantMusicForce");
        io.bindInput(GLFW_KEY_KP_0, "stopAllMusic");

        // Cursor
        io.setCursorTrackingState(false);
    }

    Mesh* planeMesh = &Engine::getInstance()->resourceManager.add<Mesh>(
        "PlaneGround", Mesh::createQuad(0.5f, 0.5f, 100.f, 0, 0, Mesh::Axis::Y));
}
