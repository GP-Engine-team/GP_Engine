#pragma once

#include "GameApiMacros.hpp"
#include "Engine/ECS/System/SystemsManager.hpp"

extern "C" GAME_API void setGameSystemsManagerInstance(GPE::SystemsManager& systemManager);
extern "C" GAME_API GPE::SystemsManager& getGameSystemsManagerInstance();

extern "C" GAME_API void setContextCurrent(GLFWwindow* window);
extern "C" GAME_API void setImguiCurrentContext(struct ImGuiContext* ctx);