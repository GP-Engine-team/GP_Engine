#include <Engine/ECS/Component/Model.hpp>

File_GENERATED

#include <Engine/Core/Debug/Assert.hpp>
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Core/Tools/Hash.hpp>
#include <Engine/ECS/System/RenderSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Material.hpp>
#include <Engine/Resources/Mesh.hpp>
#include <Engine/Resources/ResourcesManagerType.hpp>
#include <Engine/Resources/Scene.hpp>
#include <Engine/Resources/Shader.hpp>
#include <Engine/Resources/Texture.hpp>
#include <GPM/Matrix3.hpp>
#include <GPM/Matrix4.hpp>
#include <GPM/Shape3D/Sphere.hpp>

#include <filesystem>
#include <imgui.h>

    using namespace GPE;
using namespace GPM;

bool GPE::isSubModelHasPriorityOverAnother(const SubModel* lhs, const SubModel* rhs) noexcept
{
    return lhs->pShader->getID() < rhs->pShader->getID() || lhs->pMesh->getID() < rhs->pMesh->getID() ||
           lhs->pMaterial->getID() < rhs->pMaterial->getID() ||
           lhs->pMaterial->getDiffuseTexture()->getID() < rhs->pMaterial->getDiffuseTexture()->getID() ||
           (lhs->pMaterial->isOpaque() && !rhs->pMaterial->isOpaque());
}

Model::Model(Model&& other) noexcept : Component(other.getOwner()), m_subModels{other.m_subModels}
{
    auto&& itNew = m_subModels.begin();
    auto&& itOld = other.m_subModels.begin();
    for (; itNew != m_subModels.end(); ++itNew, ++itOld)
    {
        getOwner().pOwnerScene->sceneRenderer.updateSubModelPointer(&(*itNew), &(*itOld));
    }
}

Model::~Model()
{
    for (SubModel& subMesh : m_subModels)
        getOwner().pOwnerScene->sceneRenderer.removeSubModel(subMesh);
}

Model::Model(GameObject& owner) : Model(owner, CreateArg{})
{
}

Model::Model(GameObject& owner, const CreateArg& arg) : Component{owner}, m_subModels{arg.subModels}
{
    for (SubModel& subMesh : m_subModels)
    {
        subMesh.pModel = this;
        getOwner().pOwnerScene->sceneRenderer.addSubModel(subMesh);
    }
}

Model& Model::operator=(Model&& other) noexcept
{
    m_subModels = other.m_subModels;

    auto&& itNew = m_subModels.begin();
    auto&& itOld = other.m_subModels.begin();
    for (; itNew != m_subModels.end(); ++itNew, ++itOld)
    {
        getOwner().pOwnerScene->sceneRenderer.updateSubModelPointer(&(*itNew), &(*itOld));
    }

    return static_cast<Model&>(Component::operator=(std::move(other)));
}

void Model::awake()
{
    setActive(false);
    setActive(true);
}

void Model::moveTowardScene(class Scene& newOwner)
{
    for (SubModel& subMesh : m_subModels)
        getOwner().pOwnerScene->sceneRenderer.removeSubModel(subMesh);

    for (SubModel& subMesh : m_subModels)
    {
        subMesh.pModel = this;
        newOwner.sceneRenderer.addSubModel(subMesh);
    }
}

template <typename T>
void renderResourceExplorer(const char* name, T*& inRes)
{
    auto& resourceContainer = GPE::Engine::getInstance()->resourceManager.getAll<T>();

    std::vector<const char*> items;
    items.reserve(resourceContainer.size());

    for (auto&& res : resourceContainer)
    {
        items.emplace_back(res.first.c_str());
    }

    // Init position of the combo box cursor
    int itemCurrent;
    if (inRes == nullptr)
    {
        itemCurrent = -1;
    }
    else
    {
        itemCurrent = 0;
        for (auto&& it = resourceContainer.begin(); &it->second != inRes; ++itemCurrent, ++it)
            ;
    }

    if (ImGui::Combo(name, &itemCurrent, items.data(), static_cast<int>(items.size())))
    {
        auto&& it = resourceContainer.begin();
        for (int i = 0; i < itemCurrent; ++i, ++it)
            ;

        inRes = &it->second;
    }
}

