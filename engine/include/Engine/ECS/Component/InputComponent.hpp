﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include <Engine/Core/Tools/FunctionPtr.hpp>
#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Serialization/ComponentGen.h>

// Generated
#include <Generated/InputComponent.rfk.h>

enum class EKeyMode
{
    KEY_PRESSED  = 0,
    KEY_RELEASED = 2,
    KEY_DOWN     = 3,
    KEY_UP       = 4,
};

namespace GPE
{
template <>
void save(XmlSaver& context, const EKeyMode& inspected, const XmlSaver::SaveInfo& info);
template <>
void load(XmlLoader& context, EKeyMode& inspected, const XmlLoader::LoadInfo& info);
} // namespace GPE

enum class EInputMode
{
    EDITOR = 0,
    GAME   = 1,
};

namespace GPE RFKNamespace()
{
    class RFKClass(ComponentGen(), Serialize()) InputComponent : public Component
    {
    public:
        InputComponent(GameObject & owner);

        InputComponent();
        InputComponent(const InputComponent& other) = delete;
        InputComponent& operator=(InputComponent const& other) = delete;
        virtual ~InputComponent();
        InputComponent(InputComponent && other);
        InputComponent& operator=(InputComponent&& other);

    private:
        RFKField(Serialize())
        std::unordered_map<std::string, GPE::Function> m_functionMap;
        int                                            m_key = -1;

    public:
        RFKField(Serialize())
        std::unordered_map<std::string, EKeyMode>    keyModeMap;
        RFKField(Serialize())
        std::unordered_map<std::string, std::string> inputModeMap;
        /**
         * @brief Bind a function to an action
         * @param action
         * @param function
         */

        template <typename T>
        void bindAction(const std::string& action, const EKeyMode& keyMode, const std::string& inputMode, T* owner,
                        const std::string& methodName) noexcept
        {
            m_functionMap.emplace(action, GPE::Function::make(owner, methodName));
            keyModeMap.emplace(action, keyMode);
            inputModeMap.emplace(action, inputMode);
        }

        /**
         * @brief launch an action
         * @param action
         */
        void fireAction(const std::string& action) noexcept;

        /**
         * @brief Add or remove current component from it's system which have for effect to enable or disable it
         * @param newState
         * @return
         */
        void setActive(bool newState) noexcept override;

        virtual void awake() override;

        InputComponent_GENERATED
    };
} // namespace )
