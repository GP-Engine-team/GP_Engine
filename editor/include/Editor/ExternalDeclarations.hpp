#pragma once

/**
 * @brief Signature of createGameInstance, whose definition should be inside a dll.
 */
class AbstractGame* createGameInstance()
{
    return nullptr;
}
/**
 * @brief Signature of destroyGameInstance, whose definition should be inside a dll.
 */
void destroyGameInstance(class AbstractGame* game)
{
}
/**
 * @brief Signature of setGameEngineInstance, whose definition should be inside a dll.
 */
void setGameEngineInstance(GPE::Engine& engine)
{
}

void setImguiCurrentContext(struct ImGuiContext* ctx)
{
}

void setLogInstance(GPE::Log& log)
{
}

void setContextCurrent(GLFWwindow* window)
{
}
