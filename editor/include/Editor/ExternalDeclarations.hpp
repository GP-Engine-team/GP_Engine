#pragma once

/**
 * @brief Signature of createGameInstance, whose definition should be inside a dll.
*/
inline class GPE::AbstractGame* createGameInstance() { return nullptr; }
/**
 * @brief Signature of destroyGameInstance, whose definition should be inside a dll.
*/
inline void destroyGameInstance(class GPE::AbstractGame* game) {}
/**
 * @brief Signature of setGameEngineInstance, whose definition should be inside a dll.
*/
inline void setGameEngineInstance(GPE::Engine& engine) {}

inline void setImguiCurrentContext(struct ImGuiContext* ctx) {}

inline struct ImGuiContext* getGameUIContext() {}

inline void setLogInstance(GPE::Log& log) {}

inline void saveCurrentScene(XmlSaver& context) {}
inline void loadCurrentScene(XmlLoader& context) {}
