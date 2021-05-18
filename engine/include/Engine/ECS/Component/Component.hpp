/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Serialization/ComponentGen.h"
#include "Engine/Serialization/DataInspector.hpp"
#include "Engine/Serialization/Inspect.hpp"
#include "Engine/Serialization/InspectContext.hpp"
#include "Engine/Serialization/Serialize.hpp"
#include "Engine/Serialization/xml/xmlLoader.hpp"
#include "Engine/Serialization/xml/xmlSaver.hpp"
#include "Refureku/Object.h"

// Generated
#include "Generated/Component.rfk.h"

namespace GPE RFKNamespace()
{
    template <>
    void DataInspector::inspect(GPE::InspectContext & context, class Component & inspected);

    class GameObject;

    class RFKClass(Inspect(false), Serialize(false)) Component : public rfk::Object
    {
    protected:
        RFKField(Serialize()) GameObject*                m_gameObject{nullptr}; // can not be ref for move
        RFKField(Inspect("setActive"), Serialize()) bool m_isActivated{true};

    public:
        inline Component(GameObject & owner) noexcept;
        inline Component() noexcept                       = default;
        inline Component(const Component& other) noexcept = delete;
        inline Component(Component && other) noexcept     = default;
        inline virtual ~Component() noexcept              = default;
        inline Component& operator=(const Component& other) noexcept = delete;
        inline Component& operator=(Component&& other) noexcept = default;

        [[nodiscard]] constexpr inline GameObject& getOwner() noexcept;

        /**
         * @brief WARNING this function is not legal. Do not use it if you don't now exactly it's effect.
                          Must be use only if the component is generate without parent
         * @param owner
         * @return
        */
        inline void setOwner(GameObject & owner) noexcept;

        [[nodiscard]] constexpr inline const GameObject& getOwner() const noexcept;

        [[nodiscard]] constexpr inline bool isActivated() const noexcept;

        virtual inline void setActive(bool newState) noexcept;

        virtual void moveTowardScene(class Scene & newOwner)
        {
        }

        virtual void destroy()
        {
        }

        virtual void onPostLoad();

        Component_GENERATED
    };
} // namespace )

#include "Component.inl"