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

void sendDataToInitShader(Camera& camToUse, std::vector<Light*> lights, Shader* pCurrentShaderUse)
{
    pCurrentShaderUse->setMat4("view", camToUse.getView().e);
    pCurrentShaderUse->setMat4("projection", camToUse.getProjection().e);

    if ((pCurrentShaderUse->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        std::vector<LightData> lightBuffer;

        for (auto&& light : lights)
        {
            light->addToLightToUseBuffer(lightBuffer);
        }

        pCurrentShaderUse->setLightBlock(lightBuffer, camToUse.getGameObject().getTransform().getGlobalPosition());
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
        Mat4 view = camToUse.getView();
        //suppress translation
        view.c[3].xyz = {0.f, 0.f, 0.f};
        pCurrentShaderUse->setMat4("view", view.e);
        pCurrentShaderUse->setMat4("projection", camToUse.getProjection().e);
    }
    /*
    if ((pCurrentShaderUse->getFeature() & SCALE_TIME_ACC) == SCALE_TIME_ACC)
    {
        pCurrentShaderUse->setFloat("scaledTimeAcc", TimeSystem::getAccumulateTime());
    }

    if ((pCurrentShaderUse->getFeature() & UNSCALED_TIME_ACC) == UNSCALED_TIME_ACC)
    {
        pCurrentShaderUse->setFloat("unscaledTimeAcc", TimeSystem::getAccumulateUnscaledTime());
    }*/
}

void sendModelDataToShader(Camera& camToUse, ModelPart& modelPart)
{
    Shader* pShader = modelPart.pModel->getpShader();

    if ((pShader->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        Mat3 inverseModelMatrix3(GPM::toMatrix3(modelPart.pModel->getGameObject().getTransform().getModelMatrix().inversed()));

        pShader->setMat4("projectViewModelMatrix", (camToUse.getProjection() * camToUse.getView() * modelPart.pModel->getGameObject().getTransform().getModelMatrix()).e);
        pShader->setMat3("inverseModelMatrix", inverseModelMatrix3.e);
    }

    pShader->setMat4("model", modelPart.pModel->getGameObject().getTransform().getModelMatrix().e);
   

    if ((pShader->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        pShader->setMaterialBlock(modelPart.pMaterialToUse->getMaterialComponent());
    }
    
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

    sendDataToInitShader(*m_pCameras[0], m_pLights ,m_currentPShaderUse);
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
    glEnable(GL_DEPTH_TEST); 

    glClearColor(0.3f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        sendModelDataToShader(*m_pCameras[0], modelPart);
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


void RenderSystem::addLight(Light* pLight) noexcept
{
    m_pLights.push_back(pLight);
}

void RenderSystem::updateLightPointer(Light* newPointorLight, Light* exPointorLight) noexcept
{
    for (std::vector<Light*>::iterator it = m_pLights.begin(); it != m_pLights.end(); it++)
    {
        if ((*it) == exPointorLight)
        {
            *it = newPointorLight;
            return;
        }
    }
}

void RenderSystem::removeLight(Light* pLight) noexcept
{
    for (std::vector<Light*>::iterator it = m_pLights.begin(); it != m_pLights.end(); it++)
    {
        if ((*it) == pLight)
        {
            std::swap<Light*>(m_pLights.back(), (*it));
            m_pLights.pop_back();
            return;
        }
    }
}