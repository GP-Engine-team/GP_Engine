/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>
#include <unordered_set>

namespace GPE
{
class AnimationComponent;

class AnimationSystem
{
protected:
    std::vector<AnimationComponent*> m_pAnimComponents;

public:

    //void fixedUpdate(double deltaTime) noexcept;

    void update(double deltaTime) const noexcept;

    void addBehaviour(AnimationComponent& behaviour) noexcept;

    void removeBehaviour(AnimationComponent& behaviour) noexcept;
};

} /*namespace GPE*/
