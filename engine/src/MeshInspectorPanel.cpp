#include "Engine/Core/Tools/ImGuiTools.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/GPMDataInspector.hpp"
#include "Engine/Serialization/STDDataInspector.hpp"

#include "Engine/Serialization/MeshInspectorPanel.hpp"
File_GENERATED

    namespace GPE
{

    MeshInspectorPanel::MeshInspectorPanel(const std::string& inPath)
        : IInspectable(), m_config(readMeshFile(inPath.c_str())), m_path(inPath), m_isDirty(false)
    {
    }

    void MeshInspectorPanel::inspect(InspectContext & context)
    {
        ImGui::TextUnformatted("Mesh importer");
        ImGui::Text("Path : %s", m_path.c_str());

        // TODO: dirty flag must change when inspect will returned bool

        ImGui::Text("Triangle count : %i", m_config.indices.size() / 3);
        ImGui::Text("Vertices count : %i", m_config.indices.size());

        int         elem                                           = (int)m_config.boundingVolumeType;
        const char* elems_names[(int)Mesh::EBoundingVolume::COUNT] = {"NONE", "SPHERE", "AABB"};
        const char* elem_name = (elem >= 0 && elem < (int)Mesh::EBoundingVolume::COUNT) ? elems_names[elem] : "Unknown";
        ImGui::TextUnformatted("Bounding volume");
        ImGui::SameLine();
        if (ImGui::SliderInt("##BoundingVolumeSlider", &elem, 0, (int)Mesh::EBoundingVolume::COUNT - 1, elem_name))
        {
            m_isDirty = true;
        }
        m_config.boundingVolumeType = (Mesh::EBoundingVolume)elem;

        ImGui::PushEnabled(m_isDirty);

        if (ImGui::Button("Apply"))
        {
            writeMeshFile(m_path.c_str(), m_config);

            // Update loaded resource
            if (Mesh* pMesh = Engine::getInstance()->resourceManager.get<Mesh>(m_path))
            {
                // Check if user want to get bounding volume
                if (pMesh->getBoundingVolumeType() != m_config.boundingVolumeType)
                {
                    pMesh->setBoundingVolume(m_config.boundingVolumeType);
                }
            }

            m_isDirty = false;
        }

        ImGui::PopEnabled();
    }

} // End of namespace GPE