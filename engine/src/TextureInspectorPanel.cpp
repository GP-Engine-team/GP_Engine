#include "Engine/Core/Tools/ImGuiTools.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Serialization/FileExplorer.hpp"
#include "Engine/Serialization/GPMDataInspector.hpp"
#include "Engine/Serialization/STDDataInspector.hpp"

#include "Engine/Serialization/TextureInspectorPanel.hpp"
File_GENERATED

    namespace GPE
{

    TextureInspectorPanel::TextureInspectorPanel(const std::string& inPath)
        : IInspectable(), m_config(readTextureFile(inPath.c_str())), m_texture(m_config), m_path(inPath),
          m_isDirty(false)
    {
    }

    void TextureInspectorPanel::inspect(InspectContext & context)
    {
        context.startProperty("TextureInspectorPanel");
        ImGui::TextUnformatted("Texture setting");
        ImGui::Text("Path : %s", m_path.c_str());

        ImGui::Image((ImTextureID)m_texture.getID(), {ImGui::GetWindowWidth(), ImGui::GetWindowWidth()});

        ImGui::Text("Width : %i ", m_config.w);
        ImGui::Text("Height : %i", m_config.h);
        { // Component
            const char* elems_names[(int)Texture::EFormat::COUNT] = {"NONE", "R", "RG", "RGB", "RGBA"};
            ImGui::Text("Component : %s", elems_names[m_config.comp]);
        }

        { // textureMinFilter
            int         elem                                                = (int)m_config.properties.textureMinFilter;
            const char* elems_names[(int)Texture::ETextureMinFilter::COUNT] = {"NEAREST",
                                                                               "LINEAR",
                                                                               "NEAREST_MIPMAP_NEAREST",
                                                                               "LINEAR_MIPMAP_NEAREST",
                                                                               "NEAREST_MIPMAP_LINEAR",
                                                                               "LINEAR_MIPMAP_LINEAR"};
            const char* elem_name =
                (elem >= 0 && elem < (int)Texture::ETextureMinFilter::COUNT) ? elems_names[elem] : "Unknown";
            ImGui::TextUnformatted("Texture Min Filter");
            ImGui::SameLine();
            if (ImGui::SliderInt("##textureMinFilter", &elem, 0, (int)Texture::ETextureMinFilter::COUNT - 1, elem_name))
            {
                m_isDirty = true;
            }
            m_config.properties.textureMinFilter = (Texture::ETextureMinFilter)elem;
        }

        { // textureMagFilter
            int         elem                                                = (int)m_config.properties.textureMagFilter;
            const char* elems_names[(int)Texture::ETextureMagFilter::COUNT] = {"NEAREST", "LINEAR"};
            const char* elem_name =
                (elem >= 0 && elem < (int)Texture::ETextureMagFilter::COUNT) ? elems_names[elem] : "Unknown";
            ImGui::TextUnformatted("Texture Mag Filter");
            ImGui::SameLine();
            if (ImGui::SliderInt("##textureMagFilter", &elem, 0, (int)Texture::ETextureMagFilter::COUNT - 1, elem_name))
            {
                m_isDirty = true;
            }
            m_config.properties.textureMagFilter = (Texture::ETextureMagFilter)elem;
        }

        { // textureWrap S and T
            int         elem                                                = (int)m_config.properties.textureWrapS;
            const char* elems_names[(int)Texture::ETextureMinFilter::COUNT] = {
                "CLAMP_TO_EDGE", "CLAMP_TO_BORDER", "MIRRORED_REPEAT", "REPEAT", "MIRROR_CLAMP_TO_EDGE"};
            const char* elem_name =
                (elem >= 0 && elem < (int)Texture::ETextureWrap::COUNT) ? elems_names[elem] : "Unknown";
            ImGui::TextUnformatted("Texture Wrap S");
            ImGui::SameLine();
            if (ImGui::SliderInt("##textureWrapS", &elem, 0, (int)Texture::ETextureWrap::COUNT - 1, elem_name))
            {
                m_isDirty = true;
            }
            m_config.properties.textureWrapS = (Texture::ETextureWrap)elem;

            elem      = (int)m_config.properties.textureWrapT;
            elem_name = (elem >= 0 && elem < (int)Texture::ETextureWrap::COUNT) ? elems_names[elem] : "Unknown";
            ImGui::TextUnformatted("Texture Wrap T");
            ImGui::SameLine();
            if (ImGui::SliderInt("##textureWrapT", &elem, 0, (int)Texture::ETextureWrap::COUNT - 1, elem_name))
            {
                m_isDirty = true;
            }
            m_config.properties.textureWrapT = (Texture::ETextureWrap)elem;
        }

        GPE::DataInspector::inspect(context, m_config.properties.anisotropy, "Anisotropy");
        GPE::DataInspector::inspect(context, m_config.properties.generateMipmaps, "Mipmap");

        m_isDirty |= context.isDirty();

        if (m_isDirty)
        {
            if (Texture* pTexture = Engine::getInstance()->resourceManager.get<Texture>(m_path))
            {
                pTexture->reload(m_config);
            }
            m_isDirty           = false;
            m_canSaveInHardDisk = true;
        }

        ImGui::PushEnabled(m_canSaveInHardDisk);

        if (ImGui::Button("Apply"))
        {
            writeTextureFile(m_path.c_str(), m_config);
            m_canSaveInHardDisk = false;
        }

        ImGui::PopEnabled();
        context.endProperty();
    }

} // End of namespace GPE