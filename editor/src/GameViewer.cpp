#include <Editor/GameViewer.hpp>

#include <Editor/EditorStartup.hpp>

#include <Engine/Core/Game/AbstractGame.hpp>
#include <Engine/ECS/Component/Camera.hpp>
#include <Engine/Engine.hpp>

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/imgui_internal.h>

namespace Editor
{

using namespace GPE;

void GameViewer::releaseInputs()
{
    InputManager& io = Engine::getInstance()->inputManager;
    io.setCursorLockState(false);
    io.setCursorTrackingState(false);
    io.restorePreviousInputMode();

    m_captureInputs = false;
}


void GameViewer::captureInputs()
{
    Log::getInstance()->log("GameViewer::captureInputs()");
    InputManager& io = Engine::getInstance()->inputManager;
    io.setCursorLockState(true);
    io.setCursorTrackingState(true);
    io.setInputMode("Game");

    m_captureInputs = true;
}


GameViewer::GameViewer(int width, int height)
    : framebuffer{width, height},
      window{GPE::Engine::getInstance()->window.getGLFWWindow()},
      m_captureInputs{false}
{}


void GameViewer::render(EditorStartup& startup)
{
    // Use the whole window content
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {.0f, .0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

    if (ImGui::Begin("Game view"))
    {
        // Decide what to do with inputs
        if (startup.game().state == EGameState::PLAYING   &&
            !m_captureInputs                              &&
            ImGui::IsWindowHovered()                      &&
            ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            captureInputs();
        }

        // The input manager is not used here because this class and its methods cannot be serialized
        else if (m_captureInputs && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            releaseInputs();
        }

        const ImVec2 size{ImGui::GetContentRegionAvail()};

        // Set main camera
        Camera& cam = Engine::getInstance()->sceneManager.getCurrentScene()->sceneRenderer.setMainCamera(0);

        // Update game viewport
        startup.game().setViewport(ImGui::GetWindowPos().x + ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.x,
                                   ImGui::GetWindowPos().y + ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.y,
                                   size.x, size.y);
        
        // Update camera's aspect and resizing FBO
        cam.setAspect(size.x / size.y);
        framebuffer.resize(int(size.x), int(size.y));
        framebuffer.bind();

        // Render
        startup.game().render();
        ImGui::Image((void*)(intptr_t)framebuffer.textureID(), size, {.0f, 1.f}, {1.f, .0f});
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}

} // namespace Editor