﻿
#include <AL/alc.h>
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/Core/Tools/Raycast.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>

#include <PhysX/PxRigidActor.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <GLFW/glfw3.h>

#include <BasePlayer.hpp>
#include <Firearm/PPSH41.hpp>

#include <Generated/BasePlayer.rfk.h>
File_GENERATED

    using namespace GPG;
using namespace GPM;
using namespace GPE;

BasePlayer::BasePlayer(GPE::GameObject& owner) noexcept : BaseCharacter(owner)
{
    onPostLoad();
}

void BasePlayer::onPostLoad()
{
    enableUpdate(true);
    enableOnGUI(true);

    input  = &getOwner().getOrCreateComponent<GPE::InputComponent>();
    source = &getOwner().getOrCreateComponent<GPE::AudioComponent>();

    GPE::Wave testSound3("./resources/sounds/E_Western.wav", "Western");

    GPE::SourceSettings sourceSettings;
    sourceSettings.pitch = 1.f;
    sourceSettings.loop  = AL_TRUE;

    source->setSound("Western", "Western", sourceSettings);

    BaseCharacter::onPostLoad();
}

void BasePlayer::start()
{
    BaseCharacter::start();

    GAME_ASSERT(input, "null");
    GAME_ASSERT(source, "null");

    for (auto&& firearm : m_firearmsGO)
    {
        GAME_ASSERT(firearm.pData, "No gameObject");
        m_firearms.emplace_back(firearm.pData->getComponent<Firearm>());
        GAME_ASSERT(m_firearms.back(), "No firearm in gameObject");
    }

    GAME_ASSERT(m_groundParticleComponent.pData, "null");
    m_groundParticleComponent.pData->start();

    // Keys
    input->bindAction("forward", EKeyMode::KEY_DOWN, "Game", this, "forward");
    input->bindAction("backward", EKeyMode::KEY_DOWN, "Game", this, "backward");
    input->bindAction("left", EKeyMode::KEY_DOWN, "Game", this, "left");
    input->bindAction("right", EKeyMode::KEY_DOWN, "Game", this, "right");
    input->bindAction("jump", EKeyMode::KEY_DOWN, "Game", this, "jump");
    input->bindAction("exit", EKeyMode::KEY_PRESSED, "Game", this, "leave");
    input->bindAction("sprintStart", EKeyMode::KEY_PRESSED, "Game", this, "sprintStart");
    input->bindAction("sprintEnd", EKeyMode::KEY_RELEASED, "Game", this, "sprintEnd");
    input->bindAction("shoot", EKeyMode::KEY_DOWN, "Game", this, "shoot");
    input->bindAction("aim", EKeyMode::KEY_PRESSED, "Game", this, "aimBegin");
    input->bindAction("aim", EKeyMode::KEY_RELEASED, "Game", this, "aimEnd");
    input->bindAction("playAmbiantMusic", EKeyMode::KEY_PRESSED, "Game", this, "playAmbiantMusic");
    input->bindAction("playAmbiantMusicForce", EKeyMode::KEY_PRESSED, "Game", this, "playAmbiantMusicForce");
    input->bindAction("stopAllMusic", EKeyMode::KEY_PRESSED, "Game", this, "stopAllMusic");

    source->playSound("Western", true);

    { // Cursor
        GPE::InputManager& io = GPE::Engine::getInstance()->inputManager;
        io.setCursorTrackingState(true);
        io.setCursorLockState(true);
    }
}

void BasePlayer::rotate(const GPM::Vec2& deltaDisplacement)
{
    const Quat& orientation{transform().getSpacialAttribut().rotation};
    const Vec2  axis{deltaDisplacement.rotated90()};
    float       deltaTime = Engine::getInstance()->timeSystem.getDeltaTime();
    const Quat  rotX{Quat::angleAxis(axis.x * controller->getAngularSpeed() * deltaTime, Vec3::right())};
    const Quat  rotY{Quat::angleAxis(axis.y * controller->getAngularSpeed() * deltaTime, Vec3::up())};
    const Quat  newRot{rotY * orientation * rotX};

    transform().setRotation(newRot);
}

// TOOD: Detect whether we are in editor or launcher
void BasePlayer::leave()
{

    GPE::Engine::getInstance()->exit();
}

// Exemple de Raycast
void BasePlayer::raycastExample()
{
    GPE::Raycast ray;
    ray.fire(transform().getGlobalPosition(), transform().getVectorForward(), 100000);
    ray.fire(transform().getGlobalPosition(), transform().getVectorForward(), 100000);
    if (ray.hit.hasBlock)
    {
        GPE::GameObject* owner = static_cast<GPE::GameObject*>(ray.hit.block.actor->userData);
        if (owner)
        {
            std::cout << "yolo" << std::endl;
        }
    }
}

void BasePlayer::playAmbiantMusic()
{
    source->playSound("Western", false);
}

void BasePlayer::playAmbiantMusicForce()
{
    source->playSound("Western", true);
}

void BasePlayer::stopAllMusic()
{
    source->stopAllSound();
}

