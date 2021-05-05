#pragma once

#include "GameApiMacros.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Core/Debug/Log.hpp"

struct ImGuiContext;

extern "C" GAME_API void          setGameEngineInstance (GPE::Engine & engine);
extern "C" GAME_API void          setLogInstance        (GPE::Log & log);
extern "C" GAME_API GPE::Engine&  getGameEngineInstance ();
extern "C" GAME_API GPE::Log&     getLogInstance        ();
extern "C" GAME_API void          setImguiCurrentContext(struct ImGuiContext* ctx);
extern "C" GAME_API ImGuiContext* getGameUIContext();
extern "C" GAME_API void          saveScene(XmlSaver& context, GPE::Scene* scene);
extern "C" GAME_API void          loadScene(XmlLoader& context, GPE::Scene* scene);
extern "C" GAME_API void          saveSceneToPath(GPE::Scene* scene, const char* path, GPE::SavedScene::EType saveMode);
extern "C" GAME_API void          loadSceneFromPath(GPE::Scene* scene, const char* path);