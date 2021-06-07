#pragma once

#include "Engine/Core/HotReload/EngineApiMacros.hpp"
#include <Engine/Core/Game/AbstractGame.hpp>
#include <Engine/ECS/Component/ComponentInclude.hpp>

struct GLFWwindow;

namespace GPE
{
class BehaviourSystem;
class GameObject;
} // namespace GPE

class Game final : public GPE::AbstractGame
{
protected:
    double FPLogDelay = 1.;
    float  m_x = .0f, m_y = .0f, m_w = .0f, m_h = .0f;

public:
    Game();
    virtual ~Game() final;

    void update(double unscaledDeltaTime, double deltaTime) final;
    void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime) final;
    void render() final;

    void initDearImGui(GLFWwindow* window);
    void setViewport(float x, float y, float w, float h);
};

/**
 * @brief Called by the exe, by retriewing the dll functions. Can't be inlined.
 */
extern "C" ENGINE_API class GPE::AbstractGame* createGameInstance();
/**
 * @brief Called by the exe, by retriewing the dll functions. Can't be inlined.
 * The class is created in the dll, so it must be deleted in the dll.
 */
extern "C" ENGINE_API void destroyGameInstance(class GPE::AbstractGame* game);
