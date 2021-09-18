/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Generated/MainMenu.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(ComponentGen, Serialize()) MainMenu : public GPE::BehaviourComponent
    {
    protected:
        RFKField(Serialize()) GPE::Texture* m_buttonTexture;
        RFKField(Serialize()) std::string   m_scenePath;

    public:
        MainMenu() noexcept = default;
        MainMenu(GPE::GameObject & owner) noexcept;
        MainMenu(const MainMenu& other) noexcept = delete;
        MainMenu(MainMenu && other) noexcept     = delete;
        virtual ~MainMenu() noexcept;

        MainMenu& operator=(MainMenu const& other) noexcept = delete;
        MainMenu& operator=(MainMenu&& other) noexcept = delete;

    public:
        void start() final;
        void onGUI() final;
        void onPostLoad() final;

        void inspect(GPE::InspectContext & context);

        MainMenu_GENERATED
    };

} // namespace )

File_GENERATED