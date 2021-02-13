#include "Engine/Resources/Model.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Resources/Camera.hpp"
//#include "Engine/Resources/Light/light.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "GPM/mat.hpp"
//#include "Engine/Maths/Shape3D/Sphere.hpp"
#include "Engine/Intermediate/RenderSystem.hpp"

#include <memory>

using namespace Engine::Resources;
using namespace Engine::Intermediate;
using namespace Engine::Core::Debug;
using namespace GPM;

Model::Model(const Model &other)
    : IModel  (other._gameObject)
{
    m_name = __FUNCTION__;
    RendererSystem::addModel(this);
}

Model::Model(Model &&other)
    : IModel  (other._gameObject)
{
    m_name = __FUNCTION__;
    RendererSystem::addModel(this);
}

Model::~Model()
{
    RendererSystem::removeModel(this);
}

void Model::initTextureBufferWithMTLId ()
{
    for (auto&& idMat : m_pMesh->getIdMaterials())
    {
        unsigned int sizeTexBuffer = m_pMaterialToUse.size();

        for (Material& material : *m_pMaterial)
        {
            if(material.getName() == idMat)
            {
                m_pMaterialToUse.push_back(&material);
                break;
            }
        }

        if (sizeTexBuffer == m_pMaterialToUse.size())
        {
            functError((std::string("Invalid Tag \"") + idMat + "\"").c_str());
            exit(0);
        }
    }
}

Model::Model (GameObject &refGameObject, const ModelCreateArg& arg)
    :   IModel                  {refGameObject},
        m_pShader                {t_RessourcesManager::getRessourceManagerUse()->get<Shader>(arg.shaderName)},
        m_pMaterial              {t_RessourcesManager::getRessourceManagerUse()->get<std::vector<Material>>(arg.materialName)},
        m_pMesh                  {t_RessourcesManager::getRessourceManagerUse()->get<Mesh>(arg.meshName)},
        m_shaderName             {arg.shaderName},
        m_materialName           {arg.materialName},
        m_meshName               {arg.meshName},
        m_enableBackFaceCulling  {arg.enableBackFaceCulling},
        m_isOpaque               {arg.isOpaque}
{
    RendererSystem::addModel(this);

    if (m_pShader == nullptr)
    {
        og::logError(std::string("Canno't find shader with key \"") + arg.shaderName + "\" in gameObject " + _gameObject.getName());
        exit(0);
    }

    if (m_pMaterial == nullptr)
    {
        og::logError(std::string("Canno't find material with key \"") + arg.materialName + "\" in gameObject " + _gameObject.getName());
        exit(0);
    }

    if (m_pMesh == nullptr)
    {
        og::logError(std::string("Canno't find mesh with key \"") + arg.meshName + "\" in gameObject " + _gameObject.getName());
        exit(0);  
    }

    initTextureBufferWithMTLId();

    if (arg.loadInGPU)
    {
        loadInGPU ();
    }
    m_name = __FUNCTION__;
}

Model::Model(GameObject &refGameObject, std::vector<std::string>& params, t_RessourcesManager& ressourcesManager)
    :   IModel                  (refGameObject),
        m_pShader                (ressourcesManager.get<Shader>(params[0])),
        m_pMaterial              (ressourcesManager.get<std::vector<Material>>(params[1])),
        m_pMesh                  (ressourcesManager.get<Mesh>(params[2])),
        m_shaderName             (params[0]),
        m_materialName           ({params[1]}),
        m_meshName               (params[2]),
        m_enableBackFaceCulling  (true),
        m_isOpaque               (true)
{
    RendererSystem::addModel(this);
    initTextureBufferWithMTLId();

    loadInGPU ();
    m_name = __FUNCTION__;
}

