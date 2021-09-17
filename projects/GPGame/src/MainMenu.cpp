#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>

#include <Engine/Serialization/FileExplorer.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <MainMenu.hpp>

#include <Generated/MainMenu.rfk.h>
File_GENERATED

    using namespace GPG;
using namespace GPM;
using namespace GPE;

MainMenu::MainMenu(GPE::GameObject& owner) noexcept : BehaviourComponent(owner)
{
    onPostLoad();
}

void MainMenu::onPostLoad()
{
    enableOnGUI(true);

    BehaviourComponent::onPostLoad();
}

void MainMenu::start()
{
    BehaviourComponent::start();

    GAME_ASSERT(m_buttonTexture, "No button texture selected");

    { // Cursor
        GPE::InputManager& io = GPE::Engine::getInstance()->inputManager;
        io.setCursorTrackingState(false);
        io.setCursorLockState(false);
    }
}

void MainMenu::onGUI()
{
    using namespace ImGui;
    const float ratio = ImGui::GetWindowSize().y / ImGui::GetWindowSize().x;

    ImVec2 size = {GetWindowSize().x / 2.f * ratio, GetWindowSize().y / 6.f * ratio};

    PushFont(GetIO().Fonts->Fonts[1]);
    SetNextTextLayout("Green portal", 0.5f, 0.1f);
    ImGui::TextUnformatted("Green portal");
    PopFont();

    const float previousFontScale = GetFont()->Scale;
    PushFont(GetIO().Fonts->Fonts[2]);
    SetWindowFontScale(1.f * ratio);

    SetNextElementLayout(0.5f, 0.3f, size, EHAlign::Middle, EVAlign::Middle);
    if (ImGui::imageButtonWithTextCenter((ImTextureID)m_buttonTexture->getID(), "Play", size))
    {
        loadNewScene(m_scenePath.c_str());
    }

    size = {GetWindowSize().x / 2.f * ratio, GetWindowSize().y / 6.f * ratio};
    SetNextElementLayout(0.5f, 0.8f, size, EHAlign::Middle, EVAlign::Middle);
    if (ImGui::imageButtonWithTextCenter((ImTextureID)m_buttonTexture->getID(), "Quit", size))
    {
        closeApplication();
    }
    PopFont();
    ImGui::SetWindowFontScale(previousFontScale);

    std::string creditsText = "Credits : \n - Jonathan Six \n - Sami Amara \n - Thomas Dallard \n - William Nardone";
    ImVec2      creditsTextSize = ImGui::CalcTextSize(creditsText.c_str());
    SetCursorPosX(GetWindowSize().x - creditsTextSize.x - 30);
    SetCursorPosY(GetWindowSize().y - creditsTextSize.y - 20);
    ImGui::Text(creditsText.c_str(), creditsTextSize);
}

MainMenu::~MainMenu() noexcept
{
}

void MainMenu::inspect(GPE::InspectContext& context)
{
    Component::inspect(context);

    defaultInspect(context);

    GPE::DataInspector::inspect(context, m_buttonTexture, "Button texture");

    ImGui::Text("Scene to load : ");
    ImGui::SameLine();
    if (ImGui::Button(m_scenePath.size() ? m_scenePath.c_str() : "None"))
    {
        m_scenePath = GPE::openFileExplorerAndGetRelativePath(L"Select Scene", {{L"Scene", L"*.GPScene"}}).string();
    }
}