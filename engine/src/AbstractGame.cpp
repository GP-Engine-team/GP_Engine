#include "Engine/Core/Game/AbstractGame.hpp"
#include <Engine/Engine.hpp>

AbstractGame::AbstractGame()
{
}

void AbstractGame::fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime)
{
    GPE::Engine::getInstance()->physXSystem.advance(fixedDeltaTime);
}
