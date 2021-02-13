#include "Engine/Core/System/RendererSystem.hpp"

#include <algorithm>
#include <memory>
#include <cstdio>
#include <bitset>

#include "Engine/Resources/Camera.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Core/System/TimeSystem.hpp"
#include "Engine/Core/Maths/mat.hpp"

using namespace Engine::Intermediate;
using namespace Engine::Resources;
using namespace GPM;

bool isOnCameraFrustum(const CameraFrustum& camFrustum, const Model* pModel)
{
    std::shared_ptr<Volume> pBoudingVolume = pModel->getpBoudingVolume();

    Sphere* pBoudingSphere = dynamic_cast<Sphere*>(pBoudingVolume.get());
    if (pBoudingSphere)
    {
        /*Sort to be more optimized*/
        if(     SpherePlane::isSphereOnOrForwardPlanCollided(*pBoudingSphere, camFrustum.leftFace) &&
                SpherePlane::isSphereOnOrForwardPlanCollided(*pBoudingSphere, camFrustum.rightFace) &&
                SpherePlane::isSphereOnOrForwardPlanCollided(*pBoudingSphere, camFrustum.frontFace) &&
                SpherePlane::isSphereOnOrForwardPlanCollided(*pBoudingSphere, camFrustum.bottomFace) &&
                SpherePlane::isSphereOnOrForwardPlanCollided(*pBoudingSphere, camFrustum.topFace) &&
                SpherePlane::isSphereOnOrForwardPlanCollided(*pBoudingSphere, camFrustum.backFace))
        {

            return true;
        }
        return false;
    }
    
    return true;
}

void sendDataToInitShader(Shader* pCurrentShaderUse)
{
    if ((pCurrentShaderUse->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        const std::vector<light>& lightBuffer = Light::getLightsToUseInAlignasStruct();
        pCurrentShaderUse->setLightBlock(lightBuffer, Camera::getCamUse()->getPosition());
    }
    else
    {
        if ((pCurrentShaderUse->getFeature() & SKYBOX) != SKYBOX)
        {
            pCurrentShaderUse->setMat4("view", &Camera::getCamUse()->getView().mat[0]);
            pCurrentShaderUse->setMat4("projection", &Camera::getCamUse()->getProjection().mat[0]);
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
    }
}

void sendModelDataToShader(ModelPart& modelPart)
{
    Shader* pShader = modelPart.pModel->getpShader();

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
    functGlCheckAndLogError();
}

void ModelRenderingSystem::tryToBindShader(Shader* pShader)
{
    if (currentShaderId == pShader->getIdProgramm())
        return;

    // TODO : Remove gl functions
    glUseProgram(pShader->getIdProgramm());
    functGlCheckAndLogError();

    currentShaderId = pShader->getIdProgramm();
    currentPShaderUse = pShader;

    sendDataToInitShader(currentPShaderUse);
}

void ModelRenderingSystem::tryToBindTexture(unsigned int textureId)
{
    if (currentTextureId == textureId)
        return;

    // TODO : Remove gl functions
    glBindTexture(GL_TEXTURE_2D, textureId);
    functGlCheckAndLogError();

    currentTextureId = textureId;
}

void ModelRenderingSystem::tryToBindMesh(unsigned int meshId)
{
    if (currentMeshId == meshId)
        return;

    // TODO : Remove gl functions
    glBindVertexArray(meshId);
    functGlCheckAndLogError();

    currentMeshId = meshId;
}

void ModelRenderingSystem::tryToSetBackFaceCulling(bool useBackFaceCulling)
{
    if (currentBackFaceCullingModeEnable == useBackFaceCulling)
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

    currentBackFaceCullingModeEnable = useBackFaceCulling;
}

void ModelRenderingSystem::resetCurrentRenderPassKey()
{
    currentShaderId         = 0;
    currentTextureId        = 0;
    currentMeshId           = 0;
    currentBackFaceCullingModeEnable = false;
    currentPShaderUse       = nullptr;

    // TODO : Remove gl functions
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    functGlCheckAndLogError();
}

void ModelRenderingSystem::draw () noexcept
{
    std::list<ModelPart> modelParts;
    std::map<float, Model*> mapElemSortedByDistance;

    CameraFrustum camFrustum = Camera::getCamUse()->getFrustum();

    for (auto &&pModel : pModels)
    {
        if (pModel->isActivated() && isOnCameraFrustum(camFrustum, pModel))
        {
            if (pModel->isOpaque())
            {
                pModel->insertModelPartsOnContenor(modelParts);
            }
            else
            {
                float distance = (Engine::LowRenderer::Camera::getCamUse()->getGlobalPosition() - (pModel->getGameObject().getGlobalPosition())).squartLength();
                mapElemSortedByDistance[distance] = pModel;
            }
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
    for(std::map<float, Model*>::reverse_iterator it = mapElemSortedByDistance.rbegin(); it != mapElemSortedByDistance.rend(); ++it) 
    {
        it->second->draw();
    };

    resetCurrentRenderPassKey();
}

void ModelRenderingSystem::addModel (Model* pModel) noexcept
{
    pModels.push_back(pModel);
}

void ModelRenderingSystem::updateModelPointor (Model* newPointorModel, Model* exPointorModel) noexcept
{
    for (std::vector<Model*>::iterator it = pModels.begin(); it != pModels.end(); it++)
    {
        if ((*it) == exPointorModel)
        {
            *it = newPointorModel;
            return;
        }
    }
}

void ModelRenderingSystem::removeModel (Model* pModel) noexcept
{
    for (std::vector<Model*>::iterator it = pModels.begin(); it != pModels.end(); it++)
    {
        if ((*it) == pModel)
        {
            std::swap<Model*>(pModels.back(), (*it));
            pModels.pop_back();
            return;
        }
    }
}

std::vector<Model*> ModelRenderingSystem::pModels {};
unsigned int ModelRenderingSystem::currentShaderId                  {0};
unsigned int ModelRenderingSystem::currentTextureId                 {0};
unsigned int ModelRenderingSystem::currentMeshId                    {0};
Shader*      ModelRenderingSystem::currentPShaderUse                {nullptr};
bool         ModelRenderingSystem::currentBackFaceCullingModeEnable {false};