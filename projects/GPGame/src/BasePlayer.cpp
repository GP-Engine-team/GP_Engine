#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/Core/Tools/Raycast.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>

#include <PhysX/PxRigidActor.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <BasePlayer.hpp>
#include <Firearm/PPSH41.hpp>

#include <Generated/BasePlayer.rfk.h>
File_GENERATED

    using namespace GPG;
using namespace GPM;
using namespace GPE;

BasePlayer::BasePlayer(GPE::GameObject& owner) noexcept
    : BaseCharacter(owner), input{&owner.addComponent<GPE::InputComponent>()},
      source{&owner.addComponent<GPE::AudioComponent>()}, m_fireArme{&owner.addComponent<PPSH41>()}
{
    enableUpdate(true);
    enableOnGUI(true);

    GPE::Wave testSound3("./resources/sounds/E_Western.wav", "Western");

    GPE::SourceSettings sourceSettings;
    sourceSettings.pitch = 1.f;
    sourceSettings.loop  = AL_TRUE;

    source->setSound("Western", "Western", sourceSettings);

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
    input->bindAction("playAmbiantMusic", EKeyMode::KEY_PRESSED, "Game", this, "playAmbiantMusic");
    input->bindAction("playAmbiantMusicForce", EKeyMode::KEY_PRESSED, "Game", this, "playAmbiantMusicForce");
    input->bindAction("stopAllMusic", EKeyMode::KEY_PRESSED, "Game", this, "stopAllMusic");

    { // Cursor
        GPE::InputManager& io = GPE::Engine::getInstance()->inputManager;
        io.setCursorTrackingState(true);
        io.setCursorLockState(true);
    }
}

void BasePlayer::start()
{
    BaseCharacter::start();

    // source->playSound("Western", true);
}

void BasePlayer::onPostLoad()
{
    BaseCharacter::onPostLoad();
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

void BasePlayer::onGUI()
{
    static float ratio = 0.08f;

    ImGui::DragFloat("Ratio", &ratio, 0.01f, 0.f, 1.f);

    ImVec2 size = {ImGui::GetWindowSize().x * ratio, ImGui::GetWindowSize().y * ratio};

    ImGui::SetNextElementLayout(0.f, 0.f, size, ImGui::EHAlign::Left, ImGui::EVAlign::Top);
    ImGui::Text("%d/%d", m_fireArme->getMagazine().getBulletsRemaining(), m_fireArme->getMagazine().getCapacity());
}

void BasePlayer::update(double deltaTime)
{
    // TODO: find a fix to relieve the user from having to check this, or leave it like that?
    if (GPE::Engine::getInstance()->inputManager.getInputMode() == "Game")
    {
        const GPM::Vec2 deltaPos = GPE::Engine::getInstance()->inputManager.getCursor().deltaPos;

        if (deltaPos.x || deltaPos.y)
            rotate(deltaPos);
    }
}

void BasePlayer::shoot()
{
    m_fireArme->triggered();

    if (m_fireArme->isMagazineEmpty())
        m_fireArme->reload();
}