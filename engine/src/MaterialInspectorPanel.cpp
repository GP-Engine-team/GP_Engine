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
        : IInspectable(), pMat{loadMaterialFile(inPath.c_str())}, m_canSaveInHardDisk{false}
    {
    }

    void MaterialInspectorPanel::inspect(InspectContext & context)
    {
        ImGui::TextUnformatted("Material importer");
        ImGui::Text("Path : %s", Engine::getInstance()->resourceManager.getKey(pMat)->c_str());

        GPE::DataInspector::inspect(context, pMat->getComponent().ambient, "ambient");
        m_canSaveInHardDisk |= context.wasLastDirty();

        GPE::DataInspector::inspect(context, pMat->getComponent().diffuse, "diffuse");
        m_canSaveInHardDisk |= context.wasLastDirty();

        GPE::DataInspector::inspect(context, pMat->getComponent().specular, "specular");
        m_canSaveInHardDisk |= context.wasLastDirty();

        GPE::DataInspector::inspect(context, pMat->getComponent().shininess, "shininess");
        m_canSaveInHardDisk |= context.wasLastDirty();

        GPE::DataInspector::inspect(context, pMat->getComponent().opacity, "opacity");
        m_canSaveInHardDisk |= context.wasLastDirty();

        ImGui::Separator();

        {
            const std::string* const pStr = Engine::getInstance()->resourceManager.getKey(pMat->getDiffuseTexture());
            PathTo<Texture>          path{pStr ? *pStr : "None"};
            GPE::DataInspector::inspect(context, path, "Diffuse");
            m_canSaveInHardDisk |= context.wasLastDirty();

            if (context.wasLastDirty())
            {
                pMat->setDiffuseTexture(loadTextureFile(path.path.c_str()));
            }
        }

        {
            const std::string* const pStr = Engine::getInstance()->resourceManager.getKey(pMat->getNormalMapTexture());
            PathTo<Texture>          path{pStr ? *pStr : "None"};
            GPE::DataInspector::inspect(context, path, "Normal");
            m_canSaveInHardDisk |= context.wasLastDirty();

            if (context.wasLastDirty())
            {
                pMat->setNormalMapTexture(loadTextureFile(path.path.c_str()));
            }
        }

        ImGui::Separator();

        {
            const std::string* const pStr = Engine::getInstance()->resourceManager.getKey(pMat->getShader());
            PathTo<Shader>           path{pStr ? *pStr : "None"};
            GPE::DataInspector::inspect(context, path, "Shader");
            m_canSaveInHardDisk |= context.wasLastDirty();

            if (context.wasLastDirty())
            {
                pMat->setShader(*loadShaderFile(path.path.c_str()));
            }
        }

        if (pMat->getUniforms().size())
        {
            std::function<void(const std::string&, std::unique_ptr<IUniform>&)> funct;
            funct = [&](const std::string& key, std::unique_ptr<IUniform>& value) {
                pMat->getShader()->use();
                value->sendToShader(key.c_str(), *pMat->getShader());
            };
            GPE::DataInspector::inspect(context, pMat->getUniforms(), "Uniform", funct);
            m_canSaveInHardDisk |= context.wasLastDirty();
        }

        ImGui::Separator();

        ImGui::PushEnabled(m_canSaveInHardDisk);
        if (ImGui::Button("Apply"))
        {
            writeMaterialFile(Engine::getInstance()->resourceManager.getKey(pMat)->c_str(), *pMat);
            m_canSaveInHardDisk = false;
        }
        ImGui::PopEnabled();
    }

} // End of namespace GPE