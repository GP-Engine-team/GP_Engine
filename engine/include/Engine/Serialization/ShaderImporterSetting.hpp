/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/IInspectable.hpp"

#include "imgui/imgui_internal.h" //ImGuiItemFlags_Disabled, PushItemFlag

#include "Generated/ShaderImporterSetting.rfk.h"
#include <string>

// Thank's to : https://github.com/ocornut/imgui/issues/211#issuecomment-812293268
namespace ImGui
{
inline void PushEnabled(bool _enabled)
{
    PushItemFlag(ImGuiItemFlags_Disabled, !_enabled);
    PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * (_enabled ? 1.0f : 0.5f));
}

inline void PopEnabled()
{
    PopItemFlag();
    PopStyleVar();
}

} // namespace ImGui

namespace GPE RFKNamespace()
{

    struct RFKStruct() ShaderImporterModifier : public IInspectable // Allow user to modify shader importation setting
    {
        std::string       path = nullptr;
        ShaderCreateonfig config;
        bool              isDirty = false;

        ShaderImporterModifier(const std::string& inPath) : IInspectable(), path(inPath), config(readShaderFile(inPath.c_str()))
        {
        }

        void inspect(InspectContext & context) override
        {
            ImGui::Text("Shader importer configuration");
            if (ImGui::Button((config.vertexShaderPath.empty() ? "None##Vertex" : config.vertexShaderPath.c_str())))
            {
                config.vertexShaderPath =
                    openFileExplorer(L"Select vertex shader", {{L"Vertex Shader", L"*.vs"}}).string().c_str();

                isDirty = true;
            }

            if (ImGui::Button(config.fragmentShaderPath.empty() ? "None##Fragment" : config.fragmentShaderPath.c_str()))
            {
                config.fragmentShaderPath =
                    openFileExplorer(L"Select fragment shader", {{L"Fragment Shader", L"*.fs"}}).string().c_str();

                isDirty = true;
            }

            ImGui::PushEnabled(isDirty);

            if (ImGui::Button("Apply"))
            {
                writeShaderFile(path.c_str(), config);
                isDirty = false;
            }

            ImGui::PopEnabled();
        }
        ShaderImporterModifier_GENERATED
    };
} // namespace )
File_GENERATED