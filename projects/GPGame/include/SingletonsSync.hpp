#pragma once

#include "GameApiMacros.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Core/Debug/Log.hpp"

extern "C" GAME_API void setGameEngineInstance(GPE::Engine & engine);
extern "C" GAME_API void setLogInstance(GPE::Log & log);

extern "C" GAME_API GPE::Engine & getGameEngineInstance();
extern "C" GAME_API GPE::Log & getLogInstance();

extern "C" GAME_API void setContextCurrent(GLFWwindow* window);
extern "C" GAME_API void setImguiCurrentContext(struct ImGuiContext* ctx);

extern "C" GAME_API void saveScene(XmlSaver& context, GPE::GameObject* const& inspected, const XmlSaver::SaveInfo& info);
extern "C" GAME_API void loadScene(XmlLoader& context, GPE::GameObject*& inspected, const XmlLoader::LoadInfo& info);
