#include <Editor/GameViewer.hpp>

#include <Editor/EditorStartup.hpp>

#include <Engine/Core/Game/AbstractGame.hpp>
#include <Engine/ECS/Component/Camera.hpp>
#include <Engine/Engine.hpp>

#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Editor
{

using namespace GPE;

void GameViewer::captureInputs()
{
    InputManager& io = Engine::getInstance()->inputManager;
    io.setCursorLockState(cursorLockStateInGame);
    io.setCursorTrackingState(cursorTrackingStateInGame);

    // TODO : Clamp mouse in window

    lockInputToGame();
}

void GameViewer::lockInputToGame()
{
    setMouseInWindow = true;
    Engine::getInstance()->inputManager.setInputMode("Game");
    m_captureInputs = true;

    // TODO : Clamp mouse in window
}

void GameViewer::lockInputToEditor()
{
    InputManager& io = Engine::getInstance()->inputManager;

    // Memorise the previous state
    cursorLockStateInGame     = io.getCursorLockState();
    cursorTrackingStateInGame = io.getCursorTrackingState();

    // Set the previous state
    io.setCursorLockState(false);
    io.setCursorTrackingState(false);
    io.setInputMode("Editor");

    m_captureInputs = false;
}

GameViewer::GameViewer(int width, int height)
    : framebuffer{width, height}, window{GPE::Engine::getInstance()->window.getGLFWWindow()}, m_captureInputs{false}
{
}

void GameViewer::render(EditorStartup& startup)
{
    // Use the whole window content
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {.0f, .0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

    if (ImGui::Begin("Game view"))
    {
        // Decide what to do with inputs
        if (startup.game().state == EGameState::PLAYING && !m_captureInputs && ImGui::IsWindowHovered() &&
            ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            captureInputs();
        }

        // The input manager is not used here because this class and its methods cannot be serialized,
        // which prevents the use of InputComponent::bindAction()
        else if (m_captureInputs && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            lockInputToEditor();
        }

        const ImVec2 size{ImGui::GetContentRegionAvail()};

        if (setMouseInWindow)
        {
            Engine::getInstance()->inputManager.setMousePos(
                {ImGui::GetWindowPos().x + size.x / 2.f, ImGui::GetWindowPos().y + size.y / 2.f});
            setMouseInWindow = false;
        }

        // Set active camera
        RenderSystem& sceneRenderer = Engine::getInstance()->sceneManager.getCurrentScene()->sceneRenderer;

        sceneRenderer.setDefaultMainCamera();
        if (Camera* mainCam = sceneRenderer.getMainCamera())
        {
            mainCam->setAspect(size.x / size.y);

            // Update game viewport
            const ImVec2       winPos{ImGui::GetWindowPos()};
            const ImGuiWindow* win{ImGui::GetCurrentWindow()};
            startup.game().setViewport(winPos.x + win->Viewport->CurrWorkOffsetMin.x,
                                       winPos.y + win->Viewport->CurrWorkOffsetMin.y, size.x, size.y);

            // Update camera's aspect and resizing FBO
            framebuffer.resize(int(size.x), int(size.y));
            framebuffer.bind();

            // Render
            startup.game().render();
            ImGui::Image((void*)(intptr_t)framebuffer.textureID(), size, {.0f, 1.f}, {1.f, .0f});
        }
        else
        {
            const char*  text     = "No main camera set";
            const ImVec2 textSize = ImGui::CalcTextSize(text);
            ImGui::SetCursorPos(
                {ImGui::GetWindowSize().x / 2.f - textSize.x / 2.f, ImGui::GetWindowSize().y / 2.f - textSize.y / 2.f});
            ImGui::Text(text);
        }
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}

} // namespace Editor