#include "Editor/ProjectContent.hpp"

#include "Engine/Serialization/ShaderImporterSetting.hpp"

#include "Engine/Resources/Importer/Importer.hpp"
#include "Engine/Serialization/FileExplorer.hpp"

#include <Imgui/imgui.h>
#include <string>

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Tools/Hash.hpp"
#include "Engine/Engine.hpp"

using namespace Editor;
using namespace GPE;

ProjectContent::ProjectContent()
{
    resourcesTree.name = RESOURCES_DIR;
    resourcesTree.path = std::filesystem::current_path() / RESOURCES_DIR;

    if (!std::filesystem::exists(resourcesTree.path))
    {
        FUNCT_ERROR((std::string("Path \"") + resourcesTree.path.string() + "\" doesn't exist").c_str());
        return;
    }

    refreshResourcesList();
    pCurrentDirectory = &resourcesTree;
}

static std::string doTab(int num)
{
    std::string str;

    for (size_t i = 0; i < num; ++i)
    {
        str += '\t';
    }
    return str;
}

static void explore(DirectoryInfo dir, int tab = 0)
{
    GPE::Log::getInstance()->log(doTab(tab) + dir.name.string() +
                                 " - Directories : " + std::to_string(dir.directories.size()) +
                                 " - Files : " + std::to_string(dir.files.size()));

    for (auto&& file : dir.files)
    {
        GPE::Log::getInstance()->log(doTab(tab + 1) + file.filename.string() + " - " + file.extention.string() + " - " +
                                     std::to_string(file.size) + "bytes ");
    }

    for (auto&& direct : dir.directories)
    {
        explore(direct, tab + 1);
    }
}

void ProjectContent::refreshResourcesList()
{
    resourcesTree.files.clear();
    resourcesTree.directories.clear();

    std::filesystem::path path = std::filesystem::current_path() / RESOURCES_DIR;

    DirectoryInfo* pCurrentNode = &resourcesTree;
    pCurrentDirectory           = pCurrentNode;

    for (std::filesystem::recursive_directory_iterator next(path), end; next != end; ++next)
    {
        if (next->is_directory())
        {
            if (pCurrentNode->pParent != nullptr)
            {
                while (next->path().parent_path() != pCurrentNode->path)
                {
                    pCurrentNode = pCurrentNode->pParent;
                }
            }

            pCurrentNode =
                &pCurrentNode->directories.emplace_back(DirectoryInfo{next->path().stem(), pCurrentNode, next->path()});
        }
        else if (next->is_regular_file())
        {
            if (pCurrentNode->pParent != nullptr)
            {
                while (next->path().parent_path() != pCurrentNode->path)
                {
                    pCurrentNode = pCurrentNode->pParent;
                }
            }
            pCurrentNode->files.emplace_back(
                FileInfo{next->path(), next->path().filename(), next->path().extension(), next->file_size()});
        }
    }

    explore(resourcesTree);
}

static void renderMaterial(ImVec2& size)
{
    ImGuiIO&    io        = ImGui::GetIO();
    ImTextureID my_tex_id = io.Fonts->TexID;
    float       my_tex_w  = (float)io.Fonts->TexWidth;
    float       my_tex_h  = (float)io.Fonts->TexHeight;

    // -1 == uses default padding (style.FramePadding)
    ImVec2 uv0      = ImVec2(0.0f, 0.0f);                         // UV coordinates for lower-left
    ImVec2 uv1      = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h); // UV coordinates for (32,32) in our texture
    ImVec4 tint_col = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 bg_col   = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Black background

    ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
}

static void renderImage(ImVec2& size)
{
    ImGuiIO&    io        = ImGui::GetIO();
    ImTextureID my_tex_id = io.Fonts->TexID;
    float       my_tex_w  = (float)io.Fonts->TexWidth;
    float       my_tex_h  = (float)io.Fonts->TexHeight;

    // -1 == uses default padding (style.FramePadding)
    ImVec2 uv0      = ImVec2(0.0f, 0.0f);                         // UV coordinates for lower-left
    ImVec2 uv1      = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h); // UV coordinates for (32,32) in our texture
    ImVec4 tint_col = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 bg_col   = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Black background

    ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
}

static void renderModel(ImVec2& size)
{
    ImGuiIO&    io        = ImGui::GetIO();
    ImTextureID my_tex_id = io.Fonts->TexID;
    float       my_tex_w  = (float)io.Fonts->TexWidth;
    float       my_tex_h  = (float)io.Fonts->TexHeight;

    // -1 == uses default padding (style.FramePadding)
    ImVec2 uv0      = ImVec2(0.0f, 0.0f);                         // UV coordinates for lower-left
    ImVec2 uv1      = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h); // UV coordinates for (32,32) in our texture
    ImVec4 tint_col = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 bg_col   = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Black background

    ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
}

