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

Shader*                m_pShader        = nullptr;
std::vector<Material>* m_pMaterial      = nullptr; // contain the texture and material data
std::vector<Material*> m_pMaterialToUse = {};      // contain pointor to the material to use when model is display.
Mesh*                  m_pMesh          = nullptr;

bool m_enableBackFaceCulling = true;
bool m_isOpaque              = true;
bool m_isLoadInGPU           = false;

Model::Model(const Model& other) noexcept
    : Component(other.m_gameObject), m_pShader{other.m_pShader}, m_pMaterial{other.m_pMaterial},
      m_pMaterialToUse{other.m_pMaterialToUse}, m_pMesh{other.m_pMesh},
      m_enableBackFaceCulling{other.m_enableBackFaceCulling}, m_isOpaque{other.m_isOpaque}, m_isLoadInGPU{
                                                                                                other.m_isLoadInGPU}
{
    RenderSystem::getInstance()->addModel(this);
}

Model::Model(Model&& other) noexcept
    : Component(other.m_gameObject), m_pShader{std::move(other.m_pShader)}, m_pMaterial{std::move(other.m_pMaterial)},
      m_pMaterialToUse{std::move(other.m_pMaterialToUse)}, m_pMesh{std::move(other.m_pMesh)},
      m_enableBackFaceCulling{std::move(other.m_enableBackFaceCulling)}, m_isOpaque{std::move(other.m_isOpaque)},
      m_isLoadInGPU{std::move(other.m_isLoadInGPU)}
{
    RenderSystem::getInstance()->updateModelPointer(this, &other);
}

Model::~Model()
{
    RenderSystem::getInstance()->removeModel(this);
}

void Model::initTextureBufferWithMTLId() noexcept
{
    for (auto&& idMat : m_pMesh->getIdMaterials())
    {
        size_t sizeTexBuffer = m_pMaterialToUse.size();

        for (Material& material : *m_pMaterial)
        {
            if (material.getName() == idMat)
            {
                m_pMaterialToUse.push_back(&material);
                break;
            }
        }

        if (sizeTexBuffer == m_pMaterialToUse.size())
        {
            FUNCT_ERROR((std::string("Invalid Tag \"") + idMat + "\"").c_str());
            exit(0);
        }
    }
}

Model::Model(GameObject& owner, const CreateArg& arg)
    : Component{owner}, m_pShader{arg.pShader}, m_pMaterial{arg.pMaterials}, m_pMesh{arg.pMesh},
      m_enableBackFaceCulling{arg.enableBackFaceCulling}, m_isOpaque{arg.isOpaque}
{
    RenderSystem::getInstance()->addModel(this);

    initTextureBufferWithMTLId();

    if (arg.loadInGPU)
    {
        loadInGPU();
    }
}

void Model::loadInGPU() noexcept
{
    if (!m_pMesh->isLoadInGPU())
        m_pMesh->loadInGPU();

    m_isLoadInGPU = true;
}

void Model::unloadFromGPU() noexcept
{
    if (m_pMesh->isLoadInGPU())
        m_pMesh->unloadFromGPU();

    m_isLoadInGPU = false;
}

std::shared_ptr<GPM::Volume> Model::getpBoudingVolume() const noexcept
{
    // TODO: Horrible technique
    if (m_pMesh->getBoundingVolumeType() == Mesh::BoundingVolume::SPHERE)
    {
        const GPM::Sphere* boundingSphere = dynamic_cast<const GPM::Sphere*>(m_pMesh->getBoundingVolume());

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

void Model::insertModelPartsOnContenor(std::list<ModelPart>& modelPartContenor) noexcept
{
    unsigned int first     = 0;
    Material*    pMatToUse = nullptr;

    for (size_t part = 0; part < m_pMesh->getIndices().size(); part++)
    {
        if (!m_pMaterialToUse.empty())
        {
            pMatToUse = m_pMaterialToUse[part];
        }
        else
        {
            pMatToUse = &m_pMaterial->front();
        }

        modelPartContenor.emplace_back(ModelPart{
            RenderPassKey{pMatToUse->getPDiffuseTexture() == nullptr ? 0 : pMatToUse->getPDiffuseTexture()->getID(),
                          *m_pMesh->getVAOId(), m_pShader->getIdProgramm()},
            this, pMatToUse, m_enableBackFaceCulling, first,
            static_cast<unsigned int>(m_pMesh->getIndices()[part].size())});

        first += static_cast<unsigned int>(m_pMesh->getIndices()[part].size());
    }
}