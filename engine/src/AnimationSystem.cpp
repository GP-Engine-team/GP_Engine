/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#include <Engine/ECS/System/AnimationSystem.hpp>
#include <Engine/ECS/Component/AnimationComponent.hpp>

using namespace GPE;

void AnimationSystem::update(double deltaTime) noexcept
{
    for (AnimationComponent* comp : components)
    {
        comp->update(float(deltaTime));
    }
}

void AnimationSystem::addComponent(class AnimationComponent* animationComponent) noexcept
{
    components.push_back(animationComponent);
}
void AnimationSystem::removeComponent(class AnimationComponent* animationComponent) noexcept
{
    auto it = std::find(components.begin(), components.end(), animationComponent);
    if (it != components.end())
    {
        std::swap(components.back(), *it);
        components.pop_back();
    }
}