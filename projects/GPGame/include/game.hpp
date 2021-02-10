#pragma once


#include "Engine/Core/Game/AbstractGame.hpp"
#include "GameApiMacros.hpp"

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

/**
 * @brief Called by the exe, by retriewing the dll functions. Can't be inlined.
*/
extern "C" GAME_API class AbstractGame* createGameInstance();
/**
 * @brief Called by the exe, by retriewing the dll functions. Can't be inlined.
 * The class is created in the dll, so it must be deleted in the dll.
*/
extern "C" GAME_API void destroyGameInstance(class AbstractGame * game);