void Model::draw () const noexcept
{
    GPE_assert (isLoadInGPU(), "Try to draw Model not load in GPU");
    GPE_assert (Camera::getCamUse() != nullptr, "Try to draw Model without camera bind");

    m_pShader->use();

    if ((m_pShader->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        //const std::vector<light>& lightBuffer = Light::getLightsToUseInAlignasStruct();
        //m_pShader->setLightBlock(lightBuffer, Camera::getCamUse()->getPosition());

        Mat4 inverseModelMatrix;
        _gameObject.getModelMatrix().inverse(inverseModelMatrix);
        Mat3 inverseModelMatrix3 (inverseModelMatrix);

        sendToShaderModelMatrix();
        m_pShader->setMat4("projectViewModelMatrix", &(Camera::getCamUse()->getProjection() * Camera::getCamUse()->getView() * _gameObject.getModelMatrix()).mat[0]);
        m_pShader->setMat3("inverseModelMatrix", &inverseModelMatrix3.mat[0]);
    }
    else
    {
        if ((m_pShader->getFeature() & SKYBOX) != SKYBOX)
        {
            sendToShaderModelMatrix();
            m_pShader->setMat4("view", &Camera::getCamUse()->getView().mat[0]);
            m_pShader->setMat4("projection", &Camera::getCamUse()->getProjection().mat[0]);
        }
    }

    if ((m_pShader->getFeature() & SKYBOX) == SKYBOX)
    {
        Mat4 view = Camera::getCamUse()->getView();
        //suppress translation
        view[3][0] = 0;
        view[3][1] = 0;
        view[3][2] = 0;
        m_pShader->setMat4("view", &view.mat[0]);
        m_pShader->setMat4("projection", &Camera::getCamUse()->getProjection().mat[0]);
    }

    if ((m_pShader->getFeature() & SCALE_TIME_ACC) == SCALE_TIME_ACC)
    {
        m_pShader->setFloat("scaledTimeAcc", TimeSystem::getAccumulateTime());
    }

    if ((m_pShader->getFeature() & UNSCALED_TIME_ACC) == UNSCALED_TIME_ACC)
    {
        m_pShader->setFloat("unscaledTimeAcc", TimeSystem::getAccumulateUnscaledTime());
    }

    const GLuint* pIdVAO = m_pMesh->getVAOId();
    if(pIdVAO == nullptr)
    {
        m_pMesh->loadInGPU();
        pIdVAO = m_pMesh->getVAOId();
        og::logWarning((std::string("Model \"") + _gameObject.getName() + "\" was try to drawing without load in GPU").c_str());
    }

    if (m_enableBackFaceCulling)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); 
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    
    glBindVertexArray(*pIdVAO);

	unsigned int first = 0;
    for (size_t part = 0; part < m_pMesh->getIndices().size(); part++)
    {
        if (!m_pMaterialToUse.empty())
        {
            if ((m_pMaterialToUse[part])->getPDiffuseTexture() != nullptr)
            {
                (m_pMaterialToUse[part])->getPDiffuseTexture()->use();
            }
        
            if ((m_pShader->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
            {
                m_pShader->setMaterialBlock((m_pMaterialToUse[part])->getMaterialComponent());
            }
            
            if ((m_pShader->getFeature()  & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
            {
               m_pShader->setVec4("Color",  (m_pMaterialToUse[part])->getMaterialComponent().ambient.kr, 
                                            (m_pMaterialToUse[part])->getMaterialComponent().ambient.kg,
                                            (m_pMaterialToUse[part])->getMaterialComponent().ambient.kb,
                                            (m_pMaterialToUse[part])->getMaterialComponent().ambient.ki);
            }
        }
        else
        {          
            if ((m_pShader->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
            {
                m_pShader->setMaterialBlock((*m_pMaterial)[0].getMaterialComponent());
            }
            
            if ((m_pShader->getFeature()  & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
            {
               m_pShader->setVec4("Color",  (*m_pMaterial)[0].getMaterialComponent().ambient.kr, 
                                            (*m_pMaterial)[0].getMaterialComponent().ambient.kg,
                                            (*m_pMaterial)[0].getMaterialComponent().ambient.kb,
                                            (*m_pMaterial)[0].getMaterialComponent().ambient.ki);
            }
            else
            {
                if ((*m_pMaterial)[0].getPDiffuseTexture() != nullptr)
                    (*m_pMaterial)[0].getPDiffuseTexture()->use();
            }
        }

		glDrawArrays(GL_TRIANGLES, first, m_pMesh->getIndices()[part].size());
		first += m_pMesh->getIndices()[part].size();
	}
        glBindTexture(GL_TEXTURE_2D, 0);
}

void Model::loadInGPU() noexcept
{
    for (size_t part = 0; part < m_pMaterialToUse.size(); part++)
    {
        if ((m_pMaterialToUse[part])->getPDiffuseTexture() != nullptr && !(m_pMaterialToUse[part])->getPDiffuseTexture()->isLoadInGPU())
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
        if ((m_pMaterialToUse[part])->getPDiffuseTexture() != nullptr && (m_pMaterialToUse[part])->getPDiffuseTexture()->isLoadInGPU())
            (m_pMaterialToUse[part])->getPDiffuseTexture()->unloadFromGPU();
    }

    if (m_pMesh->isLoadInGPU())
        m_pMesh->unloadFromGPU();

    m_isLoadInGPU = false;
}
 
void Model::sendToShaderModelMatrix () const noexcept
{
    m_pShader->setMat4("model", &_gameObject.getModelMatrix().mat[0]);
}

/*
std::shared_ptr<Engine::Core::Maths::Shape3D::Volume> Model::getpBoudingVolume() const noexcept
{
    if (m_pMesh->getBoundingVolumeType() == BoundingVolume::SPHERE)
    {
        const Engine::Core::Maths::Shape3D::Sphere* boundingSphere = dynamic_cast<const Engine::Core::Maths::Shape3D::Sphere*>(m_pMesh->getBoundingVolume());

        float maxScale = std::max(std::max(_gameObject.getScale().x, _gameObject.getScale().y), _gameObject.getScale().z);
        return std::make_shared<Engine::Core::Maths::Shape3D::Sphere>(boundingSphere->getRadius() * maxScale, _gameObject.getGlobalPosition() + boundingSphere->getCenter());
    }
    else
    {
        return nullptr;
    }
}*/

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

        modelPartContenor.emplace_back( ModelPart{
                                                RenderPassKey{  
                                                    
                                                        pMatToUse->getPDiffuseTexture() == nullptr ? 0 : pMatToUse->getPDiffuseTexture()->getID(),
                                                        *m_pMesh->getVAOId(),
                                                        m_pShader->getIdProgramm()
                                                },
                                                this,
                                                pMatToUse,
                                                m_enableBackFaceCulling, 
                                                first,
                                                static_cast<unsigned int>(m_pMesh->getIndices()[part].size())
                                        });

        first += m_pMesh->getIndices()[part].size();
    }
}