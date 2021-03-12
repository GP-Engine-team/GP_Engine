#pragma once

#include "GameApiMacros.hpp"
#include "Engine/ECS/System/SystemsManager.hpp"

extern "C" GAME_API void setGameSystemsManagerInstance(GPE::SystemsManager& systemManager);