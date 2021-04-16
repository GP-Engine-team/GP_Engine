#include "Engine/Core/Tools/ImGuiTools.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/GPMDataInspector.hpp"
#include "Engine/Serialization/STDDataInspector.hpp"

#include "Engine/Serialization/MaterialImporterSetting.hpp"
File_GENERATED

    namespace GPE
{

    MaterialImporterModifier::MaterialImporterModifier(const std::string& inPath)
        : IInspectable(), m_config(readMaterialFile(inPath.c_str())), m_path(inPath), m_isDirty(false)
    {
    }

    void MaterialImporterModifier::inspect(InspectContext & context)
    {
        ImGui::TextUnformatted("Material importer");

        m_isDirty = true;
        // TODO: dirty flag must change when inspect will returned bool
        GPE::DataInspector::inspect(context, m_config.comp.ambient.rgbi, "ambient");
        GPE::DataInspector::inspect(context, m_config.comp.diffuse.rgbi, "diffuse");
        GPE::DataInspector::inspect(context, m_config.comp.specular.rgbi, "specular");
        GPE::DataInspector::inspect(context, m_config.comp.shininess, "shininess");
        GPE::DataInspector::inspect(context, m_config.comp.opacity, "opacity");

        ImGui::TextUnformatted("Ambiante texture");
        ImGui::SameLine();
        if (ImGui::Button(
                (m_config.ambianteTexturePath.empty() ? "None##Ambiante" : m_config.ambianteTexturePath.c_str())))
        {
            m_config.ambianteTexturePath =
                openFileExplorerAndGetRelativePath(L"Ambiante texture", {{L"Image", L"*.GPTexture"}}).string().c_str();

            m_isDirty = true;
        }

        ImGui::TextUnformatted("Diffuse texture");
        ImGui::SameLine();
        if (ImGui::Button(
                (m_config.diffuseTexturePath.empty() ? "None##Diffuse" : m_config.diffuseTexturePath.c_str())))
        {
            m_config.diffuseTexturePath =
                openFileExplorerAndGetRelativePath(L"Diffuse texture", {{L"Image", L"*.GPTexture"}}).string().c_str();

            m_isDirty = true;
        }

        ImGui::TextUnformatted("Base color texture");
        ImGui::SameLine();
        if (ImGui::Button(
                (m_config.baseColorTexturePath.empty() ? "None##BaseColor" : m_config.baseColorTexturePath.c_str())))
        {
            m_config.baseColorTexturePath =
                openFileExplorerAndGetRelativePath(L"Base color texture", {{L"Image", L"*.GPTexture"}})
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

} // End of namespace GPE