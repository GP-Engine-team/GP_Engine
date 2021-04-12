/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Tools/ImGuiTools.hpp"
#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/GPMDataInspector.hpp"
#include "Engine/Serialization/IInspectable.hpp"
#include "Engine/Serialization/STDDataInspector.hpp"

#include <imgui/imgui.h>
#include <string>

#include "Generated/MeshImporterSetting.rfk.h"

namespace GPE RFKNamespace()
{
    class RFKClass() MeshImporterModifier : public IInspectable // Allow user to modify shader importation setting
    {
    protected:
        std::string                 m_path = nullptr;
        Mesh::CreateIndiceBufferArg m_config;
        bool                        m_isDirty = false;

    public:
        MeshImporterModifier() = delete;

        MeshImporterModifier(const std::string& inPath)
            : IInspectable(), m_path(inPath), m_config(readMeshFile(inPath.c_str()))
        {
        }

        void inspect(InspectContext & context) override
        {
            ImGui::TextUnformatted("Mesh importer");

            m_isDirty = true;
            // TODO: dirty flag must change when inspect will returned bool

            ImGui::Text("Triangle count : %i", m_config.indices.size() / 3);
            ImGui::Text("Vertices count : %i", m_config.indices.size());

            ImGui::PushEnabled(m_isDirty);

            if (ImGui::Button("Apply"))
            {
                writeMeshFile(m_path.c_str(), m_config);
                m_isDirty = false;
            }

            ImGui::PopEnabled();
        }
        MeshImporterModifier_GENERATED
    };
} // namespace )
File_GENERATED