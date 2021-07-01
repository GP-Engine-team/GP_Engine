#include "Engine/Serialization/ModelImporterSetting.hpp"

#include "Engine/Core/Tools/ImGuiTools.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/GPMDataInspector.hpp"
#include "Engine/Serialization/STDDataInspector.hpp"

File_GENERATED

    namespace GPE
{

    ModelImporterSetting::ModelImporterSetting(const std::string& inPath)
        : IInspectable(), m_path(inPath), m_isDirty(false)
    {
        readMeshFile(inPath.c_str(), m_config);
    }

    void ModelImporterSetting::inspect(InspectContext & context)
    {
        ImGui::TextUnformatted("Model importer setting");

        m_isDirty = true;
        // TODO: dirty flag must change when inspect will returned bool

        ImGui::Text("Triangle count : %i", m_config.elemCount / 3);
        ImGui::Text("Vertices count : %i", m_config.dataBufferSize / m_config.getVertexSize());

        ImGui::PushEnabled(m_isDirty);

        if (ImGui::Button("Apply"))
        {
            writeMeshFile(m_path.c_str(), m_config);
            m_isDirty = false;
        }

        ImGui::PopEnabled();
    }

} // End of namespace GPE