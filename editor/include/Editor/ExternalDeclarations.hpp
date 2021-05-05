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

inline void saveScene(XmlSaver& context, GPE::Scene* scene) {}
inline void loadScene(XmlLoader& context, GPE::Scene* scene) {}

inline void saveSceneToPath(GPE::Scene* scene, const char* path, GPE::SavedScene::EType saveMode) {}
inline void loadSceneFromPath(GPE::Scene* scene, const char* path) {}