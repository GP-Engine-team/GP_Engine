/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <GPM/Vector3.hpp>
#include <Generated/FreeFly.rfk.h>

namespace GPE RFKNamespace()
{

class RFKClass() FreeFly final : public BehaviourComponent
{
protected:
    TimeSystem& timeSys;
    float       m_walkSpeed     = 15.f; // m/s
    float       m_sprintSpeed   = 30.f;
    float       m_speed         = m_walkSpeed;
    float       m_rotationSpeed = .001f;

public:
    FreeFly() noexcept = default;
    FreeFly(GameObject& owner) noexcept;
    FreeFly(FreeFly&& other) noexcept = default;
    ~FreeFly() noexcept;

    RFKMethod()
    void rotate(const GPM::Vector2& deltaDisplacement);

    RFKMethod()
    inline void up();
    
    RFKMethod()
    inline void down();

    RFKMethod()
    inline void forward();

    RFKMethod()
    inline void backward();

    RFKMethod()
    inline void left();

    RFKMethod()
    inline void right();

    RFKMethod()
    inline void walk();

    RFKMethod()
    inline void sprint();

    void update(double deltaTime) final;

    FreeFly& operator=(FreeFly&& other) noexcept;

    FreeFly_GENERATED
};

#include "FreeFly.inl"

} // namespace GPE