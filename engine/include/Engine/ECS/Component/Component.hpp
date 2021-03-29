/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Serialization/ComponentGen.h"
#include "Engine/Serialization/DataInspector.hpp"
#include "Engine/Serialization/Inspect.hpp"
#include "Engine/Serialization/Serialize.hpp"
#include "Engine/Serialization/InspectContext.hpp"
#include "Refureku/Object.h"
#include "Generated/Component.rfk.h"

namespace GPE RFKNamespace()
{
    template <>
    void DataInspector::inspect(GPE::InspectContext& context, class Component & inspected);

    class GameObject;

    class RFKClass(Inspect(false)) Component : rfk::Object
    {
    protected:
        GameObject*              m_gameObject; // can not be ref for move
        RFKField(Inspect()) bool m_isActivated{true};

    public:
        inline Component(GameObject & owner) noexcept;
        inline Component() noexcept                       = delete;
        inline Component(const Component& other) noexcept = delete;
        inline Component(Component && other) noexcept     = default;
        inline virtual ~Component() noexcept              = default;
        inline Component& operator=(const Component& other) noexcept = delete;
        inline Component& operator=(Component&& other) noexcept = default;

        [[nodiscard]] constexpr inline GameObject& getOwner() noexcept;

        [[nodiscard]] constexpr inline const GameObject& getOwner() const noexcept;

        [[nodiscard]] constexpr inline bool isActivated() const noexcept;

        constexpr inline void setActive(bool newState) noexcept;

        virtual void moveTowardScene(class Scene & newOwner){};

        //virtual void destroy() = 0;

        Component_GENERATED
    };

#include "Component.inl"

} // namespace )

File_GENERATED