﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Core/Physics/Collisions/SphereCollider.hpp>
#include <Engine/ECS/Component/AudioComponent.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/InputComponent.hpp>
#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyDynamic.hpp>
#include <Engine/ECS/System/InputManagerGLFW.hpp>
#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>
#include <PxForceMode.h>

namespace GPG
{
class MyFreeFlyWithCollisionScript : public GPE::BehaviourComponent
{
public:
    inline MyFreeFlyWithCollisionScript(GPE::GameObject& owner) noexcept
        : GPE::BehaviourComponent(owner), input(owner.addComponent<GPE::InputComponent>()),
          source(owner.addComponent<GPE::AudioComponent>())
    {
        enableUpdate(true);
        input.bindAction("jump", EKeyMode::KEY_DOWN, "game01", this, "up");
        input.bindAction("down", EKeyMode::KEY_DOWN, "game01", this, "down");
        input.bindAction("right", EKeyMode::KEY_DOWN, "game01", this, "right");
        input.bindAction("left", EKeyMode::KEY_DOWN, "game01", this, "left");
        input.bindAction("forward", EKeyMode::KEY_DOWN, "game01", this, "forward");
        input.bindAction("back", EKeyMode::KEY_DOWN, "game01", this, "back");
        input.bindAction("exit", EKeyMode::KEY_PRESSED, "game01", this, "leave");
        input.bindAction("sprintStart", EKeyMode::KEY_PRESSED, "game01", this, "sprintStart");
        input.bindAction("sprintEnd", EKeyMode::KEY_RELEASED, "game01", this, "sprintEnd");
        input.bindAction("growUpCollider", EKeyMode::KEY_DOWN, "game01", this, "growUpSphereCollider");
        input.bindAction("growDownCollider", EKeyMode::KEY_DOWN, "game01", this, "growDownSphereCollider");

        GPE::Wave testSound("./resources/sounds/RickRoll.wav", "RICKROLL");
        GPE::Wave testSound2("./resources/sounds/YMCA.wav", "YMCA");
        GPE::Wave testSound3("./resources/sounds/E_Western.wav", "Western");

        GPE::SourceSettings sourceSettings;
        sourceSettings.pitch = 1;
        sourceSettings.loop  = AL_TRUE;

        source.setSound("Western", "Western", sourceSettings);
        source.playSound("Western", true);
    }

    MyFreeFlyWithCollisionScript() noexcept                                          = delete;
    MyFreeFlyWithCollisionScript(const MyFreeFlyWithCollisionScript& other) noexcept = delete;
    MyFreeFlyWithCollisionScript(MyFreeFlyWithCollisionScript&& other) noexcept      = default;
    virtual ~MyFreeFlyWithCollisionScript() noexcept                                 = default;
    MyFreeFlyWithCollisionScript& operator=(MyFreeFlyWithCollisionScript const& other) noexcept = delete;
    MyFreeFlyWithCollisionScript& operator=(MyFreeFlyWithCollisionScript&& other) noexcept = delete;

    GPE::InputComponent& input;
    GPE::AudioComponent& source;
    float                speed = 100;

    void rotate(const GPM::Vec2& deltaDisplacement)
    {
        if (deltaDisplacement.length() > 0.4)
        {
            getOwner().getTransform().setRotation(getOwner().getTransform().getSpacialAttribut().rotation *
                                                  GPM::Quaternion::angleAxis(deltaDisplacement.y * 0.001f, {1, 0, 0}));
            getOwner().getTransform().setRotation(GPM::Quaternion::angleAxis(deltaDisplacement.x * 0.001f, {0, 1, 0}) *
                                                  getOwner().getTransform().getSpacialAttribut().rotation);
        }
    }

    RFKMethod() inline void up()
    {
        getOwner().getComponent<GPE::RigidbodyDynamic>()->rigidbody->addForce(
            GPE::PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getVectorUp()) * speed,
            physx::PxForceMode::eFORCE);
    }

    RFKMethod() inline void down()
    {
        getOwner().getComponent<GPE::RigidbodyDynamic>()->rigidbody->addForce(
            GPE::PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getVectorUp()) * -speed,
            physx::PxForceMode::eFORCE);
    }

    RFKMethod() inline void forward()
    {
        getOwner().getComponent<GPE::RigidbodyDynamic>()->rigidbody->addForce(
            GPE::PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getVectorForward()) * speed,
            physx::PxForceMode::eFORCE);
    }

    RFKMethod() inline void back()
    {
        getOwner().getComponent<GPE::RigidbodyDynamic>()->rigidbody->addForce(
            GPE::PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getVectorForward()) * -speed,
            physx::PxForceMode::eFORCE);
    }

    RFKMethod() inline void left()
    {
        getOwner().getComponent<GPE::RigidbodyDynamic>()->rigidbody->addForce(
            GPE::PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getVectorRight()) * -speed,
            physx::PxForceMode::eFORCE);
    }

    RFKMethod() inline void right()
    {
        getOwner().getComponent<GPE::RigidbodyDynamic>()->rigidbody->addForce(
            GPE::PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getVectorRight()) * speed,
            physx::PxForceMode::eFORCE);
    }

    RFKMethod() inline void leave()
    {
        exit(666);
    }

    RFKMethod() inline void sprintStart()
    {
        speed *= 2.f;
    }

    RFKMethod() inline void sprintEnd()
    {
        speed /= 2.f;
    }

    RFKMethod() inline void growUpSphereCollider()
    {
        GPE::SphereCollider* collider = getOwner().getComponent<GPE::SphereCollider>();
        if (collider)
        {
            collider->setRadius(collider->getRadius() + 1);
        }
    }

    RFKMethod() inline void growDownSphereCollider()
    {
        GPE::SphereCollider* collider = getOwner().getComponent<GPE::SphereCollider>();
        if (collider)
        {
            collider->setRadius(collider->getRadius() - 1);
        }
    }

    void update(float deltaTime)
    {
        rotate(GPE::Engine::getInstance()->inputManager.getCursor().deltaPos);
    }
};
} /*namespace GPG*/
