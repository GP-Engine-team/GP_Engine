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
    virtual void update(double unscaledDeltaTime, double deltaTime) override final;

    virtual void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime) override final;

    virtual void render() override final;

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
extern "C" GAME_API class GPE::AbstractGame* createGameInstance();
/**
 * @brief Called by the exe, by retriewing the dll functions. Can't be inlined.
 * The class is created in the dll, so it must be deleted in the dll.
 */
extern "C" GAME_API void destroyGameInstance(class GPE::AbstractGame* game);
