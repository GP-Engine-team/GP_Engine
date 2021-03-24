#pragma once

#include "Engine/Core/Game/AbstractGame.hpp"
#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/ECS/System/BehaviourSystem.hpp"
#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/ECS/System/RenderSystem.hpp"
#include "Engine/ECS/System/TimeSystem.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Resources/Scene.hpp"
#include "GameApiMacros.hpp"
#include <iostream>

class Game final : public AbstractGame
{
protected:
    GPE::Window&              win      = GPE::Engine::getInstance()->window;
    GPE::Renderer&            ren      = GPE::Engine::getInstance()->renderer;
    GPE::TimeSystem&          ts       = GPE::Engine::getInstance()->timeSystem;
    GPE::InputManager&        iManager = GPE::Engine::getInstance()->inputManager;
    GPE::BehaviourSystem&     bSys     = GPE::Engine::getInstance()->behaviourSystem;
    GPE::RenderSystem&        rSys     = GPE::Engine::getInstance()->renderSystem;
    GPE::ResourceManagerType& rm       = GPE::Engine::getInstance()->resourceManager;
    GPE::SceneManager&        sm       = GPE::Engine::getInstance()->sceneManager;

    int    fixedUpdateFrameCount   = 0;
    int    unFixedUpdateFrameCount = 0;
    double FPLogDelay              = 1.;

private:
    virtual void update(double unscaledDeltaTime, double deltaTime) override final
    {
        // AbstractGame::update(unscaledDeltaTime, deltaTime);
        bSys.update(static_cast<float>(deltaTime));
        ++unFixedUpdateFrameCount;

        sm.getCurrentScene()->world.updateSelfAndChildren();
    }

    virtual void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime) override final
    {
        AbstractGame::fixedUpdate(fixedUnscaledDeltaTime, fixedDeltaTime);
        ++fixedUpdateFrameCount;
        bSys.fixedUpdate(static_cast<float>(fixedDeltaTime));
    }

    virtual void render() override final
    {
        rSys.draw(rSys.defaultRenderPipeline());
    }

public:
    Game();

    virtual ~Game() final
    {
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
