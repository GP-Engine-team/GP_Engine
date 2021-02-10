#pragma once

#include "Engine/Core/Game/ContextStartup.hpp"

class GameStartup final : public ContextStartup
{
private:
	class AbstractGame* game;

public:
	GameStartup();

	virtual void update() override final;

	virtual ~GameStartup() final;
};