#include "Engine/Core/Tools/ImGuiTools.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/GPMDataInspector.hpp"
#include "Engine/Serialization/STDDataInspector.hpp"

#include "Engine/Serialization/MaterialInspectorPanel.hpp"
File_GENERATED

    namespace GPE
{

    MaterialInspectorPanel::MaterialInspectorPanel(const std::string& inPath)
        : IInspectable(), m_config(readMaterialFile(inPath.c_str())), m_path(inPath), m_isDirty(false)
    {
    }

    void MaterialInspectorPanel::inspect(InspectContext & context)
    {
        ImGui::TextUnformatted("Material importer");

        m_isDirty = true;
        // TODO: dirty flag must change when inspect will returned bool
        GPE::DataInspector::inspect(context, m_config.comp.ambient, "ambient");
        GPE::DataInspector::inspect(context, m_config.comp.diffuse, "diffuse");
        GPE::DataInspector::inspect(context, m_config.comp.specular, "specular");
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

        ImGui::TextUnformatted("Normal map texture");
        ImGui::SameLine();
        if (ImGui::Button(
                (m_config.normalMapTexturePath.empty() ? "None##NormalMap" : m_config.normalMapTexturePath.c_str())))
        {
            m_config.normalMapTexturePath =
                openFileExplorerAndGetRelativePath(L"Normal map texture", {{L"Image", L"*.GPTexture"}})
                    .string()
                    .c_str();

            m_isDirty = true;
        }

        ImGui::PushEnabled(m_isDirty);

        if (ImGui::Button("Apply"))
        {
            writeMaterialFile(m_path.c_str(), m_config);

            // Update loaded resource
            if (Material* pMaterial = Engine::getInstance()->resourceManager.get<Material>(m_path))
            {
                pMaterial->setComponent(m_config.comp);

                if (Texture* pTexture =
                        Engine::getInstance()->resourceManager.get<Texture>(m_config.ambianteTexturePath))
                    pMaterial->setAmbianteTexture(pTexture);
                else
                    pMaterial->setAmbianteTexture(loadTextureFile(m_config.ambianteTexturePath.c_str()));

                if (Texture* pTexture =
                        Engine::getInstance()->resourceManager.get<Texture>(m_config.diffuseTexturePath.c_str()))
                    pMaterial->setDiffuseTexture(pTexture);
                else
                    pMaterial->setDiffuseTexture(loadTextureFile(m_config.diffuseTexturePath.c_str()));

                if (Texture* pTexture =
                        Engine::getInstance()->resourceManager.get<Texture>(m_config.normalMapTexturePath))
                    pMaterial->setNormalMapTexture(pTexture);
                else
                    pMaterial->setNormalMapTexture(loadTextureFile(m_config.normalMapTexturePath.c_str()));
            }

            m_isDirty = false;
        }
        ImGui::PopEnabled();
    }

} // End of namespace GPE