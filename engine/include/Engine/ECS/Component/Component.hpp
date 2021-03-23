/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Serialization/Inspect.hpp"
#include "Engine/Serialization/DataInspector.hpp"
#include "Refureku/Object.h"
#include "Generated/Component.rfk.h"

namespace GPE RFKNamespace()
{
class GameObject;

class RFKClass() Component : public rfk::Object
{
protected:
    GameObject& m_gameObject;
    RFKField(Inspect())
    bool        m_isActivated{true};

public:
    inline Component(GameObject& owner) noexcept;
    inline Component() noexcept                       = delete;
    inline Component(const Component& other) noexcept = delete;
    inline Component(Component&& other) noexcept      = default;
    inline virtual ~Component() noexcept              = default;
    inline Component& operator=(const Component& other) noexcept = delete;
    inline Component& operator=(Component&& other) noexcept = default;

    [[nodiscard]] constexpr inline GameObject& getOwner() noexcept;

    [[nodiscard]] constexpr inline const GameObject& getOwner() const noexcept;

    [[nodiscard]] constexpr inline bool isActivated() const noexcept;

    constexpr inline void setActive(bool newState) noexcept;

    virtual bool inspect();

    virtual void moveTowardScene(class Scene& newOwner){};
    
    Component_GENERATED
};

#include "Component.inl"

} // namespace GPE

File_GENERATED