#include "Engine/Core/Tools/ImGuiTools.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/GPMDataInspector.hpp"
#include "Engine/Serialization/STDDataInspector.hpp"

#include "Engine/Serialization/MeshImporterSetting.hpp"
File_GENERATED

namespace GPE
{

MeshImporterModifier::MeshImporterModifier(const std::string& inPath)
    : IInspectable(),
      m_config(readMeshFile(inPath.c_str())), m_path(inPath),
      m_isDirty(false)
{
}

void MeshImporterModifier::inspect(InspectContext & context)
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

} // End of namespace GPE