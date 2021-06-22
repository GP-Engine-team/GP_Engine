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

        m_isDirty = false;
        // TODO: dirty flag must change when inspect will returned bool
        GPE::DataInspector::inspect(context, m_config.comp.ambient, "ambient");
        m_isDirty |= context.wasLastDirty();

        GPE::DataInspector::inspect(context, m_config.comp.diffuse, "diffuse");
        m_isDirty |= context.wasLastDirty();

        GPE::DataInspector::inspect(context, m_config.comp.specular, "specular");
        m_isDirty |= context.wasLastDirty();

        GPE::DataInspector::inspect(context, m_config.comp.shininess, "shininess");
        m_isDirty |= context.wasLastDirty();

        GPE::DataInspector::inspect(context, m_config.comp.opacity, "opacity");
        m_isDirty |= context.wasLastDirty();

        GPE::DataInspector::inspect(context, m_config.comp.opacity, "opacity");

        ImGui::Separator();

        GPE::DataInspector::inspect(context, m_config.diffuseTexturePath, "Diffuse");
        m_isDirty |= context.wasLastDirty();

        GPE::DataInspector::inspect(context, m_config.normalMapTexturePath, "Normal");
        m_isDirty |= context.wasLastDirty();

        ImGui::Separator();

        GPE::DataInspector::inspect(context, m_config.shaderPath, "Shader");
        m_isDirty |= context.wasLastDirty();

        if (m_config.uniforms.size())
        {
            GPE::DataInspector::inspect(context, m_config.uniforms, "Uniform");
            m_isDirty |= context.wasLastDirty();
        }

        ImGui::Separator();

        if (m_isDirty)
        {
            if (Material* pMaterial = Engine::getInstance()->resourceManager.get<Material>(m_path))
            {
                pMaterial->setComponent(m_config.comp);

                if (Texture* pTexture =
                        Engine::getInstance()->resourceManager.get<Texture>(m_config.diffuseTexturePath.path.c_str()))
                    pMaterial->setDiffuseTexture(pTexture);
                else
                    pMaterial->setDiffuseTexture(loadTextureFile(m_config.diffuseTexturePath.path.c_str()));

                if (Texture* pTexture =
                        Engine::getInstance()->resourceManager.get<Texture>(m_config.normalMapTexturePath))
                    pMaterial->setNormalMapTexture(pTexture);
                else
                    pMaterial->setNormalMapTexture(loadTextureFile(m_config.normalMapTexturePath.path.c_str()));

                if (Shader* pShader = Engine::getInstance()->resourceManager.get<Shader>(m_config.shaderPath))
                    pMaterial->setShader(*pShader);
                else
                    pMaterial->setShader(*loadShaderFile(m_config.shaderPath.path.c_str()));
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