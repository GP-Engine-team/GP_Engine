/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/BehaviourComponent.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "GPM/Vector3.hpp"
#include "Generated/FreeFly.rfk.h"

namespace GPE RFKNamespace()
{

class RFKClass() FreeFly final : public BehaviourComponent
{
protected:
    float m_speed         = 1.f;
    float m_rotationSpeed = .001f;

public:
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
    inline void back();

    RFKMethod()
    inline void left();

    RFKMethod()
    inline void right();

    RFKMethod()
    inline void sprint();

    void update(float deltaTime) final;

    FreeFly& operator=(FreeFly&& other) noexcept;

    FreeFly_GENERATED
};

#include "FreeFly.inl"

} // namespace GPE