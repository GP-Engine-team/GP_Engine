#include "Engine/Intermediate/RenderSystem.hpp"

#include <algorithm> //std::sort
#include <memory>
#include <cstdio>
#include <map> //std::map


#include "Engine/Intermediate/GameObject.hpp"
//#include "Engine/Core/System/TimeSystem.hpp"
#include "GPM/Matrix4.hpp"

using namespace Engine::Intermediate;
using namespace Engine::Resources;
using namespace GPM;

RenderSystem*   RenderSystem::m_pInstance{nullptr};
std::shared_mutex RenderSystem::m_mutex;

void sendDataToInitShader(Camera& camToUse, Shader* pCurrentShaderUse)
{
    /*
    if ((pCurrentShaderUse->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        const std::vector<light>& lightBuffer = Light::getLightsToUseInAlignasStruct();
        pCurrentShaderUse->setLightBlock(lightBuffer, camToUse.getPosition());
    }
    else
    {
        if ((pCurrentShaderUse->getFeature() & SKYBOX) != SKYBOX)
        {
            pCurrentShaderUse->setMat4("view", camToUse.getView().e);
            pCurrentShaderUse->setMat4("projection", camToUse.getProjection().e);
        }
    }

    if ((pCurrentShaderUse->getFeature() & SKYBOX) == SKYBOX)
    {
        Mat4 view = Camera::getCamUse()->getView();
        //suppress translation
        view[3][0] = 0;
        view[3][1] = 0;
        view[3][2] = 0;
        pCurrentShaderUse->setMat4("view", &view.mat[0]);
        pCurrentShaderUse->setMat4("projection", &Camera::getCamUse()->getProjection().mat[0]);
    }

    if ((pCurrentShaderUse->getFeature() & SCALE_TIME_ACC) == SCALE_TIME_ACC)
    {
        pCurrentShaderUse->setFloat("scaledTimeAcc", TimeSystem::getAccumulateTime());
    }

    if ((pCurrentShaderUse->getFeature() & UNSCALED_TIME_ACC) == UNSCALED_TIME_ACC)
    {
        pCurrentShaderUse->setFloat("unscaledTimeAcc", TimeSystem::getAccumulateUnscaledTime());
    }*/
}

