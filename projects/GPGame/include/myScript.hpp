﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/BehaviourComponent.hpp"
#include "Engine/ECS/Component/InputComponent.hpp"
#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Serialization/DataInspector.hpp"
#include "Engine/Serialization/Inspect.hpp"
#include "Engine/Serialization/Serialize.hpp"
#include "Engine/Serialization/Slider.hpp"

#include "Generated/myScript.rfk.h"

namespace GPG RFKNamespace()
{

    class RFKClass(Inspect(), Serialize()) MyScript : public GPE::BehaviourComponent
    {
    public:
        inline MyScript(GPE::GameObject & owner) noexcept
            : GPE::BehaviourComponent(owner), input(&owner.addComponent<GPE::InputComponent>())
        {

            enableUpdate(true);
            input->bindAction("jump", EKeyMode::KEY_DOWN, "game01", this, "up");
            input->bindAction("down", EKeyMode::KEY_DOWN, "game01", this, "down");
            input->bindAction("right", EKeyMode::KEY_DOWN, "game01", this, "right");
            input->bindAction("left", EKeyMode::KEY_DOWN, "game01", this, "left");
            input->bindAction("forward", EKeyMode::KEY_DOWN, "game01", this, "forward");
            input->bindAction("back", EKeyMode::KEY_DOWN, "game01", this, "back");
            input->bindAction("exit", EKeyMode::KEY_DOWN, "game01", this, "leave");
            input->bindAction("sprint", EKeyMode::KEY_DOWN, "game01", this, "sprint");
        }

        MyScript() noexcept
        {
            enableUpdate(true);
            // TODO : Bind inputs later
        }
        MyScript(const MyScript& other) noexcept = delete;
        MyScript(MyScript && other) noexcept     = default;
        virtual ~MyScript() noexcept             = default;
        MyScript& operator=(MyScript const& other) noexcept = delete;
        MyScript& operator=(MyScript&& other) noexcept = delete;

        RFKField(Serialize()) GPE::InputComponent* input = nullptr;

        RFKField(Serialize(), Inspect(), Slider(0, 10)) float sprintSpeed = 2;

        RFKField(Serialize(), Inspect(), Slider(0, 10)) float defaultSpeed = 1;

        RFKField(Serialize()) float speed = defaultSpeed;

        void rotate(const GPM::Vec2& deltaDisplacement)
        {
            if (deltaDisplacement.sqrLength() > .16f)
            {
                getOwner().getTransform().setRotation(
                    getOwner().getTransform().getSpacialAttribut().rotation *
                    GPM::Quaternion::angleAxis(deltaDisplacement.y * .001f, {1.f, .0f, .0f}));
                getOwner().getTransform().setRotation(
                    GPM::Quaternion::angleAxis(deltaDisplacement.x * .001f, {.0f, 1.f, .0f}) *
                    getOwner().getTransform().getSpacialAttribut().rotation);
            }
        }

        RFKMethod() inline void up()
        {
            getOwner().getTransform().translate(getOwner().getTransform().getVectorUp() * speed);
        }

        RFKMethod() inline void down()
        {
            getOwner().getTransform().translate(getOwner().getTransform().getVectorUp() * -speed);
        }

        RFKMethod() inline void forward()
        {
            getOwner().getTransform().translate(getOwner().getTransform().getVectorForward() * speed);
        }

        RFKMethod() inline void back()
        {
            getOwner().getTransform().translate(getOwner().getTransform().getVectorForward() * -speed);
        }

        RFKMethod() inline void left()
        {
            getOwner().getTransform().translate(getOwner().getTransform().getVectorRight() * -speed);
        }

        RFKMethod() inline void right()
        {
            getOwner().getTransform().translate(getOwner().getTransform().getVectorRight() * speed);
        }

        RFKMethod() inline void leave()
        {
            exit(666);
        }

        RFKMethod() inline void sprint()
        {
            speed = sprintSpeed;
        }

        void update(double deltaTime) final
        {
            speed = defaultSpeed;

            if (GPE::Engine::getInstance()->inputManager.getCursor().deltaPos.sqrLength() > .00001f)
                rotate(GPE::Engine::getInstance()->inputManager.getCursor().deltaPos);
        }

        MyScript_GENERATED
    };
} // namespace )
