/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/BehaviourComponent.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "GPM/Vector3.hpp"

namespace GPE
{

class FreeFly final : public BehaviourComponent
{
protected:
    float m_speed         = 1.f;
    float m_rotationSpeed = .001f;

public:
    FreeFly(GameObject& owner) noexcept;
    FreeFly(FreeFly&& other) noexcept = default;
    ~FreeFly() noexcept;

    void rotate(const GPM::Vector2& deltaDisplacement);

    inline void up();
    inline void down();
    inline void forward();
    inline void backward();
    inline void left();
    inline void right();
    inline void sprint();

    void update(float deltaTime) final;

    FreeFly& operator=(FreeFly&& other) noexcept;
};

#include "FreeFly.inl"

} // namespace GPE
