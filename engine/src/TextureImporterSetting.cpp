#include "Engine/Serialization/TextureImporterSetting.hpp"

#include "Engine/Core/Tools/ImGuiTools.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/GPMDataInspector.hpp"
#include "Engine/Serialization/STDDataInspector.hpp"

File_GENERATED

    namespace GPE
{

    TextureImporterSetting::TextureImporterSetting(const char* srcPath, const char* dstPath)
        : IInspectable(), m_config(), m_srcPath(srcPath), m_dstPath(dstPath)
    {
    }

    void TextureImporterSetting::inspect(InspectContext & context)
    {
        ImGui::TextUnformatted("Texture importer setting");

        ImGui::TextUnformatted("Source");
        ImGui::SameLine();
        if (ImGui::Button((m_srcPath.empty() ? "None##Source" : m_srcPath.c_str())))
        {
            m_srcPath =
                openFileExplorerAndGetAbsoluePath(L"Select image", {{L"Image", L"*.jpg;*.jpeg;*.bmp;*.png;*.tga"}})
                    .string();
        }

        ImGui::TextUnformatted("Destination");
        ImGui::SameLine();
        if (ImGui::Button((m_dstPath.empty() ? "None##Destination" : m_dstPath.c_str())))
        {
            m_dstPath =
                openFileExplorerAndGetAbsoluePath(L"Select image", {{L"Image", L"*.jpg;*.jpeg;*.bmp;*.png;*.tga"}})
                    .string();
        }

        ImGui::Checkbox("Enable vertical flip", &m_config.verticalFlip);

        int                            currentItem  = (int)m_config.format;
        const std::vector<const char*> combotChoise = m_config.allFormatToString();
        if (ImGui::Combo("Internal format :", &currentItem, combotChoise.data(), combotChoise.size()))
        {
            m_config.format = (TextureImportConfig::EFormatType)currentItem;
        }

        ImGui::PushEnabled(!m_isImported);
        if (ImGui::Button("Import"))
        {
            importeTextureFile(m_srcPath.c_str(), m_dstPath.c_str(), m_config);
            m_isImported = true;
        }

        ImGui::PopEnabled();
    }

} // End of namespace GPE