template <>
void GPE::save(XmlSaver & context, const SubModel& inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    GPE::save(context, inspected.pModel, XmlSaver::SaveInfo{"pModel", "Model*", 0});

    if (inspected.pShader != nullptr)
    {
        if (const std::string* shaderName = GPE::Engine::getInstance()->resourceManager.getKey(inspected.pShader))
        {
            GPE::save(context, *shaderName, XmlSaver::SaveInfo{"pShader", "Shader*", 0});
        }
    }

    if (inspected.pMaterial != nullptr)
    {
        if (const std::string* matName = GPE::Engine::getInstance()->resourceManager.getKey(inspected.pMaterial))
        {
            GPE::save(context, *matName, XmlSaver::SaveInfo{"pMaterial", "Material*", 0});
        }
    }

    if (inspected.pMesh != nullptr)
    {
        if (const std::string* meshName = GPE::Engine::getInstance()->resourceManager.getKey(inspected.pMesh))
        {
            GPE::save(context, *meshName, XmlSaver::SaveInfo{"pMesh", "Mesh*", 0});
        }
    }

    GPE::save(context, inspected.enableBackFaceCulling, XmlSaver::SaveInfo{"enableBackFaceCulling", "bool", 0});

    context.pop();
}

template <>
void GPE::load(XmlLoader & context, SubModel & inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        GPE::load(context, inspected.pModel, XmlLoader::LoadInfo{"pModel", "Model*", 0});

        {
            std::string shaderName;
            GPE::load(context, shaderName, XmlLoader::LoadInfo{"pShader", "Shader*", 0});
            inspected.pShader = Engine::getInstance()->resourceManager.get<GPE::Shader>(shaderName);
        }

        {
            std::string matName;
            GPE::load(context, matName, XmlLoader::LoadInfo{"pMaterial", "Material*", 0});
            inspected.pMaterial = Engine::getInstance()->resourceManager.get<GPE::Material>(matName);
        }

        {
            std::string meshName;
            GPE::load(context, meshName, XmlLoader::LoadInfo{"pMesh", "Mesh*", 0});
            inspected.pMesh = Engine::getInstance()->resourceManager.get<GPE::Mesh>(meshName);
        }

        GPE::load(context, inspected.enableBackFaceCulling, XmlLoader::LoadInfo{"enableBackFaceCulling", "bool", 0});

        context.pop();
    }
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, SubModel& inspected)
{
    const bool isPreviousElementVoid =
        !((size_t)inspected.pMesh & (size_t)inspected.pShader & (size_t)inspected.pMaterial);

    renderResourceExplorer<Mesh>("Mesh", inspected.pMesh);
    // Drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_MESH_EXTENSION))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

            if (Mesh* pMesh = Engine::getInstance()->resourceManager.get<Mesh>(path.string().c_str()))
            {
                inspected.pMesh = pMesh;
            }
            else
            {
                if (const std::string* str = Engine::getInstance()->resourceManager.getKey(inspected.pMesh))
                    inspected.pModel->getOwner().pOwnerScene->removeLoadedResourcePath(str->c_str());

                inspected.pMesh = loadMeshFile(path.string().c_str());
                inspected.pModel->getOwner().pOwnerScene->addLoadedResourcePath(path.string().c_str());
            }
        }
    }

    renderResourceExplorer<Shader>("Shader", inspected.pShader);

    // Drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_SHADER_EXTENSION))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

            if (Shader* pShader = Engine::getInstance()->resourceManager.get<Shader>(path.string().c_str()))
            {
                inspected.pShader = pShader;
            }
            else
            {
                if (const std::string* str = Engine::getInstance()->resourceManager.getKey(inspected.pShader))
                    inspected.pModel->getOwner().pOwnerScene->removeLoadedResourcePath(str->c_str());

                inspected.pShader = loadShaderFile(path.string().c_str());
                inspected.pModel->getOwner().pOwnerScene->addLoadedResourcePath(path.string().c_str());
            }
        }
    }

    renderResourceExplorer<Material>("Material", inspected.pMaterial);

    // Drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_MATERIAL_EXTENSION))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

            if (Material* pMaterial = Engine::getInstance()->resourceManager.get<Material>(path.string().c_str()))
            {
                inspected.pMaterial = pMaterial;
            }
            else
            {
                if (const std::string* str = Engine::getInstance()->resourceManager.getKey(inspected.pMaterial))
                    inspected.pModel->getOwner().pOwnerScene->removeLoadedResourcePath(str->c_str());

                inspected.pMaterial = loadMaterialFile(path.string().c_str());
                inspected.pModel->getOwner().pOwnerScene->addLoadedResourcePath(path.string().c_str());
            }
        }
    }

    ImGui::Checkbox("Enable back face culling", &inspected.enableBackFaceCulling);

    const bool isCurrentElementVoid =
        !((size_t)inspected.pMesh & (size_t)inspected.pShader & (size_t)inspected.pMaterial);

    // This operation check if element must be added or remove from the the scene render system
    if (isPreviousElementVoid != isCurrentElementVoid)
    {
        if (isPreviousElementVoid)
        {
            inspected.pModel->getOwner().pOwnerScene->sceneRenderer.addSubModel(inspected);
        }
        else
        {
            inspected.pModel->getOwner().pOwnerScene->sceneRenderer.removeSubModel(inspected);
        }
    }
}

