#pragma once

#include <Engine/Core/Game/AbstractGame.hpp>
#include <Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp>
#include <Engine/Core/Rendering/Window/WindowGLFW.hpp>
#include <Engine/ECS/System/BehaviourSystem.hpp>
#include <Engine/ECS/System/InputManagerGLFW.hpp>
#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/ECS/System/RenderSystem.hpp>
#include <Engine/ECS/System/SystemsManager.hpp>
#include <Engine/ECS/System/TimeSystem.hpp>
#include <Engine/Resources/Scene.hpp>
#include <GameApiMacros.hpp>
#include <iostream>

class Game final : public AbstractGame
{
protected:
    GPE::Window&              win      = GPE::SystemsManager::getInstance()->window;
    GPE::Renderer&            ren      = GPE::SystemsManager::getInstance()->renderer;
    GPE::TimeSystem&          ts       = GPE::SystemsManager::getInstance()->timeSystem;
    GPE::InputManager&        iManager = GPE::SystemsManager::getInstance()->inputManager;
    GPE::BehaviourSystem&     bSys     = GPE::SystemsManager::getInstance()->behaviourSystem;
    GPE::RenderSystem&        rSys     = GPE::SystemsManager::getInstance()->renderSystem;
    GPE::ResourceManagerType& rm       = GPE::SystemsManager::getInstance()->resourceManager;
    GPE::SceneManager&        sm       = GPE::SystemsManager::getInstance()->sceneManager;
    GPE::PhysXSystem&         pXSys    = GPE::SystemsManager::getInstance()->physXSystem;

    int    fixedUpdateFrameCount   = 0;
    int    unFixedUpdateFrameCount = 0;
    double FPLogDelay              = 1.;

private:
    virtual void update(double unscaledDeltaTime, double deltaTime) override final
    {
        bSys.update(static_cast<float>(deltaTime));
        ++unFixedUpdateFrameCount;

        sm.getCurrentScene()->world.updateSelfAndChildren();
    }

    virtual void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime) override final
    {
        ++fixedUpdateFrameCount;

        pXSys.advance(fixedDeltaTime);
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
        GPE::Log::closeAndTryToCreateFile();
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
