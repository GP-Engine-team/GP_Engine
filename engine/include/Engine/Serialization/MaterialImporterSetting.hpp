/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Tools/ImGuiTools.hpp"
#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/GPMDataInspector.hpp"
#include "Engine/Serialization/IInspectable.hpp"
#include "Engine/Serialization/STDDataInspector.hpp"

#include <imgui/imgui.h>
#include <string>

#include "Generated/MaterialImporterSetting.rfk.h"

namespace GPE RFKNamespace()
{
    class RFKClass() MaterialImporterModifier : public IInspectable // Allow user to modify shader importation setting
    {
    protected:
        std::string          m_path = nullptr;
        Material::ImporteArg m_config;
        bool                 m_isDirty = false;

    public:
        MaterialImporterModifier() = delete;

        MaterialImporterModifier(const std::string& inPath)
            : IInspectable(), m_path(inPath), m_config(readMaterialFile(inPath.c_str()))
        {
        }

        void inspect(InspectContext & context) override
        {
            ImGui::TextUnformatted("Material importer");

            m_isDirty = true;
            // TODO: dirty flag must change when inspect will returned bool
            GPE::DataInspector::inspect(context, m_config.comp.ambient.rgbi, "ambient");
            GPE::DataInspector::inspect(context, m_config.comp.diffuse.rgbi, "diffuse");
            GPE::DataInspector::inspect(context, m_config.comp.specular.rgbi, "specular");
            GPE::DataInspector::inspect(context, m_config.comp.shininess, "shininess");
            GPE::DataInspector::inspect(context, m_config.comp.opacity, "opacity");

            ImGui::TextUnformatted("Diffuse texture");
            ImGui::SameLine();
            if (ImGui::Button(
                    (m_config.diffuseTexturePath.empty() ? "None##Vertex" : m_config.diffuseTexturePath.c_str())))
            {
                m_config.diffuseTexturePath =
                    openFileExplorerAndGetRelativePath(L"Select vertex shader",
                                                       {{L"Image", L"*.jpg;*.jpeg;*.bmp;*.png;*.tga"}})
                        .string()
                        .c_str();

                m_isDirty = true;
            }

            ImGui::PushEnabled(m_isDirty);

            if (ImGui::Button("Apply"))
            {
                writeMaterialFile(m_path.c_str(), m_config);
                m_isDirty = false;
            }

            ImGui::PopEnabled();
        }
        MaterialImporterModifier_GENERATED
    };
} // namespace )
File_GENERATED