void sendModelDataToShader(ModelPart& modelPart)
{
    Shader* pShader = modelPart.pModel->getpShader();
    /*
    if ((pShader->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        Mat4 inverseModelMatrix;
        modelPart.pModel->getGameObject().getModelMatrix().inverse(inverseModelMatrix);
        Mat3 inverseModelMatrix3 (inverseModelMatrix);

        modelPart.pModel->sendToShaderModelMatrix();
        pShader->setMat4("projectViewModelMatrix", &(Camera::getCamUse()->getProjection() * Camera::getCamUse()->getView() * modelPart.pModel->getGameObject().getModelMatrix()).mat[0]);
        pShader->setMat3("inverseModelMatrix", &inverseModelMatrix3.mat[0]);
    }
    else
    {
        if ((pShader->getFeature() & SKYBOX) != SKYBOX)
        {
            modelPart.pModel->sendToShaderModelMatrix();
        }
    }

    if ((pShader->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        pShader->setMaterialBlock(modelPart.pMaterialToUse->getMaterialComponent());
    }*/
    
    if ((pShader->getFeature()  & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
    {
        pShader->setVec4("Color",   modelPart.pMaterialToUse->getMaterialComponent().ambient.kr, 
                                    modelPart.pMaterialToUse->getMaterialComponent().ambient.kg,
                                    modelPart.pMaterialToUse->getMaterialComponent().ambient.kb,
                                    modelPart.pMaterialToUse->getMaterialComponent().ambient.ki);
    }
}

void drawModelPart(const ModelPart& modelPart)
{
    // TODO : Remove gl functions
    glDrawArrays(GL_TRIANGLES, modelPart.indexStart, modelPart.indexCount);
}

void RenderSystem::tryToBindShader(Shader* pShader)
{
    if (m_currentShaderId == pShader->getIdProgramm())
        return;

    // TODO : Remove gl functions
    glUseProgram(pShader->getIdProgramm());

    m_currentShaderId = pShader->getIdProgramm();
    m_currentPShaderUse = pShader;

    sendDataToInitShader(*m_pCameras[0] ,m_currentPShaderUse);
}

void RenderSystem::tryToBindTexture(unsigned int textureId)
{
    if (m_currentTextureId == textureId)
        return;

    // TODO : Remove gl functions
    glBindTexture(GL_TEXTURE_2D, textureId);

    m_currentTextureId = textureId;
}

void RenderSystem::tryToBindMesh(unsigned int meshId)
{
    if (m_currentMeshId == meshId)
        return;

    // TODO : Remove gl functions
    glBindVertexArray(meshId);

    m_currentMeshId = meshId;
}

void RenderSystem::tryToSetBackFaceCulling(bool useBackFaceCulling)
{
    if (m_currentBackFaceCullingModeEnable == useBackFaceCulling)
        return;

    if (useBackFaceCulling)
    {
        // TODO : Remove gl functions
        glEnable(GL_CULL_FACE);
    }
    else
    {
        // TODO : Remove gl functions
        glDisable(GL_CULL_FACE);
    }

    m_currentBackFaceCullingModeEnable = useBackFaceCulling;
}

void RenderSystem::resetCurrentRenderPassKey()
{
    m_currentShaderId         = 0;
    m_currentTextureId        = 0;
    m_currentMeshId           = 0;
    m_currentBackFaceCullingModeEnable = false;
    m_currentPShaderUse       = nullptr;

    // TODO : Remove gl functions
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void RenderSystem::draw () noexcept
{
    std::list<ModelPart> modelParts;
    std::map<float, Model*> mapElemSortedByDistance;

    //CameraFrustum camFrustum = Camera::getCamUse()->getFrustum();

    for (auto &&pModel : m_pModels)
    {
        if (pModel->isActivated()/* && isOnCameraFrustum(camFrustum, pModel)*/)
        {
            if (pModel->isOpaque())
            {
                pModel->insertModelPartsOnContenor(modelParts);
            }
            /*else
            {
                
                float distance = (m_pCameras[0]->getGameObject().getTransform().getGlobalPosition() -
                                  (pModel->getGameObject().getTransform().getGlobalPosition())).length2();
                mapElemSortedByDistance[distance] = pModel;
            }*/
        }
    }

    modelParts.sort();

    resetCurrentRenderPassKey();
    // TODO : Remove gl functions
    glDisable(GL_BLEND);

    for (auto &&modelPart : modelParts)
    {
        tryToBindShader(modelPart.pModel->getpShader());
        tryToBindMesh(modelPart.key.meshId);
        tryToBindTexture(modelPart.key.textureId);
        tryToSetBackFaceCulling(modelPart.useBackFaceCulling);

        //TODO: To optimize ! Use Draw instanced Array
        sendModelDataToShader(modelPart);
        drawModelPart(modelPart);
    }

    // TODO : Remove gl functions
    glEnable(GL_BLEND);

    /*Display transparent element*/
    /*for(std::map<float, Model*>::reverse_iterator it = mapElemSortedByDistance.rbegin(); it != mapElemSortedByDistance.rend(); ++it) 
    {
        it->second->draw();
    };*/

    resetCurrentRenderPassKey();
}

void RenderSystem::addModel(Model* pModel) noexcept
{
    m_pModels.push_back(pModel);
}

void RenderSystem::updateModelPointer(Model* newPointorModel, Model* exPointorModel) noexcept
{
    for (std::vector<Model*>::iterator it = m_pModels.begin(); it != m_pModels.end(); it++)
    {
        if ((*it) == exPointorModel)
        {
            *it = newPointorModel;
            return;
        }
    }
}

void RenderSystem::removeModel (Model* pModel) noexcept
{
    for (std::vector<Model*>::iterator it = m_pModels.begin(); it != m_pModels.end(); it++)
    {
        if ((*it) == pModel)
        {
            std::swap<Model*>(m_pModels.back(), (*it));
            m_pModels.pop_back();
            return;
        }
    }
}

void RenderSystem::addCamera(Camera* pCamera) noexcept
{
    m_pCameras.push_back(pCamera);
}

void RenderSystem::updateCameraPointer(Camera* newPointorCamera, Camera* exPointorCamera) noexcept
{
    for (std::vector<Camera*>::iterator it = m_pCameras.begin(); it != m_pCameras.end(); it++)
    {
        if ((*it) == exPointorCamera)
        {
            *it = newPointorCamera;
            return;
        }
    }
}

void RenderSystem::removeCamera(Camera* pCamera) noexcept
{
    for (std::vector<Camera*>::iterator it = m_pCameras.begin(); it != m_pCameras.end(); it++)
    {
        if ((*it) == pCamera)
        {
            std::swap<Camera*>(m_pCameras.back(), (*it));
            m_pCameras.pop_back();
            return;
        }
    }
}