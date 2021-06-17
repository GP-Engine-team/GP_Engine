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
        ImGui::Text("Path : %s", m_path.c_str());

        m_isDirty = true;
        // TODO: dirty flag must change when inspect will returned bool
        GPE::DataInspector::inspect(context, m_config.comp.ambient, "ambient");
        GPE::DataInspector::inspect(context, m_config.comp.diffuse, "diffuse");
        GPE::DataInspector::inspect(context, m_config.comp.specular, "specular");
        GPE::DataInspector::inspect(context, m_config.comp.shininess, "shininess");
        GPE::DataInspector::inspect(context, m_config.comp.opacity, "opacity");

        // ImGui::PushID("Ambiante");
        // ImGui::TextUnformatted("Ambiante texture");
        // ImGui::SameLine();
        // if (ImGui::Button(
        //        (m_config.ambianteTexturePath.empty() ? "None##Ambiante" : m_config.ambianteTexturePath.c_str())))
        //{
        //    m_config.ambianteTexturePath =
        //        openFileExplorerAndGetRelativePath(L"Ambiante texture", {{L"Image",
        //        L"*.GPTexture"}}).string().c_str();

        //    m_isDirty = true;
        //}
        // ImGui::PopID();

        ImGui::PushID("Diffuse");
        ImGui::TextUnformatted("Diffuse texture");
        ImGui::SameLine();
        if (ImGui::Button(
                (m_config.diffuseTexturePath.empty() ? "None##DiffuseTexture" : m_config.diffuseTexturePath.c_str())))
        {
            m_config.diffuseTexturePath =
                openFileExplorerAndGetRelativePath(L"Diffuse texture", {{L"Image", L"*.GPTexture"}}).string().c_str();

            m_isDirty = true;
        }
        // Drop from content browser
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_TEXTURE_EXTENSION))
            {
                IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                m_config.diffuseTexturePath = static_cast<std::filesystem::path*>(payload->Data)->string();
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::PopID();

        ImGui::PushID("Normal");
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
        // Drop from content browser
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_TEXTURE_EXTENSION))
            {
                IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                m_config.normalMapTexturePath = static_cast<std::filesystem::path*>(payload->Data)->string();
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopID();

        ImGui::Separator();
        ImGui::PushID("Shader");
        ImGui::TextUnformatted("Shader");
        ImGui::SameLine();
        if (ImGui::Button((m_config.shaderPath.empty() ? "None##Shader" : m_config.shaderPath.c_str())))
        {
            m_config.shaderPath =
                openFileExplorerAndGetRelativePath(L"Select shader", {{L"Shader", L"*.GPShader"}}).string().c_str();
            m_isDirty = true;
        }
        // Drop from content browser
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_SHADER_EXTENSION))
            {
                IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                m_config.shaderPath = static_cast<std::filesystem::path*>(payload->Data)->string();
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopID();
        if (m_config.uniforms.size())
            GPE::DataInspector::inspect(context, m_config.uniforms, "Uniform");
        ImGui::Separator();

        if (m_isDirty)
        {
            if (Material* pMaterial = Engine::getInstance()->resourceManager.get<Material>(m_path))
            {
                pMaterial->setComponent(m_config.comp);

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

                if (Shader* pShader = Engine::getInstance()->resourceManager.get<Shader>(m_config.shaderPath))
                    pMaterial->setShader(pShader);
                else
                    pMaterial->setShader(loadShaderFile(m_config.shaderPath.c_str()));
            }
            m_isDirty           = false;
            m_canSaveInHardDisk = true;
        }

        ImGui::PushEnabled(m_canSaveInHardDisk);
        if (ImGui::Button("Apply"))
        {
            writeMaterialFile(m_path.c_str(), m_config);
            m_canSaveInHardDisk = false;
        }
        ImGui::PopEnabled();
    }

} // End of namespace GPE