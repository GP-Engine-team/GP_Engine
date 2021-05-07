#include <Editor/GameControlBar.hpp>
#include <Editor/EditorStartup.hpp>

#include <Engine/Core/Game/AbstractGame.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Editor
{

using namespace GPE;

GameControlBar::GameControlBar()
    : playButtonTex{{"..\\..\\editor\\resources\\play.png", Texture::ETextureMinFilter::LINEAR,
                     Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                     Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      pauseButtonTex{{"..\\..\\editor\\resources\\pause.png", Texture::ETextureMinFilter::LINEAR,
                      Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                      Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      stopButtonTex{{"..\\..\\editor\\resources\\stop.png", Texture::ETextureMinFilter::LINEAR,
                     Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                     Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      buttonColors{IM_COL32(66u, 150u, 255u, 102u), IM_COL32(50u, 50u, 50u, 255u)},
      buttonMask{STOPPED}
{
}


void GameControlBar::render(EditorStartup& startup)
{
    { // Remove the docking tab bar
        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&window_class);
    }

    buttonMask = startup.game().state;

    // Make an undecorated window for the control buttons
    if (ImGui::Begin("Game controls", nullptr, ImGuiWindowFlags_NoDecoration))
    {
        ImVec2 buttonSize, cursorPos;

        { // Compute the size and position of the buttons
            const ImVec2 winSize  {ImGui::GetContentRegionAvail()};
            const float  extraSide{ImGui::GetCurrentWindow()->WindowPadding.y * .5f};
            const float  side     {winSize.y + extraSide};

            buttonSize.x = buttonSize.y = side;
            cursorPos.x  = winSize.x * .5f - side * 1.5f;
            cursorPos.y  = extraSide;
        }

        // Render the "Play" button
        ImGui::SetCursorPos(cursorPos);
        ImGui::PushStyleColor(ImGuiCol_Button, buttonColors[(bool)(buttonMask & PLAYING)]);
        ImGui::ImageButton((void*)(intptr_t)playButtonTex.getID(), buttonSize);
        ImGui::PopStyleColor();
        if (ImGui::IsItemClicked())
        {
            if (buttonMask & PLAYING)
            {
                buttonMask = STOPPED;
                startup.stopGame();
            }
            else
            {
                buttonMask = PLAYING;
                startup.playGame();
            }
        }

        // Render the "Pause" button
        cursorPos.x += buttonSize.x;
        ImGui::SetCursorPos(cursorPos);
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, buttonColors[(bool)(buttonMask & PAUSED)]);
        ImGui::ImageButton((void*)(intptr_t)pauseButtonTex.getID(), buttonSize);
        ImGui::PopStyleColor();
        if (ImGui::IsItemClicked())
        {
            if (buttonMask & PAUSED)
            {
                buttonMask = PLAYING;
                startup.playGame();
            }
            else
            {
                buttonMask = PAUSED;
                startup.pauseGame();
            }
        }

        // Render the "Stop" button
        cursorPos.x += buttonSize.x;
        ImGui::SetCursorPos(cursorPos);
        ImGui::SameLine();
        ImGui::ImageButton((void*)(intptr_t)stopButtonTex.getID(), buttonSize);

        if (ImGui::IsItemClicked())
        {
            buttonMask = STOPPED;
            startup.stopGame();
        }
    }
    ImGui::End();
}

} // End of namespace Editor