static void renderUnknowFormat(ImVec2& size)
{
    ImGuiIO&    io        = ImGui::GetIO();
    ImTextureID my_tex_id = io.Fonts->TexID;
    float       my_tex_w  = (float)io.Fonts->TexWidth;
    float       my_tex_h  = (float)io.Fonts->TexHeight;

    // -1 == uses default padding (style.FramePadding)
    ImVec2 uv0      = ImVec2(0.0f, 0.0f);                         // UV coordinates for lower-left
    ImVec2 uv1      = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h); // UV coordinates for (32,32) in our texture
    ImVec4 tint_col = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
    ImVec4 bg_col   = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Black background

    ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
}

static void renderSound(ImVec2& size)
{
    ImGuiIO&    io        = ImGui::GetIO();
    ImTextureID my_tex_id = io.Fonts->TexID;
    float       my_tex_w  = (float)io.Fonts->TexWidth;
    float       my_tex_h  = (float)io.Fonts->TexHeight;

    // -1 == uses default padding (style.FramePadding)
    ImVec2 uv0      = ImVec2(0.0f, 0.0f);                         // UV coordinates for lower-left
    ImVec2 uv1      = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h); // UV coordinates for (32,32) in our texture
    ImVec4 tint_col = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
    ImVec4 bg_col   = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Black background

    ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
}

static void renderShader(ImVec2& size)
{
    ImGuiIO&    io        = ImGui::GetIO();
    ImTextureID my_tex_id = io.Fonts->TexID;
    float       my_tex_w  = (float)io.Fonts->TexWidth;
    float       my_tex_h  = (float)io.Fonts->TexHeight;

    // -1 == uses default padding (style.FramePadding)
    ImVec2 uv0      = ImVec2(0.0f, 0.0f);                         // UV coordinates for lower-left
    ImVec2 uv1      = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h); // UV coordinates for (32,32) in our texture
    ImVec4 tint_col = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    ImVec4 bg_col   = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Black background

    ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
}

static void renderfolder(ImVec2& size, DirectoryInfo** pSelectectDir, DirectoryInfo& currentDir)
{
    ImGuiIO&    io        = ImGui::GetIO();
    ImTextureID my_tex_id = io.Fonts->TexID;
    float       my_tex_w  = (float)io.Fonts->TexWidth;
    float       my_tex_h  = (float)io.Fonts->TexHeight;

    // -1 == uses default padding (style.FramePadding)
    ImVec2 uv0      = ImVec2(0.0f, 0.0f);                         // UV coordinates for lower-left
    ImVec2 uv1      = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h); // UV coordinates for (32,32) in our texture
    ImVec4 bg_col   = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

    if (ImGui::ImageButton(my_tex_id, size, uv0, uv1, 2, bg_col, tint_col))
    {
        *pSelectectDir = &currentDir;
    }
}

