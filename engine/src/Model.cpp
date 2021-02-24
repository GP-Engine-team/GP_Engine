#include "Engine/Resources/Model.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Intermediate/RenderSystem.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Texture.hpp"
#include "GPM/Matrix3.hpp"
#include "GPM/Matrix4.hpp"
#include "GPM/Shape3D/Sphere.hpp"

using namespace GPE;
using namespace GPM;

Model::Model(const Model& other) noexcept
    : Component(other.m_gameObject), m_pShader{other.m_pShader}, m_pMaterial{other.m_pMaterial}, m_pSubMeshs{other.m_pSubMeshs},
      m_enableBackFaceCulling{other.m_enableBackFaceCulling}, m_isOpaque{other.m_isOpaque}, m_isLoadInGPU{
                                                                                                other.m_isLoadInGPU}
{
    RenderSystem::getInstance()->addModel(this);
}

Model::Model(Model&& other) noexcept
    : Component(other.m_gameObject), m_pShader{std::move(other.m_pShader)}, m_pMaterial{std::move(other.m_pMaterial)}, m_pSubMeshs{std::move(other.m_pSubMeshs)},
      m_enableBackFaceCulling{std::move(other.m_enableBackFaceCulling)}, m_isOpaque{std::move(other.m_isOpaque)},
      m_isLoadInGPU{std::move(other.m_isLoadInGPU)}
{
    RenderSystem::getInstance()->updateModelPointer(this, &other);
}

Model::~Model()
{
    RenderSystem::getInstance()->removeModel(this);
}

Model::Model(GameObject& owner, const CreateArg& arg)
    : Component{owner}, m_pShader{arg.pShader}, m_pMaterial{arg.pMaterials}, m_pSubMeshs{arg.m_pSubMeshs},
      m_enableBackFaceCulling{arg.enableBackFaceCulling}, m_isOpaque{arg.isOpaque}
{
    RenderSystem::getInstance()->addModel(this);
}

std::shared_ptr<GPM::Volume> Model::getpBoudingVolume() const noexcept
{
    // TODO: Horrible technique
    if ((*m_pSubMeshs)[0].getBoundingVolumeType() == Mesh::BoundingVolume::SPHERE)
    {
        const GPM::Sphere* boundingSphere = dynamic_cast<const GPM::Sphere*>((*m_pSubMeshs)[0].getBoundingVolume());

        float maxScale =
            std::max(std::max(m_gameObject.getTransform().getScale().x, m_gameObject.getTransform().getScale().y), m_gameObject.getTransform().getScale().z);
        return std::make_shared<GPM::Sphere>(boundingSphere->getRadius() * maxScale,
                                             m_gameObject.getTransform().getGlobalPosition() + boundingSphere->getCenter());
    }
    else
    {
        return nullptr;
    }
}