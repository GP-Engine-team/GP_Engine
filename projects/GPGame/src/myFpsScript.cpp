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

#include <Firearm/PPSH41.hpp>
#include <MyFpsScript.hpp>

#include <Generated/myFpsScript.rfk.h>
File_GENERATED

namespace GPG
{

MyFpsScript::MyFpsScript(GPE::GameObject & owner) noexcept
    : GPE::BehaviourComponent(owner), input{&owner.addComponent<GPE::InputComponent>()},
        source{&owner.addComponent<GPE::AudioComponent>()},
        controller{&owner.addComponent<GPE::CharacterController>()}, m_fireArme{&owner.addComponent<PPSH41>()}
{
    enableFixedUpdate(true);
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

    // Setup controller
    controller->setHasGravity(true);
    controller->setSpeed(1.f);
    controller->setMouseSpeed(.0025f);
    controller->setGravity(.1f);
}

void MyFpsScript::start()
{
    source->playSound("Western", true);
}

void MyFpsScript::onPostLoad()
{
    BehaviourComponent::onPostLoad();

    GPE::Wave testSound3("./resources/sounds/E_Western.wav", "Western");

    GPE::SourceSettings sourceSettings;
    sourceSettings.pitch = 1.f;
    sourceSettings.loop  = AL_TRUE;

    source->setSound("Western", "Western", sourceSettings);
}

void MyFpsScript::rotate(const GPM::Vec2& deltaDisplacement)
{
    using namespace GPM;

    const Quat& orientation{getOwner().getTransform().getSpacialAttribut().rotation};
    const Vec2  axis       {deltaDisplacement.rotated90()};
    const Quat  rotX       {Quat::angleAxis(axis.x * controller->getMouseSpeed(), Vec3::right())};
    const Quat  rotY       {Quat::angleAxis(axis.y * controller->getMouseSpeed(), Vec3::up())};
    const Quat  newRot     {rotY * orientation * rotX};

    getOwner().getTransform().setRotation(newRot);
}

void MyFpsScript::jump()
{
    if (controller->getJumping() == false)
    {
        controller->addForce(GPM::Vec3::up() * 3.f);
        controller->setJumping(true);
    }
}

void MyFpsScript::forward()
{
    GPM::Vec3 vec = getOwner().getTransform().getVectorForward();
    vec.y         = .0f;
    controller->move(vec);
    // rigidbody.rigidbody->addForce(vec * -speed, physx::PxForceMode::eFORCE);
}

void MyFpsScript::backward()
{
    GPM::Vec3 vec = getOwner().getTransform().getVectorForward();
    vec.y         = .0f;
    controller->move(-vec);
    // rigidbody.rigidbody->addForce(vec * speed, physx::PxForceMode::eFORCE);
}

void MyFpsScript::left()
{
    GPM::Vec3 vec = getOwner().getTransform().getVectorRight();
    vec.y         = .0f;
    controller->move(-vec);
    // rigidbody.rigidbody->addForce(vec * -speed, physx::PxForceMode::eFORCE);
}

void MyFpsScript::right()
{
    GPM::Vec3 vec = getOwner().getTransform().getVectorRight();
    vec.y         = .0f;
    controller->move(vec);
    // rigidbody.rigidbody->addForce(vec * speed, physx::PxForceMode::eFORCE);
}

// TOOD: Detect whether we are in editor or launcher
void MyFpsScript::leave()
{

    GPE::Engine::getInstance()->exit();
}

void MyFpsScript::sprintStart()
{
    controller->setSpeed(controller->getSpeed() * 2.f);
}

void MyFpsScript::sprintEnd()
{
    controller->setSpeed(controller->getSpeed() * .5f);
}

// Exemple de Raycast
void MyFpsScript::raycastExample()
{
    GPE::Raycast ray;
    ray.fire(getOwner().getTransform().getGlobalPosition(), getOwner().getTransform().getVectorForward(), 100000);
    if (ray.hit.hasBlock)
    {
        GPE::GameObject* owner = static_cast<GPE::GameObject*>(ray.hit.block.actor->userData);
        if (owner)
        {
            std::cout << "yolo" << std::endl;
        }
    }
}

void MyFpsScript::shoot()
{
    m_fireArme->triggered();

    if (m_fireArme->isMagazineEmpty())
        m_fireArme->reload();
}

void MyFpsScript::playAmbiantMusic()
{
    source->playSound("Western", false);
}

void MyFpsScript::playAmbiantMusicForce()
{
    source->playSound("Western", true);
}

void MyFpsScript::stopAllMusic()
{
    source->stopAllSound();
}
/*
void MyFpsScript::growUpSphereCollider()
{
    // collider.setRadius(collider.getRadius() + 1);
}


void MyFpsScript::growDownSphereCollider()
{
    // collider.setRadius(collider.getRadius() - 1);
}
*/

void MyFpsScript::onGUI()
{
    static float ratio = 0.08f;

    ImGui::DragFloat("Ratio", &ratio, 0.01f, 0.f, 1.f);

    ImVec2 size = {ImGui::GetWindowSize().x * ratio, ImGui::GetWindowSize().y * ratio};

    ImGui::SetNextElementLayout(0.f, 0.f, size, ImGui::EHAlign::Left, ImGui::EVAlign::Top);
    ImGui::Text("%d/%d", m_fireArme->getMagazine().getBulletsRemaining(), m_fireArme->getMagazine().getCapacity());
}

void MyFpsScript::fixedUpdate(double deltaTime)
{
    controller->update(deltaTime);
}

void MyFpsScript::update(double deltaTime)
{
    // TODO: find a fix to relieve the user from having to check this, or leave it like that?
    if (GPE::Engine::getInstance()->inputManager.getInputMode() == "Game")
    {
        const GPM::Vec2 deltaPos = GPE::Engine::getInstance()->inputManager.getCursor().deltaPos;

        if (deltaPos.x || deltaPos.y)
            rotate(deltaPos);
    }
}

} // End of namespace GPG