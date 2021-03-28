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

    // Init current position
    int itemCurrent = 0;
    for (auto&& it = resourceContainer.begin(); &it->second != inRes; ++itemCurrent, ++it)
        ;

    if (ImGui::Combo(name, &itemCurrent, items.data(), items.size()))
    {
        auto&& it = resourceContainer.begin();
        for (int i = 0; i < itemCurrent; ++i, ++it)
            ;

        inRes = &it->second;
    }

    // GPE::Engine::getInstance()->resourceManager.getKey<Mesh>(inspected.pMesh)->c_str()
}

template <>
void GPE::DataInspector::inspect(SubModel& inspected)
{
    renderResourceExplorer<Mesh>("Mesh", inspected.pMesh);
    renderResourceExplorer<Shader>("Shader", inspected.pShader);
    renderResourceExplorer<Material>("Material", inspected.pMaterial);

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
