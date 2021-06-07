
#include <AL/alc.h>
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/Core/Tools/Interpolation.hpp>
#include <Engine/Core/Tools/Raycast.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>
#include <Engine/Serialization/FileExplorer.hpp>
#include <gpm/Random.hpp>

#include <PhysX/PxRigidActor.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <GLFW/glfw3.h>

#include <BasePlayer.hpp>
#include <Firearm/PPSH41.hpp>
#include <Loot.hpp>

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
    sourceSettings.pitch    = 1.f;
    sourceSettings.loop     = AL_TRUE;
    sourceSettings.relative = AL_TRUE;

    source->setSound("Western", "Western", sourceSettings);

    getOwner().getTransform().OnUpdate += Function::make(this, "updateListener");

    BaseCharacter::onPostLoad();
}

void BasePlayer::start()
{
    BaseCharacter::start();

    GAME_ASSERT(m_evacuationPoint.pData, "Missing m_evacuationPoint");
    GAME_ASSERT(m_buttonTexture, "No button texture selected");
    GAME_ASSERT(input, "null");
    GAME_ASSERT(source, "null");
    GAME_ASSERT(m_firearmsGO.size(), "null");

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
    input->bindAction("aimBegin", EKeyMode::KEY_PRESSED, "Game", this, "aimBegin");
    input->bindAction("aimEnd", EKeyMode::KEY_RELEASED, "Game", this, "aimEnd");
    input->bindAction("playAmbiantMusic", EKeyMode::KEY_PRESSED, "Game", this, "playAmbiantMusic");
    input->bindAction("playAmbiantMusicForce", EKeyMode::KEY_PRESSED, "Game", this, "playAmbiantMusicForce");
    input->bindAction("stopAllMusic", EKeyMode::KEY_PRESSED, "Game", this, "stopAllMusic");
    input->bindAction("reload", EKeyMode::KEY_PRESSED, "Game", this, "reload");

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
    const float deltaTime = float(Engine::getInstance()->timeSystem.getDeltaTime());
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
void displayStaminaBar(float staminCount, float staminaMax, const ImVec2& size_arg)
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
    float fraction = staminCount / staminaMax;
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

    RenderRectFilledRangeH(window->DrawList, bb, ImColor{255, 0, 0, 150}, 0.0f, fraction, rounding);

    // Default displaying the fraction as percentage string, but user can override it
    char        overlay_buf[32];
    const char* overlay = nullptr;

    ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), "%0.2f% / %0.0f%", fraction, staminaMax);
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
        ImVec2 size = {GetWindowSize().x / 2.f * ratio, GetWindowSize().y / 6.f * ratio};

        const float previousFontScale = GetFont()->Scale;
        SetWindowFontScale(2.f * ratio);

        SetNextElementLayout(0.5f, 0.3f, size, EHAlign::Middle, EVAlign::Middle);
        if (ImGui::imageButtonWithTextCenter((void*)(intptr_t)m_buttonTexture->getID(), "Retry", size))
        {
            reloadScene();
            Engine::getInstance()->timeSystem.setTimeScale(1.0);
        }

        size = {GetWindowSize().x / 2.f * ratio, GetWindowSize().y / 6.f * ratio};
        SetNextElementLayout(0.5f, 0.6f, size, EHAlign::Middle, EVAlign::Middle);
        if (ImGui::imageButtonWithTextCenter((void*)(intptr_t)m_buttonTexture->getID(), "Quit", size))
        {
            closeApplication();
        }
        ImGui::SetWindowFontScale(previousFontScale);
    }
    else if (displayWinMenu)
    {
        const float ratio = ImGui::GetWindowSize().y / ImGui::GetWindowSize().x;

        ImVec2 size = {GetWindowSize().x / 2.f * ratio, GetWindowSize().y / 6.f * ratio};

        PushFont(GetIO().Fonts->Fonts[1]);
        SetNextTextLayout("Evacuation success !", 0.5f, 0.1f);
        ImGui::TextUnformatted("Evacuation success !");
        PopFont();

        const float previousFontScale = GetFont()->Scale;
        SetWindowFontScale(2.f * ratio);

        SetNextElementLayout(0.5f, 0.5f, size, EHAlign::Middle, EVAlign::Middle);
        if (ImGui::imageButtonWithTextCenter((void*)(intptr_t)m_buttonTexture->getID(), "Main menu", size))
        {
            Engine::getInstance()->timeSystem.setTimeScale(1.f);
            loadNewScene(m_mainMenuPath.c_str());
        }

        size = {GetWindowSize().x / 2.f * ratio, GetWindowSize().y / 6.f * ratio};
        SetNextElementLayout(0.5f, 0.7f, size, EHAlign::Middle, EVAlign::Middle);
        if (ImGui::imageButtonWithTextCenter((void*)(intptr_t)m_buttonTexture->getID(), "Quit", size))
        {
            closeApplication();
        }
        ImGui::SetWindowFontScale(previousFontScale);
    }
    else
    {
        ImVec2 size = {GetWindowSize().x / 1.2f * ratio, GetWindowSize().y / 15.f * ratio};

        // Life bar
        SetNextElementLayout(0.5f, 0.f, size, EHAlign::Middle, EVAlign::Top);
        displayBar(m_currentLife, m_maxLife, size, 5.f, 3.f);

        // Stamina bar
        ImGui::SetCursorPosX(
            ImGui::GetStyle().FramePadding.x + ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.x +
            (ImGui::GetWindowSize().x - ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.x) * 0.5f -
            size.x * 0.5f);
        size.y /= 3.f;
        displayBar(m_staminaCount, m_staminaMax, size, 2.f, 2.f, {255, 255, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 255},
                   "%.2f% / %.0f%");

        if (isInEvacuationMode)
        {
            // Evacuation
            ImGui::SetCursorPosX(
                ImGui::GetStyle().FramePadding.x + ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.x +
                (ImGui::GetWindowSize().x - ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.x) * 0.5f -
                size.x * 0.5f);
            displayBar(m_evacuationPoint.pData->getTimer(), m_evacuationPoint.pData->getTimerDuration(), size, 2.f, 2.f,
                       {0, 255, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 255}, "%.2f% / %.0f%");
        }

        // Fire arm stats
        if (m_firearms.size())
        {
            size = ImGui::CalcTextSize("30/30");
            SetNextElementLayout(0.05f, 0.95f, size, EHAlign::Left, EVAlign::Bottom);
            Text("%d/%d", m_firearms.front()->getMagazine().getBulletsRemaining(),
                 m_firearms.front()->getMagazine().getCapacity());
        }

        // FPS
        size = ImGui::CalcTextSize("FPS : 144");
        SetNextElementLayout(0.95f, 0.f, size, EHAlign::Right, EVAlign::Top);
        Text("FPS : %0.0f", ImGui::GetIO().Framerate);

        // Loot count
        size = ImGui::CalcTextSize("0 / 6");
        SetNextElementLayout(0.95f, 0.95f, size, EHAlign::Left, EVAlign::Middle);
        Text("%d / %d", m_lootCount, m_lootCountToWin);
    }
}

