#include "Editor/GameControlBar.hpp"
#include "Editor/Editor.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace GPE;

namespace Editor
{

GameControlBar::GameControlBar()
    : playButtonTex {{"..\\..\\editor\\resources\\play.png",  Texture::ETextureMinFilter::LINEAR, Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE, Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      pauseButtonTex{{"..\\..\\editor\\resources\\pause.png", Texture::ETextureMinFilter::LINEAR, Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE, Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      stopButtonTex {{"..\\..\\editor\\resources\\stop.png",  Texture::ETextureMinFilter::LINEAR, Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE, Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      buttonColors  {IM_COL32(66u, 150u, 255u, 102u), IM_COL32(50u, 50u, 50u, 255u)},
      buttonMask    {EButton::STOP}
{}


// TODO: decide what to do with "editor" when clicking play/pause/stop
void GameControlBar::render(Editor& editor)
{
    // Remove the docking tab bar
	ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class);

    // Make an undecorated window for the control buttons
    ImGui::Begin("Game controls", nullptr, ImGuiWindowFlags_NoDecoration);
    {
        ImVec2 buttonSize, cursorPos;

        { // Compute the size and position of the buttons
            const ImVec2 winSize    {ImGui::GetContentRegionAvail()};
            const float  extraSide  {ImGui::GetCurrentWindow()->WindowPadding.y * .5f};
            const float  side       {winSize.y + extraSide};

            buttonSize.x = buttonSize.y = side;
            cursorPos    = {winSize.x * .5f - side * 1.5f, extraSide};
        }

        // Render the "Play" button
        ImGui::SetCursorPos(cursorPos);
        ImGui::PushStyleColor(ImGuiCol_Button, buttonColors[(bool)(buttonMask & PLAY)]);
        ImGui::ImageButton((void*)(intptr_t)playButtonTex.getID(), buttonSize);
        ImGui::PopStyleColor();
        if (ImGui::IsItemClicked())
        {
            if (buttonMask & PLAY)
            {
                buttonMask = STOP;
                // TODO: "Play" is on. Tell the editor the game should be playing
            }
            else
            {
                buttonMask = PLAY;
                // TODO: "Play" is off. Tell the editor the game should be stopped
            }
        }

        // Render the "Pause" button
        cursorPos.x += buttonSize.x;
        ImGui::SetCursorPos(cursorPos);
	    ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, buttonColors[(bool)(buttonMask & PAUSE)]);
	    ImGui::ImageButton((void*)(intptr_t)pauseButtonTex.getID(), buttonSize);
        ImGui::PopStyleColor();
        if (ImGui::IsItemClicked())
        {
            if (buttonMask & PAUSE)
            {
                buttonMask = PLAY;
                // TODO: "Pause" is on. Tell the editor the game should be launch then immediatly paused
            }
            else
            {
                buttonMask = PAUSE;
                // TODO: "Pause" is off. Tell the editor the game should keep on playing
            }
        }
        
        // Render the "Stop" button
        cursorPos.x += buttonSize.x;
        ImGui::SetCursorPos(cursorPos);
	    ImGui::SameLine();
	    ImGui::ImageButton((void*)(intptr_t)stopButtonTex.getID(), buttonSize);

        if (ImGui::IsItemClicked())
        {
            buttonMask = STOP;
            // TODO: "Stop" was pressed. Tell the editor the game should be stopped
        }
    }
    ImGui::End();
}

} // End of namespace Editor