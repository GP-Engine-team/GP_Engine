/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/TransformComponent.hpp>
#include <foundation/PxErrorCallback.h>
#include <vector>

namespace GPE
{
class AnimationSystem
{
private:
    std::vector<class AnimationComponent*> components;

public:
    /**
     * @brief Update the animation of used components
     * @param deltaTime The time elapsed during the last frame
    */
    void update(double deltaTime) noexcept;

    /**
     * @brief add the AnimationComponent to the component list
     * @param animationComponent The component you want to enable
     */
    void addComponent(class AnimationComponent* animationComponent) noexcept;

    /**
     * @brief remove the AnimationComponent to the component list
     * @param animationComponent The component you want to disable
     */
    void removeComponent(class AnimationComponent* animationComponent) noexcept;
};

} // namespace GPE
