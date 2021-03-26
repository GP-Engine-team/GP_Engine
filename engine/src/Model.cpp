#include "Engine/ECS/Component/Model.hpp"

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

bool GPE::isSubModelHasPriorityOverAnother(const SubModel* lhs, const SubModel* rhs) noexcept
{
    return lhs->pShader->getID() < rhs->pShader->getID() || lhs->pMesh->getID() < rhs->pMesh->getID() ||
           lhs->pMaterial->getID() < rhs->pMaterial->getID() ||
           lhs->pMaterial->getDiffuseTexture()->getID() < rhs->pMaterial->getDiffuseTexture()->getID() ||
           (lhs->pMaterial->isOpaque() && !rhs->pMaterial->isOpaque());
}

Model::Model(Model&& other) noexcept : Component(other.getOwner()), m_subModels{other.m_subModels}
{
    // Access to invalide access memory but pointer is still valid
    for (size_t i = 0; i < m_subModels.size(); ++i)
        getOwner().pOwnerScene->sceneRenderer.updateSubModelPointer(&m_subModels[i], &other.m_subModels[i]);
}

Model::~Model()
{
    for (SubModel& pSubMesh : m_subModels)
        getOwner().pOwnerScene->sceneRenderer.removeSubModel(&pSubMesh);

    DataChunk<Model>::getInstance()->destroy(this);
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
    m_subModels = std::move(other.m_subModels);

    for (size_t i = 0; i < m_subModels.size(); i++)
        getOwner().pOwnerScene->sceneRenderer.updateSubModelPointer(&m_subModels[i], &other.m_subModels[i]);

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

template <>
void GPE::DataInspector::inspect(SubModel& inspected)
{
    static bool resourceExplorerMesh   = false;
    static bool resourceExplorerShader = false;

    ImGui::Text("Mesh");
    ImGui::SameLine();
    if (ImGui::Button(GPE::Engine::getInstance()->resourceManager.getKey<Mesh>(inspected.pMesh)->c_str()))
    {
        resourceExplorerMesh = !resourceExplorerMesh;
    }

    if (resourceExplorerMesh)
    {
        ImGui::Begin("Resource explorer", &resourceExplorerMesh, ImGuiWindowFlags_HorizontalScrollbar);

        for (auto&& res : GPE::Engine::getInstance()->resourceManager.getAll<Mesh>())
        {
            if (ImGui::Button(res.first.c_str()))
            {
                resourceExplorerMesh = false;
                inspected.pMesh      = &res.second;
            }
        }

        ImGui::End();
    }

    ImGui::Text("Shader");
    ImGui::SameLine();
    if (ImGui::Button(GPE::Engine::getInstance()->resourceManager.getKey<Shader>(inspected.pShader)->c_str()))
    {
        resourceExplorerShader = !resourceExplorerShader;
    }

    if (resourceExplorerShader)
    {
        ImGui::Begin("Resource explorer", &resourceExplorerShader, ImGuiWindowFlags_HorizontalScrollbar);

        for (auto&& res : GPE::Engine::getInstance()->resourceManager.getAll<Shader>())
        {
            if (ImGui::Button(res.first.c_str()))
            {
                resourceExplorerShader = false;
                inspected.pShader      = &res.second;
            }
        }

        ImGui::End();
    }

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

                    // Engine::getInstance()->resourceManager.add(path.string(), )
                }
            }
        }
    }
}
