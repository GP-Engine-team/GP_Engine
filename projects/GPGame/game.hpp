#pragma once

#ifdef GAME_EXPORTS
#define GAME_API __declspec(dllexport)
#else
#define GAME_API __declspec(dllimport)
#endif

#include "Engine/Core/Game/AbstractGame.hpp"
#include <iostream>

class Game final : public AbstractGame
{
private:
	virtual void update() override final
	{

	}

public:
	Game() = default;
	virtual ~Game() final = default;
};

extern "C" GAME_API class AbstractGame* createGameInstance();
extern "C" GAME_API void destroyGameInstance(class AbstractGame * game);