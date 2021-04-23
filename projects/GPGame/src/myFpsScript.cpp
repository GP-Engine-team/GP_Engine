#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <MyFpsScript.hpp>
File_GENERATED

namespace GPG
{

MyFpsScript::MyFpsScript(GPE::GameObject& owner) noexcept
    : GPE::BehaviourComponent(owner),
      input     {&owner.addComponent<GPE::InputComponent>()},
      source    {&owner.addComponent<GPE::AudioComponent>()},
      controller{&owner.addComponent<GPE::CharacterController>()}
{
    enableFixedUpdate(true);
    enableUpdate(true);
    enableOnGUI(true);

    input->bindAction("jump",                  EKeyMode::KEY_DOWN,     "game01", this, "jump");
    input->bindAction("right",                 EKeyMode::KEY_DOWN,     "game01", this, "right");
    input->bindAction("left",                  EKeyMode::KEY_DOWN,     "game01", this, "left");
    input->bindAction("forward",               EKeyMode::KEY_DOWN,     "game01", this, "forward");
    input->bindAction("backward",              EKeyMode::KEY_DOWN,     "game01", this, "backward");
    input->bindAction("exitGame01",            EKeyMode::KEY_PRESSED,  "game01", this, "leave");
    input->bindAction("sprint",                EKeyMode::KEY_PRESSED,  "game01", this, "sprintStart");
    input->bindAction("sprint",                EKeyMode::KEY_RELEASED, "game01", this, "sprintEnd");
    // input->bindAction("growUpCollider",        EKeyMode::KEY_DOWN,     "game01", this, "growUpSphereCollider");
    // input->bindAction("growDownCollider",      EKeyMode::KEY_DOWN,     "game01", this, "growDownSphereCollider");
    input->bindAction("swapInputModeToGame01", EKeyMode::KEY_PRESSED,  "game02", this, "swapInputModeToGame01");
    input->bindAction("swapInputModeToGame02", EKeyMode::KEY_PRESSED,  "game01", this, "swapInputModeToGame02");

    GPE::Wave testSound3("./resources/sounds/E_Western.wav", "Western");

    GPE::SourceSettings sourceSettings;
    sourceSettings.pitch = 1.f;
    sourceSettings.loop  = AL_TRUE;

    source->setSound("Western", "Western", sourceSettings);
    source->playSound("Western");

    controller->setHasGravity(true);
    controller->setSpeed(1.f);
    controller->setMouseSpeed(0.0025f);
    controller->setGravity(0.1f);
}


MyFpsScript::~MyFpsScript() noexcept                
{
    enableFixedUpdate(false);
    enableUpdate(false);
}

/* Variable setter serialization example
void MyFpsScript::setPrintHello(bool p)
{
    if (printHello != p) // Called everytime if no if 
    {
        printHello = p;

        if (printHello)
        {
            GPE::Log::getInstance()->log("Hello world!");
        }
        else
        {
            GPE::Log::getInstance()->log("Set me to true!");
        }
    }
}
*/

void MyFpsScript::rotate(const GPM::Vec2& deltaDisplacement)
{
    using namespace GPM;

    const float speed = controller->getMouseSpeed();

    const Quat& orientation{getOwner().getTransform().getSpacialAttribut().rotation};
    const Vec2  axis       {deltaDisplacement.rotated90()};
    const Quat  rotX       {Quat::angleAxis(axis.x * speed, Vec3::right())};
    const Quat  rotY       {Quat::angleAxis(axis.y * speed, Vec3::up())};
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
    vec.y         = 0;
    controller->move(-vec);
    // rigidbody.rigidbody->addForce(vec * -speed, physx::PxForceMode::eFORCE);
}


void MyFpsScript::backward()
{
    GPM::Vec3 vec = getOwner().getTransform().getVectorForward();
    vec.y         = 0;
    controller->move(vec);
    // rigidbody.rigidbody->addForce(vec * speed, physx::PxForceMode::eFORCE);
}


void MyFpsScript::left()
{
    GPM::Vec3 vec = getOwner().getTransform().getVectorRight();
    vec.y         = 0;
    controller->move(-vec);
    // rigidbody.rigidbody->addForce(vec * -speed, physx::PxForceMode::eFORCE);
}


void MyFpsScript::right()
{
    GPM::Vec3 vec = getOwner().getTransform().getVectorRight();
    vec.y         = 0;
    controller->move(vec);
    // rigidbody.rigidbody->addForce(vec * speed, physx::PxForceMode::eFORCE);
}


void MyFpsScript::leave()
{
    // exit(666);
}


void MyFpsScript::sprintStart()
{
    controller->setSpeed(controller->getSpeed() * 2.f);
}


void MyFpsScript::sprintEnd()
{
    controller->setSpeed(controller->getSpeed() * .5f);
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
    ImGui::Button("Top/Left", size);

    ImGui::SetNextElementLayout(0.5, 0.f, size, ImGui::EHAlign::Middle, ImGui::EVAlign::Top);
    ImGui::Button("Top", size);

    ImGui::SetNextElementLayout(1.f, 0.f, size, ImGui::EHAlign::Right, ImGui::EVAlign::Top);
    ImGui::Button("Top/Right", size);

    ImGui::SetNextElementLayout(0.f, 0.5f, size, ImGui::EHAlign::Left);
    ImGui::Button("Mid/Left", size);

    ImGui::SetNextElementLayout(0.5f, 0.5f, size);
    ImGui::Button("Mid", size);

    ImGui::SetNextElementLayout(1.f, 0.5f, size, ImGui::EHAlign::Right);
    ImGui::Button("Mid/Right", size);

    ImGui::SetNextElementLayout(0.f, 1.f, size, ImGui::EHAlign::Left, ImGui::EVAlign::Bottom);
    ImGui::Button("Bot/Left", size);

    ImGui::SetNextElementLayout(0.5f, 1.f, size, ImGui::EHAlign::Middle, ImGui::EVAlign::Bottom);
    ImGui::Button("Bot", size);

    ImGui::SetNextElementLayout(1.f, 1.f, size, ImGui::EHAlign::Right, ImGui::EVAlign::Bottom);
    ImGui::Button("Bot/Right", size);
}


void MyFpsScript::swapInputModeToGame01()
{
    GPE::InputManager& iManager = GPE::Engine::getInstance()->inputManager;

    iManager.setInputMode("game01");
    iManager.setCursorTrackingState(true);
    iManager.setCursorLockState(true);
}


void MyFpsScript::swapInputModeToGame02()
{
    GPE::InputManager& iManager = GPE::Engine::getInstance()->inputManager;

    iManager.setInputMode("game02");
    iManager.setCursorTrackingState(false);
    iManager.setCursorLockState(false);
}


void MyFpsScript::fixedUpdate(float deltaTime)
{
    if (GPE::Engine::getInstance()->inputManager.getInputMode() == "game01")
    {
        const GPM::Vec2 deltaPos = GPE::Engine::getInstance()->inputManager.getCursor().deltaPos;

        if (deltaPos.x || deltaPos.y)
            rotate(deltaPos);
    }
    controller->update(deltaTime);
}

} // End of namespace GPG