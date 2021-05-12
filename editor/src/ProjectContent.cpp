#include <Editor/ProjectContent.hpp>

#include <Editor/Editor.hpp>

#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Core/HotReload/ReloadableCpp.hpp>
#include <Engine/Core/Tools/Hash.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Serialization/IInspectable.hpp>
#include <Engine/Serialization/MaterialInspectorPanel.hpp>
#include <Engine/Serialization/MeshInspectorPanel.hpp>
#include <Engine/Serialization/ShaderInspectorPanel.hpp>
#include <Engine/Serialization/FileExplorer.hpp>
#include <Engine/Serialization/TextureImporterSetting.hpp>

// Don't move up
#include "Engine/Core/HotReload/SingletonsSync.hpp"

#include <imgui/imgui.h>
#include <string>

using namespace Editor;
using namespace GPE;

ProjectContent::ProjectContent(Editor& editorContext)
    : m_folderIcone  {{"..\\..\\editor\\resources\\icone\\folder.png", Texture::ETextureMinFilter::LINEAR,
                       Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                       Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      m_textureIcone {{"..\\..\\editor\\resources\\icone\\texture.png", Texture::ETextureMinFilter::LINEAR,
                       Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                       Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      m_materialIcone{{"..\\..\\editor\\resources\\icone\\material.png", Texture::ETextureMinFilter::LINEAR,
                       Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                       Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      m_sceneIcone   {{"..\\..\\editor\\resources\\icone\\scene.png", Texture::ETextureMinFilter::LINEAR,
                       Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                       Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      m_meshIcone    {{"..\\..\\editor\\resources\\icone\\mesh.png", Texture::ETextureMinFilter::LINEAR,
                       Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                       Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      m_shaderIcone  {{"..\\..\\editor\\resources\\icone\\shader.jpg", Texture::ETextureMinFilter::LINEAR,
                       Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                       Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      m_soundIcone   {{"..\\..\\editor\\resources\\icone\\sound.jpg", Texture::ETextureMinFilter::LINEAR,
                       Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                       Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      m_prefabIcone  {{"..\\..\\editor\\resources\\icone\\prefab.png", Texture::ETextureMinFilter::LINEAR,
                       Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                       Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      m_unknowIcone  {{"..\\..\\editor\\resources\\icone\\unknow.png", Texture::ETextureMinFilter::LINEAR,
                       Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrapS::CLAMP_TO_EDGE,
                       Texture::ETextureWrapT::CLAMP_TO_EDGE, false, false}},
      m_editorContext{&editorContext}
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

static bool renderIcone(const ImVec2& size, const ImVec4& tint_col = ImVec4(0.0f, 1.0f, 1.0f, 1.0f),
                        const ImVec4& bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f))
{
    ImGuiIO&    io        = ImGui::GetIO();
    ImTextureID my_tex_id = io.Fonts->TexID;
    float       my_tex_w  = (float)io.Fonts->TexWidth;
    float       my_tex_h  = (float)io.Fonts->TexHeight;

    // -1 == uses default padding (style.FramePadding)
    ImVec2 uv0 = ImVec2(0.0f, 0.0f);                         // UV coordinates for lower-left
    ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h); // UV coordinates for (32,32) in our texture

    return ImGui::ImageButton(my_tex_id, size, uv0, uv1, 1, bg_col, tint_col);
}

void ProjectContent::renderAndGetSelected(GPE::IInspectable*& selectedGameObject)
{
    float          windowVisibleX2    = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    DirectoryInfo* pSelectedDirectory = pCurrentDirectory;

    // Try to reset SelectedGameObject
    if (importationSetting.get() && selectedGameObject != importationSetting.get())
    {
        importationSetting.reset();
    }

    if (ImGui::Button("Importe"))
    {
        const std::filesystem::path srcPath =
            GPE::openFileExplorerAndGetAbsoluePath(L"Select asset to import",
                                                   {{L"Asset", L"*.obj;*.fbx;*.jpg;*.jpeg;*.bmp;*.png;*.tga"},
                                                    {L"Model", L"*.obj;*.fbx"},
                                                    {L"Image", L"*.jpg;*.jpeg;*.bmp;*.png;*.tga"}})
                .string();

        if (!srcPath.empty())
        {
            switch (hash(srcPath.extension().string().c_str()))
            {

            case hash(".obj"):
            case hash(".fbx"):
                importeModel(srcPath.string().c_str(), pCurrentDirectory->path.string().c_str());
                break;

            case hash(".jpg"):
            case hash(".jpeg"):
            case hash(".png"):
            case hash(".bmp"):
            case hash(".tga"): {
                importationSetting = std::make_unique<GPE::TextureImporterSetting>(
                    srcPath.string().c_str(), pCurrentDirectory->path.string().c_str());
                selectedGameObject = importationSetting.get();
                break;
            }

            default:
                Log::getInstance()->logWarning(
                    stringFormat("Missing implementation to importe file \"%s\"", srcPath.string()));
                break;
            }
        }
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

    for (auto&& it = pCurrentDirectory->directories.begin(); it != pCurrentDirectory->directories.end(); ++it)
    {
        ImGui::PushID(&*it);

        ImGui::BeginGroup();
        {
            float posX = ImGui::GetCursorPosX() + ImGui::CalcTextSize(it->name.string().c_str()).x / 2.f -
                         2 * ImGui::GetStyle().ItemSpacing.x;
            ImGui::SetCursorPosX(posX);

            if (ImGui::ImageButton((void*)(intptr_t)m_folderIcone.getID(), size))
            {
                pSelectedDirectory = &*it;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

                size_t dirSize = 0;
                for (auto&& file : it->files)
                    dirSize += file.size;

                ImGui::Text("%lu bytes", dirSize);
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }

            ImGui::TextUnformatted(it->name.string().c_str());
        }
        ImGui::EndGroup();

        float last_button_x2 = ImGui::GetItemRectMax().x;
        float next_button_x2 =
            last_button_x2 + style.ItemSpacing.x + size.x; // Expected position if next button was on same line

        if (++it != pCurrentDirectory->directories.cend() && next_button_x2 < windowVisibleX2)
        {
            ImGui::SameLine();
        }
        --it; // decrement because incremented tyo check value + N above

        ImGui::PopID();
    }

    for (auto&& it = pCurrentDirectory->files.cbegin(); it != pCurrentDirectory->files.cend(); ++it)
    {
        ImGui::PushID(&*it);

        ImGui::BeginGroup();
        {
            float posX = ImGui::GetCursorPosX() + ImGui::CalcTextSize(it->filename.string().c_str()).x / 2.f -
                         2 * ImGui::GetStyle().ItemSpacing.x;
            ImGui::SetCursorPosX(posX);

            switch (GPE::hash(it->extention.string().c_str())) // runtime
            {
            case GPE::hash(ENGINE_MESH_EXTENSION): // compile time
                ImGui::ImageButton((void*)(intptr_t)m_meshIcone.getID(), size);
                break;

            case GPE::hash(ENGINE_MATERIAL_EXTENSION): // compile time
                ImGui::ImageButton((void*)(intptr_t)m_materialIcone.getID(), size);
                break;

            case GPE::hash(".wav"): // compile time
            case GPE::hash(".mp3"): // compile time
                ImGui::ImageButton((void*)(intptr_t)m_soundIcone.getID(), size);
                break;

            case GPE::hash(ENGINE_SHADER_EXTENSION): // compile time
                ImGui::ImageButton((void*)(intptr_t)m_shaderIcone.getID(), size);
                break;

            case GPE::hash(ENGINE_TEXTURE_EXTENSION): // compile time
                ImGui::ImageButton((void*)(intptr_t)m_textureIcone.getID(), size);
                break;

            case GPE::hash(ENGINE_SCENE_EXTENSION): // compile time
                ImGui::ImageButton((void*)(intptr_t)m_sceneIcone.getID(), size);
                break;

            case GPE::hash(ENGINE_PREFAB_EXTENSION): // compile time
                ImGui::ImageButton((void*)(intptr_t)m_prefabIcone.getID(), size);
                break;

            default:
                ImGui::ImageButton((void*)(intptr_t)m_unknowIcone.getID(), size);
                break;
            }

            // Drag
            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload(it->extention.string().c_str(), (void*)&it->path, sizeof(it->path));
                ImGui::TextUnformatted(it->filename.string().c_str());
                ImGui::EndDragDropSource();
            }

            // Display properties
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

                ImGui::Text("%lu bytes", it->size);
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }

            // On double left clic
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
            {
                switch (GPE::hash(it->extention.string().c_str())) // runtime
                {
                case GPE::hash(ENGINE_SCENE_EXTENSION): // compile time
                {
                    std::string sceneName = it->filename.stem().string();
                    Scene&      scene     = Engine::getInstance()->sceneManager.addEmpty(sceneName);
                    m_editorContext->loadScene(&scene, it->path.string().c_str());
                    Engine::getInstance()->sceneManager.loadScene(sceneName);
                    break;
                }

                default:
                    break;
                }
            }

            // On left clic
            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
            {
                switch (GPE::hash(it->extention.string().c_str())) // runtime
                {
                case GPE::hash(ENGINE_MESH_EXTENSION): // compile time
                    importationSetting = std::make_unique<GPE::MeshInspectorPanel>(it->path.string());
                    selectedGameObject = importationSetting.get();
                    break;

                case GPE::hash(ENGINE_MATERIAL_EXTENSION): // compile time
                    importationSetting = std::make_unique<GPE::MaterialInspectorPanel>(it->path.string());
                    selectedGameObject = importationSetting.get();
                    break;

                case GPE::hash(".wav"): // compile time
                case GPE::hash(".mp3"): // compile time

                    break;

                case GPE::hash(ENGINE_SHADER_EXTENSION): // compile time
                    importationSetting = std::make_unique<GPE::ShaderInspectorPanel>(it->path.string());
                    selectedGameObject = importationSetting.get();
                    break;

                case GPE::hash(ENGINE_TEXTURE_EXTENSION): // compile time
                    break;

                case GPE::hash(ENGINE_SCENE_EXTENSION): // compile time
                    break;

                case GPE::hash(ENGINE_PREFAB_EXTENSION): // compile time
                    break;

                default:
                    break;
                }
            }

            ImGui::TextUnformatted(it->filename.string().c_str());
        }
        ImGui::EndGroup();

        float lastButtonX2 = ImGui::GetItemRectMax().x;
        float nextButtonX2 =
            lastButtonX2 + style.ItemSpacing.x + size.x * 2; // Expected position if next button was on same line

        if (++it != pCurrentDirectory->files.cend() && nextButtonX2 < windowVisibleX2)
        {
            ImGui::SameLine();
        }
        --it; // decrement because incremented tyo check value + N above

        ImGui::PopID();
    }

    pCurrentDirectory = pSelectedDirectory;

    if (!ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered() && ImGui::GetIO().MouseClicked[ImGuiMouseButton_Right])
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

            if (ImGui::MenuItem("Material"))
            {
                std::filesystem::path materialDir  = pCurrentDirectory->path;
                std::filesystem::path materialName = "NewMaterial" ENGINE_MATERIAL_EXTENSION;

                int id = 0;
                while (pCurrentDirectory->containFile(materialName))
                {
                    materialName = stringFormat("NewMaterial(%i)" ENGINE_MATERIAL_EXTENSION, ++id);
                }

                materialDir /= materialName;

                writeMaterialFile(materialDir.string().c_str());
            }

            if (ImGui::MenuItem("Scene"))
            {
                std::filesystem::path sceneDir  = pCurrentDirectory->path;
                std::filesystem::path sceneName = "NewScene" ENGINE_SCENE_EXTENSION;

                int id = 0;
                while (pCurrentDirectory->containFile(sceneName))
                {
                    sceneName = stringFormat("NewScene(%i)" ENGINE_SCENE_EXTENSION, ++id);
                }

                sceneDir /= sceneName;

                Scene& scene = Engine::getInstance()->sceneManager.addEmpty(sceneName.stem().string().c_str());
                m_editorContext->saveScene(&scene, sceneDir.string().c_str());
            }

            if (ImGui::MenuItem("Prefab"))
            {
                std::filesystem::path prefabDir  = pCurrentDirectory->path;
                std::filesystem::path prefabName = "NewPrefab" ENGINE_PREFAB_EXTENSION;

                int id = 0;
                while (pCurrentDirectory->containFile(prefabName))
                {
                    prefabName = stringFormat("NewPrefab(%i)" ENGINE_PREFAB_EXTENSION, ++id);
                }

                prefabDir /= prefabName;

                Scene prefab;
                auto  saveFunc = GET_PROCESS((*m_editorContext->m_reloadableCpp), saveSceneToPath);
                saveFunc(&prefab, prefabDir.string().c_str(), GPE::SavedScene::EType::XML);
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }
}