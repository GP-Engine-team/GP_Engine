#include <Editor/ProjectContent.hpp>

#include <Editor/Editor.hpp>

#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Core/HotReload/ReloadableCpp.hpp>
#include <Engine/Core/Tools/Hash.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Serialization/FileExplorer.hpp>
#include <Engine/Serialization/IInspectable.hpp>
#include <Engine/Serialization/MaterialInspectorPanel.hpp>
#include <Engine/Serialization/MeshInspectorPanel.hpp>
#include <Engine/Serialization/ShaderInspectorPanel.hpp>
#include <Engine/Serialization/TextureImporterSetting.hpp>
#include <Engine/Serialization/TextureInspectorPanel.hpp>

#include <Engine/Core/HotReload/ReloadableCpp.hpp>
#include <Engine/Core/HotReload/SingletonsSync.hpp>

// Don't move up
#include "Engine/Core/HotReload/SingletonsSync.hpp"

#include <imgui/imgui.h>
#include <string>

using namespace Editor;
using namespace GPE;

ProjectContent::ProjectContent(Editor& editorContext)
    : m_folderIcone{{"..\\..\\editor\\resources\\icone\\folder.png", Texture::ETextureMinFilter::LINEAR,
                     Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                     Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
      m_textureIcone{{"..\\..\\editor\\resources\\icone\\texture.png", Texture::ETextureMinFilter::LINEAR,
                      Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                      Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
      m_materialIcone{{"..\\..\\editor\\resources\\icone\\material.png", Texture::ETextureMinFilter::LINEAR,
                       Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                       Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
      m_sceneIcone{{"..\\..\\editor\\resources\\icone\\scene.png", Texture::ETextureMinFilter::LINEAR,
                    Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                    Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
      m_meshIcone{{"..\\..\\editor\\resources\\icone\\mesh.png", Texture::ETextureMinFilter::LINEAR,
                   Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                   Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
      m_shaderIcone{{"..\\..\\editor\\resources\\icone\\shader.jpg", Texture::ETextureMinFilter::LINEAR,
                     Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                     Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
      m_soundIcone{{"..\\..\\editor\\resources\\icone\\sound.jpg", Texture::ETextureMinFilter::LINEAR,
                    Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                    Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
      m_prefabIcone{{"..\\..\\editor\\resources\\icone\\prefab.png", Texture::ETextureMinFilter::LINEAR,
                     Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                     Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
      m_skeletonIcon{{"..\\..\\editor\\resources\\icone\\skeleton.png", Texture::ETextureMinFilter::LINEAR,
                      Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                      Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
      m_animationIcon{{"..\\..\\editor\\resources\\icone\\animation.png", Texture::ETextureMinFilter::LINEAR,
                       Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                       Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
      m_skinIcon{{"..\\..\\editor\\resources\\icone\\skin.png", Texture::ETextureMinFilter::LINEAR,
                  Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                  Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
      m_unknowIcone{{"..\\..\\editor\\resources\\icone\\unknow.png", Texture::ETextureMinFilter::LINEAR,
                     Texture::ETextureMagFilter::LINEAR, Texture::ETextureWrap::CLAMP_TO_EDGE,
                     Texture::ETextureWrap::CLAMP_TO_EDGE, false, false}},
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

static std::string memorySizeToString(std::uintmax_t size)
{
    std::string rst;
    int         i{};
    double      mantissa = size;
    for (; mantissa >= 1024.; mantissa /= 1024., ++i)
    {
    }
    mantissa = std::ceil(mantissa * 10.) / 10.;
    rst += std::to_string(static_cast<size_t>(mantissa)) + ' ' + "BKMGTPE"[i];
    return i == 0 ? rst : rst + "B";
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

void ProjectContent::tryToSetCurrentCirToPreviousLocation(const std::filesystem::path& previousRelatifPath)
{
    DirectoryInfo* pNode = &resourcesTree;

    for (auto&& str : previousRelatifPath)
    {
        bool isFound = false;
        for (auto&& folder : pNode->directories)
        {
            if (folder.name == str)
            {
                pNode   = &folder;
                isFound = true;
                break;
            }
        }

        if (!isFound)
            break;
    }
    pCurrentDirectory = pNode;
}

void createResroucePath(DirectoryInfo& base, std::vector<ResourcesPath>& rp)
{
    for (auto&& dir : base.directories)
    {
        createResroucePath(dir, rp);
    }

    for (auto&& file : base.files)
    {
        rp.emplace_back(ResourcesPath{file.path});
    }
}

void ProjectContent::refreshResourcesList()
{
    std::filesystem::path path = std::filesystem::current_path() / RESOURCES_DIR;
    std::filesystem::path previousRelatifPath =
        std::filesystem::relative(pCurrentDirectory ? pCurrentDirectory->path : path, path).relative_path();

    resourcesTree.files.clear();
    resourcesTree.directories.clear();

    DirectoryInfo* pCurrentNode = &resourcesTree;

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
            pCurrentNode->files.emplace_back(FileInfo{std::filesystem::relative(next->path()), next->path().filename(),
                                                      next->path().extension(), next->file_size()});
        }
    }

    std::vector<ResourcesPath> pathContainer;
    createResroucePath(resourcesTree, pathContainer);

    GPE::Engine::getInstance()->resourceManager.clear<std::vector<GPE::ResourcesPath>>();
    GPE::Engine::getInstance()->resourceManager.add<std::vector<GPE::ResourcesPath>>("Path", std::move(pathContainer));

    explore(resourcesTree);
    tryToSetCurrentCirToPreviousLocation(previousRelatifPath);
}

void ProjectContent::createNewScene()
{
    std::filesystem::path sceneDir  = pCurrentDirectory->path;
    std::filesystem::path sceneName = "NewScene" ENGINE_SCENE_EXTENSION;

    int id = 0;
    while (pCurrentDirectory->containFile(sceneName))
    {
        sceneName = stringFormat("NewScene(%i)" ENGINE_SCENE_EXTENSION, ++id);
    }

    sceneDir /= sceneName;

    m_editorContext->sceneEditor.view.unbindScene();
    Scene& scene = Engine::getInstance()->sceneManager.setCurrentScene(sceneName.string());
    m_editorContext->saveScene(&scene, sceneDir.string().c_str());
    m_editorContext->sceneEditor.view.bindScene(scene);
    refreshResourcesList();
    m_editorContext->saveFolder = sceneDir.parent_path().string().c_str();
}

void ProjectContent::removeFile(const std::filesystem::path& path)
{
    recycleFileOrDirectory(path);
    refreshResourcesList();
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

    if (ImGui::Button("Import"))
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
            case hash(".OBJ"):
            case hash(".fbx"):
            case hash(".FBX"):
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

    // Drop prefab to save as prefab

    ImGui::BeginChild("DropRegion");

    ImVec2                size  = ImVec2(32.0f, 32.0f); // Size of the image we want to make visible
    ImGuiStyle&           style = ImGui::GetStyle();
    std::filesystem::path pathToRemovePath;

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

            // On double left clic
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
            {
                ImGui::OpenPopup("FileContext");
            }

            if (ImGui::BeginPopup("FileContext"))
            {
                ImGui::Text(it->path.stem().string().c_str());

                if (ImGui::MenuItem("Delete"))
                {
                    pathToRemovePath = it->path;
                }

                if (ImGui::MenuItem("Rename"))
                {
                    it->isInRenameMode = true;
                }

                ImGui::EndPopup();
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

                ImGui::TextUnformatted(memorySizeToString(it->computeSize()).c_str());
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }

            if (it->isInRenameMode)
            {
                ImGui::SetNextItemWidth(size.x * 2);
                constexpr size_t bufferSize = 256;
                char             buffer[bufferSize];
                strcpy_s(buffer, it->name.stem().string().c_str());
                if (ImGui::InputText("", buffer, bufferSize, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    std::filesystem::path oldPath = it->path;
                    it->name.replace_filename(buffer);
                    it->path.replace_filename(buffer);
                    std::filesystem::rename(oldPath, it->path);
                    it->isInRenameMode = false;
                }
            }
            else
            {
                ImGui::TextUnformatted(it->name.string().c_str());
            }
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

    for (auto&& it = pCurrentDirectory->files.begin(); it != pCurrentDirectory->files.end(); ++it)
    {
        ImGui::PushID(&*it);

        ImGui::BeginGroup();
        {
            float posX = ImGui::GetCursorPosX() + ImGui::CalcTextSize(it->filename.string().c_str()).x / 2.f -
                         2 * ImGui::GetStyle().ItemSpacing.x;
            ImGui::SetCursorPosX(posX);

            // Different strings can have the same hash,
            // but since the string is short, we can consider it doesn't happen.
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

            case GPE::hash(ENGINE_SKELETON_EXTENSION): // compile time
                ImGui::ImageButton((void*)(intptr_t)m_skeletonIcon.getID(), size);
                break;

            case GPE::hash(ENGINE_ANIMATION_EXTENSION): // compile time
                ImGui::ImageButton((void*)(intptr_t)m_animationIcon.getID(), size);
                break;

            case GPE::hash(ENGINE_SKIN_EXTENSION): // compile time
                ImGui::ImageButton((void*)(intptr_t)m_skinIcon.getID(), size);
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

                ImGui::TextUnformatted(memorySizeToString(it->size).c_str());
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }

            // On double left clic
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
            {
                ImGui::OpenPopup("FileContext");
            }

            if (ImGui::BeginPopup("FileContext"))
            {
                ImGui::Text(it->path.stem().string().c_str());

                if (ImGui::MenuItem("Delete"))
                {
                    pathToRemovePath = it->path;
                }

                if (ImGui::MenuItem("Rename"))
                {
                    it->isInRenameMode = true;
                }

                ImGui::EndPopup();
            }

            // On double left clic
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
            {
                switch (GPE::hash(it->extention.string().c_str())) // runtime
                {
                case GPE::hash(ENGINE_SCENE_EXTENSION): // compile time
                {
                    m_editorContext->sceneEditor.view.unbindScene();
                    Scene& scene = Engine::getInstance()->sceneManager.setCurrentScene(it->path.string());
                    m_editorContext->loadScene(&scene, it->path.string().c_str());
                    scene.setName(it->filename.stem().string().c_str());
                    m_editorContext->saveFolder = it->path.parent_path().string();
                    break;
                }

                default:
                    break;
                }
            }

            // On left click
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

                case GPE::hash(ENGINE_SHADER_EXTENSION): // compile time
                    importationSetting = std::make_unique<GPE::ShaderInspectorPanel>(it->path.string());
                    selectedGameObject = importationSetting.get();
                    break;

                case GPE::hash(ENGINE_TEXTURE_EXTENSION): // compile time
                    importationSetting = std::make_unique<GPE::TextureInspectorPanel>(it->path.string());
                    selectedGameObject = importationSetting.get();
                    break;

                case GPE::hash(ENGINE_SCENE_EXTENSION): // compile time
                    break;

                case GPE::hash(ENGINE_PREFAB_EXTENSION): // compile time
                    break;

                case GPE::hash(ENGINE_SKELETON_EXTENSION):  // compile time
                case GPE::hash(ENGINE_ANIMATION_EXTENSION): // compile time
                case GPE::hash(ENGINE_SKIN_EXTENSION):      // compile time
                    break;

                default:
                    break;
                }
            }

            if (it->isInRenameMode)
            {
                ImGui::SetNextItemWidth(size.x * 2);
                constexpr size_t bufferSize = 256;
                char             buffer[bufferSize];
                strcpy_s(buffer, it->filename.stem().string().c_str());
                if (ImGui::InputText("", buffer, bufferSize, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    std::filesystem::path oldPath = it->path;
                    it->filename.replace_filename(buffer);
                    it->filename.replace_extension(it->extention);
                    it->path.replace_filename(buffer);
                    it->path.replace_extension(it->extention);
                    std::filesystem::rename(oldPath, it->path);
                    it->isInRenameMode = false;
                }
                ImGui::SameLine();
                ImGui::TextUnformatted(it->extention.string().c_str());
            }
            else
            {
                ImGui::TextUnformatted(it->filename.string().c_str());
            }
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
            if (ImGui::MenuItem("Directory"))
            {
                std::filesystem::path dir  = pCurrentDirectory->path;
                std::filesystem::path name = "NewDirectory";

                int id = 0;
                while (pCurrentDirectory->containDirectory(name))
                {
                    name = stringFormat("NewDirectory(%i)" ENGINE_SHADER_EXTENSION, ++id);
                }

                dir /= name;
                std::filesystem::create_directory(dir);
                refreshResourcesList();
            }

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
                refreshResourcesList();
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
                refreshResourcesList();
            }

            if (ImGui::MenuItem("Scene"))
            {
                createNewScene();
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
                auto  saveFunc = GET_PROCESS((*m_editorContext->reloadableCpp), saveSceneToPath);
                saveFunc(&prefab, prefabDir.string().c_str(), GPE::SavedScene::EType::XML);
                refreshResourcesList();
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }

    if (!pathToRemovePath.empty())
    {
        removeFile(pathToRemovePath);
    }

    // End drop region
    ImGui::EndChild();
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_GAMEOBJECT"))
        {
            IM_ASSERT(payload->DataSize == sizeof(GameObject*));

            GameObject&                 gameObject = **static_cast<GPE::GameObject**>(payload->Data);
            const std::filesystem::path path =
                pCurrentDirectory->path / (gameObject.getName() + ENGINE_PREFAB_EXTENSION);

            auto saveFunc = GET_PROCESS((*m_editorContext->reloadableCpp), savePrefabToPath);
            saveFunc(gameObject, path.string().c_str(), GPE::SavedScene::EType::XML);

            if (SharedPrefab* pref =
                    Engine::getInstance()->resourceManager.get<SharedPrefab>(std::filesystem::relative(path).string()))
            {
                pref->pref.loadPrefabFromPath(path.string().c_str());
            }

            refreshResourcesList();

            // deferedSetParent.bind(**static_cast<GPE::GameObject**>(payload->Data), gameObject);
        }
        ImGui::EndDragDropTarget();
    }
}