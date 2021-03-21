#pragma once

/**
 * @brief Signature of createGameInstance, whose definition should be inside a dll.
*/
class AbstractGame* createGameInstance() { return nullptr; }
/**
 * @brief Signature of destroyGameInstance, whose definition should be inside a dll.
*/
void destroyGameInstance(class AbstractGame* game) {}
/**
 * @brief Signature of setGameSystemsManagerInstance, whose definition should be inside a dll.
*/
void setGameSystemsManagerInstance(GPE::SystemsManager& systemManager) {}

void setImguiCurrentContext(struct ImGuiContext* ctx) {}