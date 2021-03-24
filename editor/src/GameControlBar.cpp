#include "Editor/GameControlBar.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace GPE;

namespace Editor
{

GameControlBar::GameControlBar()
    : playButtonTex {{"..\\..\\editor\\resources\\play.png",  Texture::ETextureMinFilter::LINEAR, Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE, Texture::ETextureWrapT::CLAMP_TO_EDGE, false}},
      pauseButtonTex{{"..\\..\\editor\\resources\\pause.png", Texture::ETextureMinFilter::LINEAR, Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE, Texture::ETextureWrapT::CLAMP_TO_EDGE, false}},
      stopButtonTex {{"..\\..\\editor\\resources\\stop.png",  Texture::ETextureMinFilter::LINEAR, Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE, Texture::ETextureWrapT::CLAMP_TO_EDGE, false}}
{}


void GameControlBar::render()
{
    // Remove the docking tab bar
	ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class);

    ImGui::Begin("Game controls", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiDockNodeFlags_NoTabBar);
        const ImVec2 winSize    {ImGui::GetContentRegionAvail()};
        const float  extraSide  {ImGui::GetCurrentWindow()->WindowPadding.y * .5f};
        const float  side       {winSize.y + extraSide};
        const ImVec2 buttonSize {side, side};
        const ImVec2 cursorPos  {(winSize.x - winSize.y) * .5f, extraSide};
          
        // Center the buttons
        ImGui::SetCursorPos(cursorPos);

        // Render the buttons
        ImGui::ImageButton((void*)(intptr_t)playButtonTex.getID(), buttonSize);
	    ImGui::SameLine();
	    ImGui::ImageButton((void*)(intptr_t)pauseButtonTex.getID(), buttonSize);
	    ImGui::SameLine();
	    ImGui::ImageButton((void*)(intptr_t)stopButtonTex.getID(), buttonSize);

    ImGui::End();
}

} // End of namespace Editor