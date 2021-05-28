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

bool SubModel::isValide()
{
    // TODO: remove diffuse texture check
    return pModel && pMesh && pShader && pMaterial && pMaterial->getDiffuseTexture();
}

bool GPE::isSubModelHasPriorityOverAnother(const SubModel* lhs, const SubModel* rhs) noexcept
{
    return lhs->pShader->getID() < rhs->pShader->getID() || lhs->pMesh->getID() < rhs->pMesh->getID() ||
           lhs->pMaterial->getID() < rhs->pMaterial->getID() ||
           lhs->pMaterial->getDiffuseTexture()->getID() < rhs->pMaterial->getDiffuseTexture()->getID() ||
           (lhs->pMaterial->isOpaque() && !rhs->pMaterial->isOpaque());
}

Model::~Model()
{
    setActive(false);
}

Model::Model(GameObject& owner) : Model(owner, CreateArg{})
{
    updateToSystem();
}

Model::Model(GameObject& owner, const CreateArg& arg) : Component{owner}, m_subModels{arg.subModels}
{
    updateToSystem();
}

void Model::moveTowardScene(class Scene& newOwner)
{
    if (getOwner().pOwnerScene)
    {
        for (SubModel& subMesh : m_subModels)
            getOwner().pOwnerScene->sceneRenderer.removeSubModel(subMesh);
    }

    for (SubModel& subMesh : m_subModels)
    {
        subMesh.pModel = this;
        newOwner.sceneRenderer.addSubModel(subMesh);
    }
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, SubModel& inspected)
{
    const bool isPreviousElementVoid = !inspected.isValide();

    inspect(context, inspected.pMesh, "Mesh");
    inspect(context, inspected.pShader, "Shader");
    inspect(context, inspected.pMaterial, "Material");

    ImGui::Checkbox("Enable back face culling", &inspected.enableBackFaceCulling);
    ImGui::Checkbox("Cast shadow", &inspected.castShadow);

    // This operation check if element must be added or remove from the the scene render system
    if (isPreviousElementVoid != !inspected.isValide())
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
    GPE_ASSERT(arg.pMesh && arg.pShader && arg.pMaterial, "Invalid arguments to create submodel")

    SubModel& newSsub = m_subModels.emplace_back(*this, arg);
    getOwner().pOwnerScene->sceneRenderer.addSubModel(newSsub);
}

void Model::updateToSystem() noexcept
{
    if (m_isActivated)
    {
        for (SubModel& subMesh : m_subModels)
        {
            subMesh.pModel = this;
            if (subMesh.isValide())
                getOwner().pOwnerScene->sceneRenderer.addSubModel(subMesh);
        }
    }
    else
    {
        if (!getOwner().pOwnerScene)
            return;

        for (SubModel& subMesh : m_subModels)
        {
            if (subMesh.isValide())
                getOwner().pOwnerScene->sceneRenderer.removeSubModel(subMesh);
        }
    }
}