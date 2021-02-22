/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/Component.hpp"
#include "Engine/Intermediate/BehaviourSystem.hpp"
#include "Engine/Intermediate/GameObject.hpp"

namespace GPE
{
class BehaviourComponent : public Component
{
    private:

    protected:

    bool m_isUpdated = false;
    bool m_isFixedUpdated = false;

    public:

    inline BehaviourComponent(GameObject& owner) noexcept;

    BehaviourComponent() noexcept                             = delete;
    BehaviourComponent(const BehaviourComponent& other) noexcept = delete;
    BehaviourComponent(BehaviourComponent&& other) noexcept;
    virtual ~BehaviourComponent() noexcept;
    BehaviourComponent& operator=(BehaviourComponent const& other) noexcept = delete;
    BehaviourComponent& operator=(BehaviourComponent&& other) noexcept = delete;

    virtual void awake()
    {}

    virtual void start()
    {}

    virtual void fixedUpdate(float deltaTime)
    {}

    virtual void update(float deltaTime)
    {}

    void enableUpdate(bool flag) noexcept;

    void enableFixedUpdate(bool flag) noexcept;
};

#include "BehaviourComponent.inl"

} /*namespace GPE*/
