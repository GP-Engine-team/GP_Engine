#include "Editor/GameViewer.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Game/AbstractGame.hpp"
#include "glad/glad.h"

#include "imgui/imgui.h"

namespace Editor
{

GameViewer::GameViewer(int width, int height)
    : framebuffer{width, height}, m_captureInputs{false}
{}

void GameViewer::render(GPE::AbstractGame* game)
{
    // Use the whole window content
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {.0f, .0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

    ImGui::Begin("Game view");
    {
        const ImVec2 size{ImGui::GetContentRegionAvail()};

        framebuffer.resize(static_cast<int>(size.x), static_cast<int>(size.y));
        framebuffer.bind();
        game->render();

        ImGui::Image((void*)(intptr_t)framebuffer.textureID(), size, {.0f, 1.f}, {1.f, .0f});
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}

}