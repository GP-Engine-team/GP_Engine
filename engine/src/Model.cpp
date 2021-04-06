#include "Engine/ECS/Component/Model.hpp"

File_GENERATED

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Tools/Hash.hpp"
#include "Engine/ECS/System/SceneRenderSystem.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"
#include "Engine/Resources/Scene.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Texture.hpp"
#include "GPM/Matrix3.hpp"
#include "GPM/Matrix4.hpp"
#include "GPM/Shape3D/Sphere.hpp"

#include <filesystem>
#include <imgui.h>

using namespace GPE;
using namespace GPM;

void foo()
{
    GPE::Model::staticGetArchetype();
}

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
    for (SubModel& pSubMesh : m_subModels)
        getOwner().pOwnerScene->sceneRenderer.removeSubModel(&pSubMesh);
}

Model::Model(GameObject& owner) : Model(owner, CreateArg{})
{
}

Model::Model(GameObject& owner, const CreateArg& arg) : Component{owner}, m_subModels{arg.subModels}
{
    for (SubModel& pSubMesh : m_subModels)
    {
        pSubMesh.pModel = this;
        getOwner().pOwnerScene->sceneRenderer.addSubModel(&pSubMesh);
    }
}

Model& Model::operator=(Model&& other)
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

void Model::moveTowardScene(class Scene& newOwner)
{
    for (SubModel& pSubMesh : m_subModels)
        getOwner().pOwnerScene->sceneRenderer.removeSubModel(&pSubMesh);

    for (SubModel& pSubMesh : m_subModels)
    {
        pSubMesh.pModel = this;
        newOwner.sceneRenderer.addSubModel(&pSubMesh);
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

    if (ImGui::Combo(name, &itemCurrent, items.data(), items.size()))
    {
        auto&& it = resourceContainer.begin();
        for (int i = 0; i < itemCurrent; ++i, ++it)
            ;

        inRes = &it->second;
    }
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, SubModel& inspected)
{
    const bool isPreviousElementVoid =
        !((size_t)inspected.pMesh & (size_t)inspected.pShader & (size_t)inspected.pMaterial);

    renderResourceExplorer<Mesh>("Mesh", inspected.pMesh);
    renderResourceExplorer<Shader>("Shader", inspected.pShader);
    renderResourceExplorer<Material>("Material", inspected.pMaterial);

    ImGui::Checkbox("Enable back face culling", &inspected.enableBackFaceCulling);

    const bool isCurrentlementVoid =
        !((size_t)inspected.pMesh & (size_t)inspected.pShader & (size_t)inspected.pMaterial);

    // This operation check if element must be added or remove from the the scene render system
    if (isPreviousElementVoid != isCurrentlementVoid)
    {
        if (isPreviousElementVoid)
        {
            inspected.pModel->getOwner().pOwnerScene->sceneRenderer.addSubModel(&inspected);
        }
        else
        {
            inspected.pModel->getOwner().pOwnerScene->sceneRenderer.removeSubModel(&inspected);
        }
    }

    /*
    // Drop
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::GetDragDropPayload();

        if (payload->IsDataType("RESOURCE_PATH"))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

            size_t hasedExtention = hash(path.extension().string().c_str());
            if (hasedExtention == hash(".obj"))
            {
                // Can drop
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_PATH"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                    std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);
                }
            }
        }
    }*/
}

void Model::inspect(InspectContext& context)
{
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
                        it->pModel->getOwner().pOwnerScene->sceneRenderer.removeSubModel(&(*it));

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
                {
                    DataInspector::inspect(context, *it);
                    ImGui::TreePop();
                }

                ++it;
            }
        }
        ImGui::TreePop();
    }

    return;
}