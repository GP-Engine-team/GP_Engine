/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/IInspectable.hpp"
#include "Engine/engine.hpp"

#include "Engine/Core/Tools/ImGuiTools.hpp"
#include <imgui/imgui.h>

#include "Generated/ShaderImporterSetting.rfk.h"
#include <string>

namespace GPE RFKNamespace()
{

    class RFKClass() ShaderImporterModifier : public IInspectable // Allow user to modify shader importation setting
    {
    protected:
        std::string       m_path = nullptr;
        ShaderCreateonfig m_config;
        bool              m_isDirty = false;

    public:
        ShaderImporterModifier() = delete;

        ShaderImporterModifier(const std::string& inPath)
            : IInspectable(), m_path(inPath), m_config(readShaderFile(inPath.c_str()))
        {
        }

        void inspect(InspectContext & context) override
        {
            ImGui::TextUnformatted("Shader importer");

            ImGui::TextUnformatted("Vertex shader");
            ImGui::SameLine();
            if (ImGui::Button((m_config.vertexShaderPath.empty() ? "None##Vertex" : m_config.vertexShaderPath.c_str())))
            {

                m_config.vertexShaderPath =
                    openFileExplorerAndGetRelativePath(L"Select vertex shader", {{L"Vertex Shader", L"*.vs"}})
                        .string()
                        .c_str();

                m_isDirty = true;
            }

            ImGui::TextUnformatted("Fragment shader");
            ImGui::SameLine();
            if (ImGui::Button(m_config.fragmentShaderPath.empty() ? "None##Fragment"
                                                                  : m_config.fragmentShaderPath.c_str()))
            {
                m_config.fragmentShaderPath =
                    openFileExplorerAndGetRelativePath(L"Select fragment shader", {{L"Fragment Shader", L"*.fs"}})
                        .string()
                        .c_str();

                m_isDirty = true;
            }

            unsigned char featureMask        = m_config.featureMask;
            bool          blinPhongFlag      = m_config.featureMask & LIGHT_BLIN_PHONG;
            bool          skyboxFlag         = m_config.featureMask & SKYBOX;
            bool          ambiantOnlyFlag    = m_config.featureMask & AMBIANTE_COLOR_ONLY;
            bool          scaleTimeAccFlag   = m_config.featureMask & SCALE_TIME_ACC;
            bool          UnscaleTimeAccFlag = m_config.featureMask & UNSCALED_TIME_ACC;

            ImGui::PushEnabled(!ambiantOnlyFlag);
            if (ImGui::Checkbox("LIGHT BLIN PHONG", &blinPhongFlag))
            {
                m_config.featureMask ^= LIGHT_BLIN_PHONG;
                m_isDirty = true;
            }
            ImGui::PopEnabled();

            if (ImGui::Checkbox("SKYBOX", &skyboxFlag))
            {
                m_config.featureMask ^= SKYBOX;
                m_isDirty = true;
            }

            ImGui::PushEnabled(!blinPhongFlag);
            if (ImGui::Checkbox("AMBIANTE COLOR ONLY", &ambiantOnlyFlag))
            {
                m_config.featureMask ^= AMBIANTE_COLOR_ONLY;
                m_isDirty = true;
            }
            ImGui::PopEnabled();

            if (ImGui::Checkbox("SCALE TIME ACC", &scaleTimeAccFlag))
            {
                m_config.featureMask ^= SCALE_TIME_ACC;
                m_isDirty = true;
            }

            if (ImGui::Checkbox("UNSCALED TIME ACC", &UnscaleTimeAccFlag))
            {
                m_config.featureMask ^= UNSCALED_TIME_ACC;
                m_isDirty = true;
            }

            ImGui::PushEnabled(m_isDirty);

            if (ImGui::Button("Apply"))
            {
                writeShaderFile(m_path.c_str(), m_config);

                // Update loaded resource
                if (Shader* pShader = Engine::getInstance()->resourceManager.get<Shader>(m_path.c_str()))
                {
                    pShader->reload((std::filesystem::current_path() / m_config.vertexShaderPath).string().c_str(),
                                    (std::filesystem::current_path() / m_config.fragmentShaderPath).string().c_str(),
                                    m_config.featureMask);
                }

                m_isDirty = false;
            }

            ImGui::PopEnabled();
        }
        ShaderImporterModifier_GENERATED
    };
} // namespace )
File_GENERATED