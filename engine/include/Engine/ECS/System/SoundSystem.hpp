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
protected:
    SoundSystem() = default;

    static SoundSystem* m_soundSystem;

public:
    /**
     * Singletons should not be cloneable.
     */
    SoundSystem(SoundSystem& other) = delete;

    /**
     * Singletons should not be assignable.
     */
    void operator=(const SoundSystem&) = delete;

    /**
     * @brief get a pointeur to the InputManager singleton
     * @return
     */
    static SoundSystem* getInstance();

    /**
     * @brief add input component to the component list
     * @param input
     * @return
     */
    inline int addComponent(AudioComponent* input) noexcept;

    /**
     * @brief remove input component to the component list
     * @param key
     * @return
     */
    inline void removeComponent(int key) noexcept;

private:
    std::unordered_map<int, AudioComponent*> m_audioComponent;
};

#include "Engine/ECS/System/SoundSystem.inl"
} // namespace GPE
