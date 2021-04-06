#pragma once

/**
 * @brief Signature of createGameInstance, whose definition should be inside a dll.
*/
inline class AbstractGame* createGameInstance() { return nullptr; }
/**
 * @brief Signature of destroyGameInstance, whose definition should be inside a dll.
*/
inline void destroyGameInstance(class AbstractGame* game) {}
/**
 * @brief Signature of setGameEngineInstance, whose definition should be inside a dll.
*/
inline void setGameEngineInstance(GPE::Engine& engine) {}

inline void setImguiCurrentContext(struct ImGuiContext* ctx) {}

inline void setLogInstance(GPE::Log& log) {}

inline void saveScene(XmlSaver& context, GPE::GameObject* const& inspected, const XmlSaver::SaveInfo& info) {}
inline void loadScene(XmlLoader& context, GPE::GameObject*& inspected, const XmlLoader::LoadInfo& info) {}
