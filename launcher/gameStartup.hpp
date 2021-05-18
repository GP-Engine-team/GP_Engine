#pragma once

#include <Engine/Core/Game/ContextStartup.hpp>
#include <functional>

struct GLFWwindow;

namespace GPE
{
class AbstractGame;
class Engine;
}

class GameStartup final : public ContextStartup
{
private:
    GPE::Engine*       m_engine;
    GPE::AbstractGame* m_game;

    const std::function<void(double, double)> m_update;
    const std::function<void(double, double)> m_fixedUpdate;
    const std::function<void()>               m_render;

public:
    GameStartup();

    void update() final;

    virtual ~GameStartup() final;
};