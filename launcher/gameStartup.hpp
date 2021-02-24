#pragma once

#include "Engine/Core/Game/ContextStartup.hpp"
#include <functional>

class GameStartup final : public ContextStartup
{
private:
	class AbstractGame* m_game;

	struct
	{
		std::function<void(double, double)> update;
		std::function<void(double, double)> fixedUpdate;
		std::function<void()> render;
	} gameFunctionsPtr;

public:
	GameStartup();

	virtual void update() override final;

	virtual ~GameStartup() final;
};