void Model::inspect(InspectContext& context)
{
    Component::inspect(context);

    ImGuiTreeNodeFlags nodeFlag =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

    const bool arrayIsOpen = ImGui::TreeNodeEx((void*)&m_subModels, nodeFlag, "SubModel");

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup(std::string("VectorOption").c_str());
    }

    if (ImGui::BeginPopup(std::string("VectorOption").c_str()))
    {
        if (ImGui::MenuItem("Emplace back"))
        {
            if (m_subModels.empty())
            {
                m_subModels.emplace_back();
            }
            else
            {
                m_subModels.emplace_back(m_subModels.back());
            }
            m_subModels.back().pModel = this;
        }

        ImGui::EndPopup();
    }

    if (arrayIsOpen)
    {
        size_t i = 0;
        for (auto&& it = m_subModels.begin(); it != m_subModels.end(); ++i)
        {
            ImGui::PushID(&*it);
            const bool treeIsOpen =
                ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlag, std::string("Element " + std::to_string(i)).c_str());

            // Check if user want to remove current element
            bool isDestroy = false;
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                ImGui::OpenPopup(std::string("InternalVectorOption").c_str());
            }

            if (ImGui::BeginPopup(std::string("InternalVectorOption").c_str()))
            {
                if (ImGui::MenuItem("Remove"))
                {
                    if ((size_t)it->pMesh & (size_t)it->pShader & (size_t)it->pMaterial) // If not nullptr detectedhh
                        it->pModel->getOwner().pOwnerScene->sceneRenderer.removeSubModel(*it);

                    it        = m_subModels.erase(it);
                    isDestroy = true;
                    --i;
                }

                ImGui::EndPopup();
            }

            if (!isDestroy)
            {
                // Check if user inspect the current element
                if (treeIsOpen)
                    DataInspector::inspect(context, *it);

                ++it;
            }
            if (treeIsOpen)
                ImGui::TreePop();
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
}

void Model::addSubModel(const SubModel::CreateArg& arg)
{
    GPE_ASSERT(!((size_t)arg.pMesh & (size_t)arg.pShader & (size_t)arg.pMaterial),
               "Invalid arguments to create submodel")

    SubModel& newSsub = m_subModels.emplace_back(*this, arg);
    getOwner().pOwnerScene->sceneRenderer.addSubModel(newSsub);
}

void Model::setActive(bool newState) noexcept
{
    if (m_isActivated == newState)
        return;

    m_isActivated = newState;
    if (m_isActivated)
    {
        for (SubModel& subMesh : m_subModels)
        {
            getOwner().pOwnerScene->sceneRenderer.addSubModel(subMesh);
        }
    }
    else
    {
        for (SubModel& subMesh : m_subModels)
        {
            getOwner().pOwnerScene->sceneRenderer.removeSubModel(subMesh);
        }
    }
}