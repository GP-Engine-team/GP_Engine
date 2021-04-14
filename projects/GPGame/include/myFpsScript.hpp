/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/AudioComponent.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/InputComponent.hpp>
#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>
#include <Engine/ECS/System/InputManagerGLFW.hpp>
#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>

#include <iostream>

// Generated
#include "Generated/myFpsScript.rfk.h"

namespace GPG RFKNamespace()
{
class RFKClass(Inspect(), Serialize()) MyFpsScript : public GPE::BehaviourComponent
    {
public:
    inline MyFpsScript(GPE::GameObject& owner) noexcept
        : GPE::BehaviourComponent(owner), input(&owner.addComponent<GPE::InputComponent>()),
          source(&owner.addComponent<GPE::AudioComponent>()), controller(&owner.addComponent<GPE::CharacterController>())
    {
        enableFixedUpdate(true);
        input->bindAction("jump", EKeyMode::KEY_PRESSED, this, "jump");
        input->bindAction("right", EKeyMode::KEY_DOWN, this, "right");
        input->bindAction("left", EKeyMode::KEY_DOWN, this, "left");
        input->bindAction("forward", EKeyMode::KEY_DOWN, this, "forward");
        input->bindAction("back", EKeyMode::KEY_DOWN, this, "back");
        input->bindAction("exit", EKeyMode::KEY_PRESSED, this, "leave");
        input->bindAction("sprintStart", EKeyMode::KEY_PRESSED, this, "sprintStart");
        input->bindAction("sprintEnd", EKeyMode::KEY_RELEASED, this, "sprintEnd");
        input->bindAction("growUpCollider", EKeyMode::KEY_DOWN, this, "growUpSphereCollider");
        input->bindAction("growDownCollider", EKeyMode::KEY_DOWN, this, "growDownSphereCollider");

        GPE::Wave testSound("./resources/sounds/RickRoll.wav", "RICKROLL");
        GPE::Wave testSound2("./resources/sounds/YMCA.wav", "YMCA");
        GPE::Wave testSound3("./resources/sounds/E_Western.wav", "Western");

        GPE::SourceSettings sourceSettings;
        sourceSettings.pitch = 1;
        sourceSettings.loop  = AL_TRUE;

        source->setSound("Western", "Western", sourceSettings);
        source->playSound("Western");

        controller->setHasGravity(true);
        controller->setSpeed(0.3);
    }

    MyFpsScript() noexcept                         
    {
        enableFixedUpdate(true);
    }
    MyFpsScript(const MyFpsScript& other) noexcept = delete;
    MyFpsScript(MyFpsScript&& other) noexcept      = default;
    virtual ~MyFpsScript() noexcept                = default;

    MyFpsScript& operator=(MyFpsScript const& other) noexcept = delete;
    MyFpsScript& operator=(MyFpsScript&& other) noexcept = delete;

    RFKField(Serialize())
    GPE::InputComponent*      input = nullptr;
    RFKField(Serialize())
    GPE::AudioComponent*      source = nullptr;
    RFKField(Serialize())
    GPE::CharacterController* controller = nullptr;

    void rotate(const GPM::Vec2& deltaDisplacement)
    {
        if (deltaDisplacement.length() > 0.4)
        {
            getOwner().getTransform().setRotation(getOwner().getTransform().getSpacialAttribut().rotation *
                                                  GPM::Quaternion::angleAxis(deltaDisplacement.y * 0.001f, {1, 0, 0}));
            getOwner().getTransform().setRotation(GPM::Quaternion::angleAxis(-deltaDisplacement.x * 0.001f, {0, 1, 0}) *
                                                  getOwner().getTransform().getSpacialAttribut().rotation);
        }
    }

    RFKMethod()
    inline void jump()
    {
        /*getOwner().getComponent<GPE::RigidbodyDynamic>()->rigidbody->addForce(physx::PxVec3{0, 1, 0} * speed,
                                                                              physx::PxForceMode::eFORCE);*/
        GPM::Vec3 vec = getOwner().getTransform().getVectorUp();
        controller->move(vec, 100);
        // controller.controller->getActor()->addForce(physx::PxVec3{ 0, 1, 0 } *10000,physx::PxForceMode::eFORCE);
    }

    RFKMethod()
    inline void forward()
    {
        GPM::Vec3 vec = getOwner().getTransform().getVectorForward();
        vec.y         = 0;
        controller->move(-vec);
        // rigidbody.rigidbody->addForce(vec * -speed, physx::PxForceMode::eFORCE);
    }

    RFKMethod()
    inline void back()
    {
        GPM::Vec3 vec = getOwner().getTransform().getVectorForward();
        vec.y         = 0;
        controller->move(vec);
        // rigidbody.rigidbody->addForce(vec * speed, physx::PxForceMode::eFORCE);
    }

    RFKMethod()
    inline void left()
    {
        GPM::Vec3 vec = getOwner().getTransform().getVectorRight();
        vec.y         = 0;
        controller->move(-vec);
        // rigidbody.rigidbody->addForce(vec * -speed, physx::PxForceMode::eFORCE);
    }

    RFKMethod()
    inline void right()
    {
        GPM::Vec3 vec = getOwner().getTransform().getVectorRight();
        vec.y         = 0;
        controller->move(vec);
        // rigidbody.rigidbody->addForce(vec * speed, physx::PxForceMode::eFORCE);
    }

    RFKMethod()
    inline void leave()
    {
        exit(666);
    }

    RFKMethod()
    inline void sprintStart()
    {
        controller->setSpeed(controller->getSpeed() * 2.f);
    }

    RFKMethod()
    inline void sprintEnd()
    {
        controller->setSpeed(controller->getSpeed() / 2.f);
    }

    RFKMethod()
    inline void growUpSphereCollider()
    {
        // collider.setRadius(collider.getRadius() + 1);
    }

    RFKMethod()
    inline void growDownSphereCollider()
    {
        // collider.setRadius(collider.getRadius() - 1);
    }

    void fixedUpdate(float deltaTime) final
    {
        rotate(GPE::Engine::getInstance()->inputManager.getCursor().deltaPos);
        controller->update(deltaTime);
    }

    MyFpsScript_GENERATED
};
} /*namespace GPG*/
