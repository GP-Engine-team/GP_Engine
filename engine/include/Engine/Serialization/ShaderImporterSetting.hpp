/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/IInspectable.hpp"

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
            ImGui::Text("Shader importer");

            ImGui::TextUnformatted("Vertex shader");
            ImGui::SameLine();
            if (ImGui::Button((m_config.vertexShaderPath.empty() ? "None##Vertex" : m_config.vertexShaderPath.c_str())))
            {
                m_config.vertexShaderPath =
                    openFileExplorer(L"Select vertex shader", {{L"Vertex Shader", L"*.vs"}}).string().c_str();

                m_isDirty = true;
            }

            ImGui::TextUnformatted("Fragment shader");
            ImGui::SameLine();
            if (ImGui::Button(m_config.fragmentShaderPath.empty() ? "None##Fragment"
                                                                  : m_config.fragmentShaderPath.c_str()))
            {
                m_config.fragmentShaderPath =
                    openFileExplorer(L"Select fragment shader", {{L"Fragment Shader", L"*.fs"}}).string().c_str();

                m_isDirty = true;
            }

            ImGui::PushEnabled(m_isDirty);

            if (ImGui::Button("Apply"))
            {
                writeShaderFile(m_path.c_str(), m_config);
                m_isDirty = false;
            }

            ImGui::PopEnabled();
        }
        ShaderImporterModifier_GENERATED
    };
} // namespace )
File_GENERATED