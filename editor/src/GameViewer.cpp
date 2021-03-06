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

void GameViewer::checkInputs(EditorStartup& startup)
{
    if (!ImGui::IsWindowHovered())
    {
        return;
    }

    // The cursor may be on the window's title bar or border, which we do not want to react to
    ImVec2 topLeft = ImGui::GetWindowPos();
    topLeft.y     += ImGui::GetWindowContentRegionMin().y;

    const ImVec2 bottomRight{topLeft.x + framebuffer.width(), topLeft.y + framebuffer.height()};

    if (!m_captureInputs
        && startup.game().state == EGameState::PLAYING
        && ImGui::IsWindowHovered()
        && ImGui::IsMouseHoveringRect(topLeft, bottomRight)
        && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        captureInputs();
    }

    // The input manager is not used here because this class and its methods cannot be serialized,
    // which prevents the use of InputComponent::bindAction()
    else if (m_captureInputs && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        releaseInputs();
    }
}

GameViewer::GameViewer(int width, int height)
    : framebuffer    {width, height},
      window         {GPE::Engine::getInstance()->window.getGLFWWindow()},
      m_captureInputs{false}
{
}

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
    InputManager& io = Engine::getInstance()->inputManager;
    io.setCursorLockState(true);
    io.setCursorTrackingState(true);
    io.setInputMode("Game");

    m_captureInputs = true;
}

void GameViewer::render(EditorStartup& startup)
{
    // Use the whole window content
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {.0f, .0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

    if (ImGui::Begin("Game view") && &startup.game() != nullptr)
    {
        checkInputs(startup);

        const ImVec2 size{ImGui::GetContentRegionAvail()};

        // Set active camera
        RenderSystem& sceneRenderer = Engine::getInstance()->sceneManager.getCurrentScene()->sceneRenderer;

        sceneRenderer.setDefaultMainCamera();
        if (Camera* mainCam = sceneRenderer.getMainCamera())
        {
            mainCam->setAspect(size.x / size.y);

            // Update game viewport
            const ImVec2       winPos{ImGui::GetWindowPos()};
            const ImGuiWindow* win   {ImGui::GetCurrentWindow()};
            startup.game().setViewport(winPos.x + win->Viewport->CurrWorkOffsetMin.x,
                                       winPos.y + win->Viewport->CurrWorkOffsetMin.y, size.x, size.y);

            // Resizing FBO
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
            ImVec2       winSize{ImGui::GetWindowSize()};
            winSize.x = (winSize.x - textSize.x) * .5f;
            winSize.y = (winSize.y - textSize.y) * .5f;

            ImGui::SetCursorPos({winSize.x, winSize.y});
            ImGui::Text(text);
        }
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}

} // namespace Editor