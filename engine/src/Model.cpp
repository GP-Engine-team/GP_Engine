#include "Engine/Resources/Model.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Intermediate/RenderSystem.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Texture.hpp"
#include "Engine/Resources/Material.hpp"
#include "GPM/Matrix3.hpp"
#include "GPM/Matrix4.hpp"
#include "GPM/Shape3D/Sphere.hpp"

using namespace GPE;
using namespace GPM;

bool GPE::isSubModelHasPriorityOverAnother(const SubModel* lhs, const SubModel* rhs) noexcept
{
    return lhs->pShader->getID() < rhs->pShader->getID() || lhs->pMesh->getID() < rhs->pMesh->getID() ||
           lhs->pMaterial->getDiffuseTexture()->getID() < rhs->pMaterial->getDiffuseTexture()->getID();
}

Model::Model(const Model& other) noexcept : Component(other.m_gameObject), m_subModels{other.m_subModels}
{
    for (SubModel& pSubMesh : m_subModels)
        RenderSystem::getInstance()->addSubModel(&pSubMesh);
}

Model::Model(Model&& other) noexcept : Component(other.m_gameObject), m_subModels{other.m_subModels}
{
    //Access to invalide access memory but pointer is still valid
    for (size_t i = 0; i < m_subModels.size(); ++i)
        RenderSystem::getInstance()->updateSubModelPointer(&m_subModels[i], &other.m_subModels[i]);

}

Model::~Model()
{
    for (SubModel& pSubMesh : m_subModels)
        RenderSystem::getInstance()->removeSubModel(&pSubMesh);
}

Model::Model(GameObject& owner, const CreateArg& arg)
    : Component{owner}, m_subModels{*arg.pSubModels}
{
    for (SubModel& pSubMesh : m_subModels)
    {
        pSubMesh.pModel = this;
        RenderSystem::getInstance()->addSubModel(&pSubMesh);
    }
}