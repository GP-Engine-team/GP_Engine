#include "Engine/Resources/Model.hpp"

#include <memory>

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "gpm/Matrix3.hpp"
#include "gpm/Matrix4.hpp"
//#include "Engine/Intermediate/RenderSystem.hpp"

using namespace Engine::Resources;
using namespace Engine::Intermediate;
using namespace Engine::Core::Debug;
using namespace GPM;

Model::Model(const Model& other) : IModel(*other.m_gameObject)
{
    //RendererSystem::addModel(this);
}

Model::Model(Model&& other) : IModel(*other.m_gameObject)
{
    //RendererSystem::addModel(this);
}

Model::~Model()
{
    //RendererSystem::removeModel(this);
}

void Model::initTextureBufferWithMTLId()
{
    for (auto&& idMat : m_pMesh->getIdMaterials())
    {
        unsigned int sizeTexBuffer = m_pMaterialToUse.size();

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

Model::Model(GameObject& refGameObject, const CreateArg& arg)
    : IModel{refGameObject}, m_pShader{arg.pShader}, m_pMaterial{arg.pMaterials}, m_pMesh{arg.pMesh},
      m_enableBackFaceCulling{arg.enableBackFaceCulling}, m_isOpaque{arg.isOpaque}
{
    //RendererSystem::addModel(this);

    initTextureBufferWithMTLId();

    if (arg.loadInGPU)
    {
        loadInGPU();
    }
}

void Model::loadInGPU() noexcept
{
    for (size_t part = 0; part < m_pMaterialToUse.size(); part++)
    {
        if ((m_pMaterialToUse[part])->getPDiffuseTexture() != nullptr &&
            !(m_pMaterialToUse[part])->getPDiffuseTexture()->isLoadInGPU())
            (m_pMaterialToUse[part])->getPDiffuseTexture()->loadInGPU();
    }

    if (!m_pMesh->isLoadInGPU())
        m_pMesh->loadInGPU();

    m_isLoadInGPU = true;
}

void Model::unloadFromGPU() noexcept
{
    for (size_t part = 0; part < m_pMaterialToUse.size(); part++)
    {
        if ((m_pMaterialToUse[part])->getPDiffuseTexture() != nullptr &&
            (m_pMaterialToUse[part])->getPDiffuseTexture()->isLoadInGPU())
            (m_pMaterialToUse[part])->getPDiffuseTexture()->unloadFromGPU();
    }

    if (m_pMesh->isLoadInGPU())
        m_pMesh->unloadFromGPU();

    m_isLoadInGPU = false;
}

void Model::insertModelPartsOnContenor(std::list<ModelPart>& modelPartContenor) noexcept
{
    unsigned int first = 0;
    Material* pMatToUse = nullptr;

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

        first += m_pMesh->getIndices()[part].size();
    }
}