// size_arg (for each axis) < 0.0f: align to end, 0.0f: auto, > 0.0f: specified size
void displayLifeBar(float currentLife, float lifeMax, const ImVec2& size_arg)
{
    using namespace ImGui;

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    // static float r = style.FrameBorderSize;
    // DragFloat("Test", &r, 0.1);

    ImVec2 pos  = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
    ImRect bb(pos, {pos.x + size.x, pos.y + size.y});
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, 0))
        return;

    // Render
    float fraction = currentLife / lifeMax;
    fraction       = ImSaturate(fraction);

    // Background
    const float rounding       = 7.0f;
    const float borderSize     = 5.f;
    const float halfBorderSize = borderSize * 0.5f;
    bb.Max.y += borderSize;
    bb.Min.y += borderSize;
    window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor{0, 0, 0, 100}, rounding);
    window->DrawList->AddRect(bb.Min - ImVec2(halfBorderSize, halfBorderSize),
                              bb.Max + ImVec2(halfBorderSize, halfBorderSize), ImColor{0, 0, 0, 150}, rounding,
                              ImDrawCornerFlags_All, borderSize);

    bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
    const ImVec2 fill_br = ImVec2(ImLerp(bb.Min.x, bb.Max.x, fraction), bb.Max.y);
    RenderRectFilledRangeH(window->DrawList, bb, ImColor{255, 0, 0, 150}, 0.0f, fraction, 7.f);

    // Default displaying the fraction as percentage string, but user can override it
    char        overlay_buf[32];
    const char* overlay = nullptr;

    ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), "%.0f% / %.0f%", currentLife, lifeMax);
    overlay = overlay_buf;

    ImVec2 overlay_size = CalcTextSize(overlay, NULL);
    if (overlay_size.x > 0.0f)
        RenderTextClipped(ImVec2(ImClamp(fill_br.x + style.ItemSpacing.x, bb.Min.x,
                                         bb.Max.x - overlay_size.x - style.ItemInnerSpacing.x),
                                 bb.Min.y),
                          bb.Max, overlay, NULL, &overlay_size, ImVec2(0.0f, 0.5f), &bb);
}

void BasePlayer::onGUI()
{
    using namespace ImGui;
    const float ratio = ImGui::GetWindowSize().y / ImGui::GetWindowSize().x;

    if (displayDepthMenu)
    {
        ImVec2 size = {GetWindowSize().x / 4.f * ratio, GetWindowSize().y / 6.f * ratio};

        SetNextElementLayout(0.5f, 0.3f, size, EHAlign::Middle, EVAlign::Middle);
        if (ImGui::Button("Retry", size))
        {
            reloadScene();
            Engine::getInstance()->timeSystem.setTimeScale(1.0);
        }

        SetNextElementLayout(0.5f, 0.6f, size, EHAlign::Middle, EVAlign::Middle);
        if (ImGui::Button("Quitte", size))
        {
            closeApplication();
        }
    }
    else
    {
        ImVec2 size = {GetWindowSize().x / 1.2f * ratio, GetWindowSize().y / 15.f * ratio};

        SetNextElementLayout(0.5f, 0.f, size, EHAlign::Middle, EVAlign::Top);
        displayLifeBar(m_currentLife, m_maxLife, size);

        if (m_firearms.size())
        {
            size = ImGui::CalcTextSize("30/30");
            SetNextElementLayout(0.05f, 0.95f, size, EHAlign::Left, EVAlign::Bottom);
            Text("%d/%d", m_firearms.front()->getMagazine().getBulletsRemaining(),
                 m_firearms.front()->getMagazine().getCapacity());
        }
        size = ImGui::CalcTextSize("FPS : 144");
        SetNextElementLayout(0.95f, 0.f, size, EHAlign::Right, EVAlign::Top);
        Text("FPS : %0.0f", ImGui::GetIO().Framerate);
    }
}

void BasePlayer::update(double deltaTime)
{
    if (isDead())
    {
        if (!displayDepthMenu)
        {
            m_animDepthCounter += deltaTime;

            if (m_animDepthCounter >= m_animDepthCounterMax)
            {
                Engine::getInstance()->timeSystem.setTimeScale(0.0);
                m_animDepthCounter = 0;
                displayDepthMenu   = true;
                Engine::getInstance()->inputManager.setCursorTrackingState(false);
                Engine::getInstance()->inputManager.setCursorLockState(false);
            }
        }
    }
    else
    {
        // TODO: find a fix to relieve the user from having to check this, or leave it like that?
        if (GPE::Engine::getInstance()->inputManager.getInputMode() == "Game")
        {
            const GPM::Vec2 deltaPos = GPE::Engine::getInstance()->inputManager.getCursor().deltaPos;

            if (deltaPos.x || deltaPos.y)
                rotate(deltaPos);
        }
    }
}

void BasePlayer::shoot()
{
    if (GPE::Engine::getInstance()->inputManager.getInputMode() == "Game" && !isDead() && m_firearms.size())
    {
        m_firearms.front()->triggered();

        if (m_firearms.front()->isMagazineEmpty())
            m_firearms.front()->reload();
    }
}

void BasePlayer::aimBegin()
{
    if (GPE::Engine::getInstance()->inputManager.getInputMode() == "Game" && !isDead() && m_firearms.size())
    {
        m_firearms.front()->setAim(true);
    }
}

void BasePlayer::aimEnd()
{
    if (GPE::Engine::getInstance()->inputManager.getInputMode() == "Game" && !isDead() && m_firearms.size())
    {
        m_firearms.front()->setAim(false);
    }
}

void BasePlayer::updateListener()
{
    GPM::Vec3 pos                   = transform().getGlobalPosition();
    GPM::Vec3 forward               = transform().getVectorForward();
    GPM::Vec3 up                    = transform().getVectorUp();
    ALfloat   listenerOrientation[] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    alListenerfv(AL_ORIENTATION, listenerOrientation);
}