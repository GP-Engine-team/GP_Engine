#pragma once

#include "Engine/Core/Game/ContextStartup.hpp"
#include <functional>

struct GLFWwindow;

namespace GPE
{
class AbstractGame;
}

class GameStartup final : public ContextStartup
{
private:
    GPE::AbstractGame* m_game = nullptr;

    struct
    {
        std::function<void(double, double)> update;
        std::function<void(double, double)> fixedUpdate;
        std::function<void()>               render;
    } gameFunctionsPtr;

public:
    GameStartup();

    virtual void update() override final;

    virtual ~GameStartup() final;
};