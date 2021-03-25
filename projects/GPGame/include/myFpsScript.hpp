/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/AudioComponent.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/InputComponent.hpp>
#include <Engine/ECS/Component/Physics/Collisions/SphereCollider.hpp>
#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyDynamic.hpp>
#include <Engine/ECS/System/InputManagerGLFW.hpp>
#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>
#include <PxForceMode.h>

#include <iostream>

namespace GPG
{
class MyFpsScript : public GPE::BehaviourComponent
{
public:
    inline MyFpsScript(GPE::GameObject& owner) noexcept
        : GPE::BehaviourComponent(owner), input(owner.addComponent<GPE::InputComponent>()),
          source(owner.addComponent<GPE::AudioComponent>()), collider(owner.addComponent<GPE::SphereCollider>()),
          rigidbody(owner.addComponent<GPE::RigidbodyDynamic>())
    {
        enableUpdate(true);
        input.bindAction("jump", EKeyMode::KEY_DOWN, this, &MyFpsScript::jump);
        input.bindAction("right", EKeyMode::KEY_DOWN, this, &MyFpsScript::right);
        input.bindAction("left", EKeyMode::KEY_DOWN, this, &MyFpsScript::left);
        input.bindAction("forward", EKeyMode::KEY_DOWN, this, &MyFpsScript::forward);
        input.bindAction("back", EKeyMode::KEY_DOWN, this, &MyFpsScript::back);
        input.bindAction("exit", EKeyMode::KEY_PRESSED, this, &MyFpsScript::leave);
        input.bindAction("sprintStart", EKeyMode::KEY_PRESSED, this, &MyFpsScript::sprintStart);
        input.bindAction("sprintEnd", EKeyMode::KEY_RELEASED, this, &MyFpsScript::sprintEnd);
        input.bindAction("growUpCollider", EKeyMode::KEY_DOWN, this, &MyFpsScript::growUpSphereCollider);
        input.bindAction("growDownCollider", EKeyMode::KEY_DOWN, this, &MyFpsScript::growDownSphereCollider);

        GPE::Wave testSound("./resources/sounds/RickRoll.wav", "RICKROLL");
        GPE::Wave testSound2("./resources/sounds/YMCA.wav", "YMCA");
        GPE::Wave testSound3("./resources/sounds/E_Western.wav", "Western");

        GPE::SourceSettings sourceSettings;
        sourceSettings.pitch = 1;
        sourceSettings.loop  = AL_TRUE;

        source.setSound("Western", "Western", sourceSettings);
        source.playSound("Western");

        collider.isVisible = true;
        collider.setRadius(10.f);
        rigidbody.collider = &collider;
        rigidbody.rigidbody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
        rigidbody.rigidbody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
        rigidbody.rigidbody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
    }

    MyFpsScript() noexcept                         = delete;
    MyFpsScript(const MyFpsScript& other) noexcept = delete;
    MyFpsScript(MyFpsScript&& other) noexcept      = default;
    virtual ~MyFpsScript() noexcept                = default;
    MyFpsScript& operator=(MyFpsScript const& other) noexcept = delete;
    MyFpsScript& operator=(MyFpsScript&& other) noexcept = delete;

    GPE::InputComponent&   input;
    GPE::AudioComponent&   source;
    GPE::SphereCollider&   collider;
    GPE::RigidbodyDynamic& rigidbody;
    float                  speed = 100;

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

    inline void jump()
    {
        getOwner().getComponent<GPE::RigidbodyDynamic>()->rigidbody->addForce(physx::PxVec3{0, 1, 0} * speed,
                                                                              physx::PxForceMode::eFORCE);
    }

    inline void forward()
    {
        physx::PxVec3 vec = GPE::PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getVectorForward());
        vec.y             = 0;
        rigidbody.rigidbody->addForce(vec * -speed, physx::PxForceMode::eFORCE);
    }

    inline void back()
    {
        physx::PxVec3 vec = GPE::PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getVectorForward());
        vec.y             = 0;
        rigidbody.rigidbody->addForce(vec * speed, physx::PxForceMode::eFORCE);
    }

    inline void left()
    {
        physx::PxVec3 vec = GPE::PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getVectorRight());
        vec.y             = 0;
        rigidbody.rigidbody->addForce(vec * -speed, physx::PxForceMode::eFORCE);
    }

    inline void right()
    {
        physx::PxVec3 vec = GPE::PhysXSystem::GPMVec3ToPxVec3(getOwner().getTransform().getVectorRight());
        vec.y             = 0;
        rigidbody.rigidbody->addForce(vec * speed, physx::PxForceMode::eFORCE);
    }

    inline void leave()
    {
        exit(666);
    }

    inline void sprintStart()
    {
        speed *= 2.f;
    }

    inline void sprintEnd()
    {
        speed /= 2.f;
    }

    inline void growUpSphereCollider()
    {
        collider.setRadius(collider.getRadius() + 1);
    }

    inline void growDownSphereCollider()
    {
        collider.setRadius(collider.getRadius() - 1);
    }

    void update(float deltaTime) final
    {
        rotate(GPE::Engine::getInstance()->inputManager.getCursor().deltaPos);
    }
};
} /*namespace GPG*/
