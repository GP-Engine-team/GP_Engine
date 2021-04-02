/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once
#include "Engine/ECS/Component/AudioComponent.hpp"
#include <unordered_map>
namespace GPE
{
class SoundSystem
{
public:
    SoundSystem()  = default;
    ~SoundSystem() = default;

    std::unordered_map<int, AudioComponent*> m_audioComponents;

    /**
     * Singletons should not be cloneable.
     */
    SoundSystem(SoundSystem& other) = delete;

    /**
     * Singletons should not be assignable.
     */
    void operator=(const SoundSystem&) = delete;

    /**
     * @brief add input component to the component list
     * @param input
     * @return
     */
    inline int addComponent(AudioComponent* input) noexcept;

    inline void updateComponent(AudioComponent* newPointorBehaviour) noexcept;

    /**
     * @brief remove input component to the component list
     * @param key
     * @return
     */
    inline void removeComponent(int key) noexcept;
};

#include "Engine/ECS/System/SoundSystem.inl"
} // namespace GPE
