#pragma once

#include "Engine/Core/Game/AbstractGame.hpp"
#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/ECS/System/BehaviourSystem.hpp"
#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/ECS/System/TimeSystem.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Resources/Scene.hpp"
#include "GameApiMacros.hpp"
#include <iostream>

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include <imgui/imgui.h>

class Game final : public AbstractGame
{
protected:
    GPE::Window&              win      = GPE::Engine::getInstance()->window;
    GPE::Renderer&            ren      = GPE::Engine::getInstance()->renderer;
    GPE::TimeSystem&          ts       = GPE::Engine::getInstance()->timeSystem;
    GPE::InputManager&        iManager = GPE::Engine::getInstance()->inputManager;
    GPE::BehaviourSystem&     bSys     = GPE::Engine::getInstance()->behaviourSystem;
    GPE::ResourceManagerType& rm       = GPE::Engine::getInstance()->resourceManager;
    GPE::SceneManager&        sm       = GPE::Engine::getInstance()->sceneManager;

    int    fixedUpdateFrameCount   = 0;
    int    unFixedUpdateFrameCount = 0;
    double FPLogDelay              = 1.;

private:
    virtual void update(double unscaledDeltaTime, double deltaTime) override final
    {
        // Initialize a new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ++unFixedUpdateFrameCount;

        bSys.update(deltaTime);
        sm.getCurrentScene()->getWorld().updateSelfAndChildren();

        int h, w;
        Engine::getInstance()->window.getSize(w, h); //TODO: Fixe it when glfw will be fixed
        ImGui::SetNextWindowSize(ImVec2{(float)w, (float)h});
        ImGui::SetNextWindowPos({0.f, 0.f});

        ImGui::Begin("UI", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

        // Draw GUI
        GPE::Engine::getInstance()->behaviourSystem.onGUI();

        ImGui::End();
    }

    virtual void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime) override final
    {
        AbstractGame::fixedUpdate(fixedUnscaledDeltaTime, fixedDeltaTime);
        // GPE::Engine::getInstance()->physXSystem.advance(fixedDeltaTime);
        ++fixedUpdateFrameCount;
        bSys.fixedUpdate(fixedDeltaTime);
    }

    virtual void render() override final
    {
        GPE::SceneRenderSystem& sceneRS = GPE::Engine::getInstance()->sceneManager.getCurrentScene()->sceneRenderer;
        sceneRS.draw(GPE::Engine::getInstance()->resourceManager, sceneRS.defaultRenderPipeline());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

protected:
    void initDearImGui(GLFWwindow* window)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

public:
    Game();

    virtual ~Game() final
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
};

/**
 * @brief Called by the exe, by retriewing the dll functions. Can't be inlined.
 */
extern "C" GAME_API class AbstractGame* createGameInstance();
/**
 * @brief Called by the exe, by retriewing the dll functions. Can't be inlined.
 * The class is created in the dll, so it must be deleted in the dll.
 */
extern "C" GAME_API void destroyGameInstance(class AbstractGame* game);
