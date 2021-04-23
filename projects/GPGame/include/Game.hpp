#pragma once

#include <Engine/Core/Game/AbstractGame.hpp>
#include <Engine/Resources/ResourcesManagerType.hpp>
#include "GameApiMacros.hpp"

struct GLFWwindow;

namespace GPE
{

class Window;
class Renderer;
class TimeSystem;
class InputManager;
class BehaviourSystem;
class SceneManager;
}

class Game final : public GPE::AbstractGame
{
protected:
    GPE::Window&              win;
    GPE::Renderer&            ren;
    GPE::TimeSystem&          ts;
    GPE::InputManager&        iManager;
    GPE::BehaviourSystem&     bSys;
    GPE::ResourceManagerType& rm;
    GPE::SceneManager&        sm;

    double FPLogDelay              = 1.;
    float  m_x = .0f, m_y = .0f, m_w = .0f, m_h = .0f;
    int    fixedUpdateFrameCount   = 0;
    int    unFixedUpdateFrameCount = 0;

private:
    virtual void update     (double unscaledDeltaTime, double deltaTime)           override final;
    virtual void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime) override final;
    virtual void render     ()                                                     override final;

public:
    Game();
    virtual ~Game() final;

    void initDearImGui(GLFWwindow* window);
    void setViewport  (float x, float y, float w, float h);

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
