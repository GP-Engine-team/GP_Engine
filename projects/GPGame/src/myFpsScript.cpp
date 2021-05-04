#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/Core/Tools/Raycast.hpp>
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

    MyFpsScript::MyFpsScript(GPE::GameObject & owner) noexcept
        : GPE::BehaviourComponent(owner), input{&owner.addComponent<GPE::InputComponent>()},
          source{&owner.addComponent<GPE::AudioComponent>()},
          controller{&owner.addComponent<GPE::CharacterController>()}
    {
        enableFixedUpdate(true);
        enableUpdate(true);
        enableOnGUI(true);

        input->bindAction("forward", EKeyMode::KEY_DOWN, "Game", this, "forward");
        input->bindAction("backward", EKeyMode::KEY_DOWN, "Game", this, "backward");
        input->bindAction("left", EKeyMode::KEY_DOWN, "Game", this, "left");
        input->bindAction("right", EKeyMode::KEY_DOWN, "Game", this, "right");
        input->bindAction("jump", EKeyMode::KEY_DOWN, "Game", this, "jump");
        input->bindAction("exit", EKeyMode::KEY_PRESSED, "Game", this, "leave");
        input->bindAction("sprintStart", EKeyMode::KEY_PRESSED, "Game", this, "sprintStart");
        input->bindAction("sprintEnd", EKeyMode::KEY_RELEASED, "Game", this, "sprintEnd");
        input->bindAction("RaycastExample", EKeyMode::KEY_PRESSED, "Game", this, "RaycastExample");
        // input->bindAction("growUpCollider",        EKeyMode::KEY_DOWN,     "Game", this, "growUpSphereCollider");
        // input->bindAction("growDownCollider",      EKeyMode::KEY_DOWN,     "Game", this, "growDownSphereCollider");

        GPE::Wave testSound3("./resources/sounds/E_Western.wav", "Western");

        GPE::SourceSettings sourceSettings;
        sourceSettings.pitch = 1.f;
        sourceSettings.loop  = AL_TRUE;

        source->setSound("Western", "Western", sourceSettings);
        source->playSound("Western");

        controller->setHasGravity(true);
        controller->setSpeed(1.f);
        controller->setMouseSpeed(.0025f);
        controller->setGravity(.1f);
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

        const Quat& orientation{getOwner().getTransform().getSpacialAttribut().rotation};
        const Vec2  axis{deltaDisplacement.rotated90()};
        const Quat  rotX{Quat::angleAxis(axis.x * controller->getMouseSpeed(), Vec3::right())};
        const Quat  rotY{Quat::angleAxis(axis.y * controller->getMouseSpeed(), Vec3::up())};
        const Quat  newRot{rotY * orientation * rotX};

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
        controller->move(vec);
        // rigidbody.rigidbody->addForce(vec * -speed, physx::PxForceMode::eFORCE);
    }

    void MyFpsScript::backward()
    {
        GPM::Vec3 vec = getOwner().getTransform().getVectorForward();
        vec.y         = 0;
        controller->move(-vec);
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
        glfwWindowShouldClose(GPE::Engine::getInstance()->window.getGLFWWindow());
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
    void MyFpsScript::RaycastExample()
    {
        GPE::Raycast ray;
        ray.Fire(getOwner().getTransform().getGlobalPosition(), getOwner().getTransform().getVectorForward(), 100000);
        if (ray.hit.hasBlock)
        {
            GPE::GameObject* owner = static_cast<GPE::GameObject*>(ray.hit.block.actor->userData);
            if (owner)
            {
                std::cout << "yolo" << std::endl;
                // Do some shit here;
            }
        }
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

    void MyFpsScript::fixedUpdate(double deltaTime)
    {
        // TODO: find a fix to relieve the user from having to check this, or leave it like that?
        if (GPE::Engine::getInstance()->inputManager.getInputMode() == "Game")
        {
            const GPM::Vec2 deltaPos = GPE::Engine::getInstance()->inputManager.getCursor().deltaPos;

            if (deltaPos.x || deltaPos.y)
                rotate(deltaPos);
        }
        controller->update(deltaTime);
    }

} // End of namespace GPG