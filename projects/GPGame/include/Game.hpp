#pragma once

#include "GameApiMacros.hpp"
#include <Engine/Core/Game/AbstractGame.hpp>

struct GLFWwindow;

namespace GPE
{
class BehaviourSystem;
class GameObject;
} // namespace GPE

class Game final : public GPE::AbstractGame
{
protected:
    GPE::GameObject& world;

    double FPLogDelay = 1.;
    float  m_x = .0f, m_y = .0f, m_w = .0f, m_h = .0f;
    int    fixedUpdateFrameCount   = 0;
    int    unFixedUpdateFrameCount = 0;

private:
    virtual void update(double unscaledDeltaTime, double deltaTime) override final;
    virtual void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime) override final;
    virtual void render() override final;

public:
    Game();
    virtual ~Game() final;

    void initDearImGui(GLFWwindow* window);
    void setViewport(float x, float y, float w, float h);
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