void ProjectContent::renderAndGetSelected(GPE::IInspectable*& selectedGameObject)
{
    float          window_visible_x2  = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    DirectoryInfo* pSelectedDirectory = pCurrentDirectory;

    // Try to reset SelectedGameObject
    if (importationSetting.get() && selectedGameObject != importationSetting.get())
    {
        importationSetting.reset();
    }

    if (ImGui::Button("Importe"))
    {
        std::string srcPath = GPE::openFileExplorer().string();
        importeModel(srcPath.c_str(), pCurrentDirectory->path.string().c_str());
    }

    ImGui::SameLine();

    if (ImGui::Button("Reload"))
    {
        refreshResourcesList();
        pSelectedDirectory = pCurrentDirectory;
    }

    ImGui::SameLine();
    if (pCurrentDirectory->pParent != nullptr)
    {
        if (ImGui::Button("Back"))
        {
            pSelectedDirectory = pCurrentDirectory->pParent;
        }
        ImGui::SameLine();
    }

    ImGui::TextUnformatted(pCurrentDirectory->path.string().c_str());

    ImVec2      size  = ImVec2(32.0f, 32.0f); // Size of the image we want to make visible
    ImGuiStyle& style = ImGui::GetStyle();

    for (int i = 0; i < pCurrentDirectory->directories.size(); ++i)
    {
        ImGui::PushID(i);

        ImGui::BeginGroup();
        {
            renderfolder(size, &pSelectedDirectory, pCurrentDirectory->directories[i]);

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

                size_t dirSize = 0;
                for (auto&& file : pCurrentDirectory->directories[i].files)
                    dirSize += file.size;

                ImGui::Text("%lu bytes", dirSize);
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }

            ImGui::TextUnformatted(pCurrentDirectory->directories[i].name.string().c_str());
        }
        ImGui::EndGroup();

        float last_button_x2 = ImGui::GetItemRectMax().x;
        float next_button_x2 =
            last_button_x2 + style.ItemSpacing.x + size.x; // Expected position if next button was on same line
        if (i + 1 < pCurrentDirectory->directories.size() && next_button_x2 < window_visible_x2)
        {
            ImGui::SameLine();
        }

        ImGui::PopID();
    }

    for (int i = 0; i < pCurrentDirectory->files.size(); ++i)
    {
        ImGui::PushID(i);

        ImGui::BeginGroup();
        {
            switch (GPE::hash(pCurrentDirectory->files[i].extention.string().c_str())) // runtime
            {
            case GPE::hash(ENGINE_MESH_EXTENSION): // compile time
                renderModel(size);
                break;

            case GPE::hash(ENGINE_MATERIAL_EXTENSION): // compile time
                renderMaterial(size);
                break;

            case GPE::hash(".wav"): // compile time
            case GPE::hash(".mp3"): // compile time
                renderSound(size);
                break;

            case GPE::hash(ENGINE_SHADER_EXTENSION): // compile time
                renderShader(size);
                break;

            case GPE::hash(ENGINE_TEXTURE_EXTENSION): // compile time
                renderImage(size);
                break;

            default:
                renderUnknowFormat(size);
                break;
            }

            // Display properties
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

                ImGui::Text("%lu bytes", pCurrentDirectory->files[i].size);
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }

            // Try to inspect
            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
            {
                switch (GPE::hash(pCurrentDirectory->files[i].extention.string().c_str())) // runtime
                {
                case GPE::hash(ENGINE_MESH_EXTENSION): // compile time

                    break;

                case GPE::hash(ENGINE_MATERIAL_EXTENSION): // compile time

                    break;

                case GPE::hash(".wav"): // compile time
                case GPE::hash(".mp3"): // compile time

                    break;

                case GPE::hash(ENGINE_SHADER_EXTENSION): // compile time
                    importationSetting =
                        std::make_unique<GPE::ShaderImporterModifier>(pCurrentDirectory->files[i].path.string());
                    selectedGameObject = importationSetting.get();
                    break;

                case GPE::hash(ENGINE_TEXTURE_EXTENSION): // compile time

                    break;

                default:
                    renderUnknowFormat(size);
                    break;
                }
            }

            // Drag
            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload(pCurrentDirectory->files[i].extention.string().c_str(),
                                          (void*)&pCurrentDirectory->files[i].path,
                                          sizeof(pCurrentDirectory->files[i].path));
                ImGui::TextUnformatted(pCurrentDirectory->files[i].filename.string().c_str());
                ImGui::EndDragDropSource();
            }

            ImGui::TextUnformatted(pCurrentDirectory->files[i].filename.string().c_str());
        }
        ImGui::EndGroup();

        float last_button_x2 = ImGui::GetItemRectMax().x;
        float next_button_x2 =
            last_button_x2 + style.ItemSpacing.x + size.x; // Expected position if next button was on same line
        if (i + 1 < pCurrentDirectory->files.size() && next_button_x2 < window_visible_x2)
        {
            ImGui::SameLine();
        }

        ImGui::PopID();
    }

    pCurrentDirectory = pSelectedDirectory;

    if (ImGui::IsWindowHovered() && ImGui::GetIO().MouseClicked[ImGuiMouseButton_Right])
    {
        ImGui::OpenPopup("ProjectContentPopup");
    }

    if (ImGui::BeginPopup("ProjectContentPopup"))
    {
        if (ImGui::BeginMenu("Create"))
        {
            if (ImGui::MenuItem("Shader"))
            {
                std::filesystem::path shaderDir  = pCurrentDirectory->path;
                std::filesystem::path shaderName = "NewShader" ENGINE_SHADER_EXTENSION;

                int id = 0;
                while (pCurrentDirectory->containFile(shaderName))
                {
                    shaderName = stringFormat("NewShader(%i)" ENGINE_SHADER_EXTENSION, ++id);
                }

                shaderDir /= shaderName;

                writeShaderFile(shaderDir.string().c_str());
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }
}