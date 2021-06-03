#include "Engine/Resources/Shader.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/engine.hpp"

#include "Engine/Core/Tools/ImGuiTools.hpp"
#include <imgui/imgui.h>

#include "Engine/Serialization/ShaderInspectorPanel.hpp"

File_GENERATED

    namespace GPE
{
    ShaderInspectorPanel::ShaderInspectorPanel(const std::string& inPath)
        : IInspectable(), m_path(inPath), m_config(readShaderFile(inPath.c_str()))
    {
    }

    void ShaderInspectorPanel::inspect(InspectContext & context)
    {
        ImGui::TextUnformatted("Shader importer");

        ImGui::TextUnformatted("Vertex shader");
        ImGui::SameLine();
        if (ImGui::Button((m_config.vertexShaderPath.empty() ? "None##Vertex" : m_config.vertexShaderPath.c_str())))
        {

            m_config.vertexShaderPath =
                openFileExplorerAndGetRelativePath(L"Select vertex shader", {{L"Vertex Shader", L"*.vs"}})
                    .string()
                    .c_str();

            m_isDirty = true;
        }

        ImGui::TextUnformatted("Fragment shader");
        ImGui::SameLine();
        if (ImGui::Button(m_config.fragmentShaderPath.empty() ? "None##Fragment" : m_config.fragmentShaderPath.c_str()))
        {
            m_config.fragmentShaderPath =
                openFileExplorerAndGetRelativePath(L"Select fragment shader", {{L"Fragment Shader", L"*.fs"}})
                    .string()
                    .c_str();

            m_isDirty = true;
        }

        uint16_t featureMask        = m_config.featureMask;
        bool     blinPhongFlag      = m_config.featureMask & LIGHT_BLIN_PHONG;
        bool     skyboxFlag         = m_config.featureMask & SKYBOX;
        bool     fog                = m_config.featureMask & FOG;
        bool     ambiantOnlyFlag    = m_config.featureMask & AMBIANTE_COLOR_ONLY;
        bool     scaleTimeAccFlag   = m_config.featureMask & SCALE_TIME_ACC;
        bool     unscaleTimeAccFlag = m_config.featureMask & UNSCALED_TIME_ACC;
        bool     projectionMatrix   = m_config.featureMask & PROJECTION_MATRIX;
        bool     viewMatrix         = m_config.featureMask & VIEW_MATRIX;
        bool     PVMMatrix          = m_config.featureMask & PROJECTION_VIEW_MODEL_MATRIX;

        ImGui::PushEnabled(!ambiantOnlyFlag);
        if (ImGui::Checkbox("LIGHT BLIN PHONG", &blinPhongFlag))
        {
            m_config.featureMask ^= LIGHT_BLIN_PHONG;
            m_isDirty = true;
        }
        ImGui::PopEnabled();

        if (ImGui::Checkbox("SKYBOX", &skyboxFlag))
        {
            m_config.featureMask ^= SKYBOX;
            m_isDirty = true;
        }

        if (ImGui::Checkbox("FOG", &fog))
        {
            m_config.featureMask ^= FOG;
            m_isDirty = true;
        }

        ImGui::PushEnabled(!blinPhongFlag);
        if (ImGui::Checkbox("AMBIANTE COLOR ONLY", &ambiantOnlyFlag))
        {
            m_config.featureMask ^= AMBIANTE_COLOR_ONLY;
            m_isDirty = true;
        }
        ImGui::PopEnabled();

        if (ImGui::Checkbox("SCALE TIME ACC", &scaleTimeAccFlag))
        {
            m_config.featureMask ^= SCALE_TIME_ACC;
            m_isDirty = true;
        }

        if (ImGui::Checkbox("UNSCALED TIME ACC", &unscaleTimeAccFlag))
        {
            m_config.featureMask ^= UNSCALED_TIME_ACC;
            m_isDirty = true;
        }

        if (ImGui::Checkbox("PROJECTION MATRIX", &projectionMatrix))
        {
            m_config.featureMask ^= PROJECTION_MATRIX;
            m_isDirty = true;
        }

        if (ImGui::Checkbox("VIEW MATRIX", &viewMatrix))
        {
            m_config.featureMask ^= VIEW_MATRIX;
            m_isDirty = true;
        }

        if (ImGui::Checkbox("PROJECTION_VIEW_MODEL_MATRIX", &PVMMatrix))
        {
            m_config.featureMask ^= PROJECTION_VIEW_MODEL_MATRIX;
            m_isDirty = true;
        }

        ImGui::PushEnabled(m_isDirty);

        if (ImGui::Button("Apply"))
        {
            writeShaderFile(m_path.c_str(), m_config);

            // Update loaded resource
            if (Shader* pShader = Engine::getInstance()->resourceManager.get<Shader>(m_path))
            {
                pShader->reload((std::filesystem::current_path() / m_config.vertexShaderPath).string().c_str(),
                                (std::filesystem::current_path() / m_config.fragmentShaderPath).string().c_str(),
                                m_config.featureMask);
            }

            m_isDirty = false;
        }
        ImGui::PopEnabled();
        ImGui::SameLine();
        if (ImGui::Button("Reload"))
        {
            // Update loaded resource
            if (Shader* pShader = Engine::getInstance()->resourceManager.get<Shader>(m_path))
            {
                pShader->reload((std::filesystem::current_path() / m_config.vertexShaderPath).string().c_str(),
                                (std::filesystem::current_path() / m_config.fragmentShaderPath).string().c_str(),
                                m_config.featureMask);
            }
        }
    }

} // End of namespace GPE