void BasePlayer::update(double deltaTime)
{
    if (isDead())
    {
        if (!displayDepthMenu)
        {
            m_animDepthCounter += float(deltaTime);

            if (m_animDepthCounter >= m_animDepthCounterMax && !displayDepthMenu)
            {
                Engine::getInstance()->timeSystem.setTimeScale(0.0);
                Engine::getInstance()->inputManager.setCursorTrackingState(false);
                Engine::getInstance()->inputManager.setCursorLockState(false);
                displayDepthMenu   = true;
                m_animDepthCounter = 0;
            }
        }
    }
    else
    {
        if (m_isSprint)
        {
            m_staminaCount -= float(deltaTime) * m_staminaSpeedConsumation;
            if (m_staminaCount <= 0.f)
            {
                m_staminaCount = 0.f;
                sprintEnd();
            }
        }
        else
        {
            m_staminaCount = std::min(m_staminaMax, m_staminaCount + float(deltaTime) * m_staminaSpeedRecharge);
        }

        // TODO: find a fix to relieve the user from having to check this, or leave it like that?
        if (GPE::Engine::getInstance()->inputManager.getInputMode() == "Game")
        {
            const GPM::Vec2 deltaPos = GPE::Engine::getInstance()->inputManager.getCursor().deltaPos;

            if (deltaPos.x || deltaPos.y)
                rotate(deltaPos);
        }

        if (m_isPlayDamageAnimation)
        {
            m_animDamageAnimCounter += float(deltaTime);

            const float t = std::clamp(m_animDamageAnimCounter / m_animDamageAnimCounterMax, 0.f, 1.f);
            updateDamageAnimation(t);

            if (m_animDamageAnimCounter >= m_animDamageAnimCounterMax)
            {
                m_isPlayDamageAnimation = false;
                m_animDamageAnimCounter = 0.f;
                m_cameraGO.pData->getTransform().setTranslation(Vec3::zero());
            }
        }
    }
}

void BasePlayer::reload()
{
    if (GPE::Engine::getInstance()->inputManager.getInputMode() == "Game" && m_firearms.size())
    {
        m_firearms.front()->reload();
    }
}

void BasePlayer::shoot()
{
    if (GPE::Engine::getInstance()->inputManager.getInputMode() == "Game" && m_firearms.size())
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

BasePlayer::~BasePlayer() noexcept
{
    getOwner().getTransform().OnUpdate -= Function::make(this, "updateListener");
}

void BasePlayer::collectLoot(const Loot& loot)
{
    if (++m_lootCount >= m_lootCountToWin)
    {
        m_evacuationPoint.pData->activeEvacutaitonPoint();
        isInEvacuationMode = true;
    }
}

void BasePlayer::onDeath()
{
    BaseCharacter::onDeath();

    input->setActive(false);
}

void BasePlayer::onWin()
{
    displayWinMenu = true;
    enableUpdate(false);
    Engine::getInstance()->timeSystem.setTimeScale(0.0);
    Engine::getInstance()->inputManager.setCursorTrackingState(false);
    Engine::getInstance()->inputManager.setCursorLockState(false);
}

void BasePlayer::updateDamageAnimation(float t)
{
    const float newT = easeInCirc(t);

    Vec3 moveStrength = Random::unitPeripheralSphericalCoordonate() * m_damageShakeStrength * (1.f - newT);

    moveStrength.z = 0.f;
    m_cameraGO.pData->getTransform().setTranslation(moveStrength); // Set the local rotation the be the rotation amount.
}

void BasePlayer::takeDamage(float damage)
{
    BaseCharacter::takeDamage(damage);
    m_isPlayDamageAnimation = true;
    m_animDamageAnimCounter = 0.f;
}

void BasePlayer::inspect(GPE::InspectContext& context)
{
    Component::inspect(context);

    defaultInspect(context);

    ImGui::Text("Main menu to load : ");
    ImGui::SameLine();
    if (ImGui::Button(m_mainMenuPath.size() ? m_mainMenuPath.c_str() : "None"))
    {
        m_mainMenuPath = GPE::openFileExplorerAndGetRelativePath(L"Select Scene", {{L"Scene", L"*.GPScene"}}).string();
    }
}