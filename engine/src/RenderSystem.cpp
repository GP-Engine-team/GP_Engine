#include "Engine/Intermediate/RenderSystem.hpp"

#include <algorithm> //std::sort
#include <cstdio>
#include <map> //std::map
#include <memory>

#include "Engine/Intermediate/GameObject.hpp"
//#include "Engine/Core/System/TimeSystem.hpp"
#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Resources/Camera.hpp"
#include "Engine/Resources/Light/Light.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/Model.hpp"
#include "Engine/Resources/RenderBuffer.hpp"
#include "Engine/Resources/RenderTexture.hpp"
#include "Engine/Resources/Shader.hpp"
#include "GPM/Matrix4.hpp"
#include "GPM/Shape3D/Sphere.hpp"
#include "GPM/Shape3D/Volume.hpp"
#include "GPM/ShapeRelation/SpherePlane.hpp"

using namespace GPE;
using namespace GPM;

RenderSystem* RenderSystem::m_pInstance{nullptr};

bool RenderSystem::isOnFrustum(const Frustum& camFrustum, const SubModel* pSubModel)
{
    const Volume* pBoudingVolume = pSubModel->pMesh->getBoundingVolume();

    const Sphere* pBoudingSphere = dynamic_cast<const Sphere*>(pBoudingVolume);
    if (pBoudingSphere)
    {
        float maxScale = std::max(std::max(pSubModel->pModel->getOwner().getTransform().getScale().x,
                                           pSubModel->pModel->getOwner().getTransform().getScale().y),
                                  pSubModel->pModel->getOwner().getTransform().getScale().z);

        Sphere globalSphere(pBoudingSphere->getRadius() * maxScale,
                            pSubModel->pModel->getOwner().getTransform().getGlobalPosition() +
                                pBoudingSphere->getCenter());

        /*Sort to be more optimized*/
        if (SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.leftFace) &&
            SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.rightFace) &&
            SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.frontFace) &&
            SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.bottomFace) &&
            SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.topFace) &&
            SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.backFace))
        {

            return true;
        }
        return false;
    }

    return true;
}

void RenderSystem::sendDataToInitShader(Camera& camToUse, std::vector<Light*> lights, Shader* pCurrentShaderUse)
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

        pCurrentShaderUse->setLightBlock(lightBuffer, camToUse.getOwner().getTransform().getGlobalPosition());
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
        // suppress translation
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

void RenderSystem::sendModelDataToShader(Camera& camToUse, SubModel& subModel)
{
    Shader* pShader = subModel.pShader;

    if ((pShader->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        Mat3 inverseModelMatrix3(
            GPM::toMatrix3(subModel.pModel->getOwner().getTransform().getModelMatrix().inversed()));

        pShader->setMat4("projectViewModelMatrix", (camToUse.getProjection() * camToUse.getView() *
                                                    subModel.pModel->getOwner().getTransform().getModelMatrix())
                                                       .e);
        pShader->setMat3("inverseModelMatrix", inverseModelMatrix3.e);
    }

    pShader->setMat4("model", subModel.pModel->getOwner().getTransform().getModelMatrix().e);

    if ((pShader->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        pShader->setMaterialBlock(subModel.pMaterial->getMaterialComponent());
    }

    if ((pShader->getFeature() & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
    {
        pShader->setVec4("Color", subModel.pMaterial->getMaterialComponent().ambient.kr,
                         subModel.pMaterial->getMaterialComponent().ambient.kg,
                         subModel.pMaterial->getMaterialComponent().ambient.kb,
                         subModel.pMaterial->getMaterialComponent().ambient.ki);
    }
}

void RenderSystem::drawModelPart(const SubModel& subModel)
{
    glDrawArrays(GL_TRIANGLES, 0, subModel.pMesh->getVerticesCount());
}

void RenderSystem::tryToBindShader(Shader& shader)
{
    if (m_currentShaderID == shader.getID())
        return;

    glUseProgram(shader.getID());

    m_currentShaderID   = shader.getID();
    m_currentPShaderUse = &shader;

    sendDataToInitShader(*m_pCameras[0], m_pLights, m_currentPShaderUse);
}

void RenderSystem::tryToBindTexture(unsigned int textureID)
{
    if (m_currentTextureID == textureID)
        return;

    glBindTexture(GL_TEXTURE_2D, textureID);

    m_currentTextureID = textureID;
}

void RenderSystem::tryToBindMesh(unsigned int meshID)
{
    if (m_currentMeshID == meshID)
        return;

    glBindVertexArray(meshID);

    m_currentMeshID = meshID;
}

void RenderSystem::tryToSetBackFaceCulling(bool useBackFaceCulling)
{
    if (m_currentBackFaceCullingModeEnable == useBackFaceCulling)
        return;

    if (useBackFaceCulling)
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    m_currentBackFaceCullingModeEnable = useBackFaceCulling;
}

void RenderSystem::resetCurrentRenderPassKey()
{
    m_currentShaderID                  = 0;
    m_currentTextureID                 = 0;
    m_currentMeshID                    = 0;
    m_currentBackFaceCullingModeEnable = false;
    m_currentPShaderUse                = nullptr;

    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

RenderSystem::RenderPipeline RenderSystem::defaultRenderPipeline() const noexcept
{
    return [](const ResourceManagerType& rm, RenderSystem& rs, std::vector<Renderer*> pRenderers,
              std::vector<SubModel*> pSubModels, std::vector<Camera*> pCameras, std::vector<Light*> pLights)

    {
        int h, w;
        pRenderers[0]->getWindow()->getSize(w, h);

        //std::map<float, Model*> mapElemSortedByDistance;

        Frustum camFrustum = pCameras[0]->getFrustum();

        /*
        for (auto&& pSubModel : m_pSubModels)
        {
            if (pSubModel->isActivated() && isOnFrustum(camFrustum, pModel))
            {
                if (pSubModel->isOpaque())
                {

                }
                else
                {

                    float distance = (m_pCameras[0]->getGameObject().getTransform().getGlobalPosition() -
                                      (pModel->getGameObject().getTransform().getGlobalPosition())).length2();
                    mapElemSortedByDistance[distance] = pModel;
                }
            }
        }*/

        rs.resetCurrentRenderPassKey();
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, w, h);
        glBindFramebuffer(GL_FRAMEBUFFER, rm.get<RenderTexture>("FBO")->getID());

        glClearColor(0.3f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto&& pSubModel : pSubModels)
        {
            if (!rs.isOnFrustum(camFrustum, pSubModel))
                continue;

            rs.tryToBindShader(*pSubModel->pShader);
            rs.tryToBindMesh(pSubModel->pMesh->getID());
            rs.tryToBindTexture(pSubModel->pMaterial->getDiffuseTexture()->getID());
            rs.tryToSetBackFaceCulling(pSubModel->enableBackFaceCulling);

            // TODO: To optimize ! Use Draw instanced Array

            rs.sendModelDataToShader(*pCameras[0], *pSubModel);
            rs.drawModelPart(*pSubModel);
        }

        // Render to screen
        {
            glViewport(0, 0, w, h);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glBindVertexArray(rm.get<Mesh>("ScreenPlan")->getID());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(rm.get<Shader>("PostProcess")->getID());
            glUniform1i(glGetUniformLocation(rm.get<Shader>("PostProcess")->getID(), "colorTexture"), 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, rm.get<Texture>("ColorBufferFBO")->getID());

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glEnable(GL_BLEND);

        /*Display transparent element*/
        /*for(std::map<float, Model*>::reverse_iterator it = mapElemSortedByDistance.rbegin(); it !=
        mapElemSortedByDistance.rend(); ++it)
        {
            it->second->draw();
        };*/

        rs.resetCurrentRenderPassKey();
    };
}

void RenderSystem::draw(const ResourceManagerType& res, RenderPipeline renderPipeline) noexcept
{
    renderPipeline(res, *this, m_pRenderers, m_pSubModels, m_pCameras, m_pLights);
}

void RenderSystem::addRenderer(Renderer* pRenderer) noexcept
{
    m_pRenderers.push_back(pRenderer);
}

void RenderSystem::updateRendererPointer(Renderer* newPointerRenderer, Renderer* exPointerRenderer) noexcept
{
    for (std::vector<Renderer*>::iterator it = m_pRenderers.begin(); it != m_pRenderers.end(); it++)
    {
        if ((*it) == exPointerRenderer)
        {
            *it = newPointerRenderer;
            return;
        }
    }
}

void RenderSystem::removeRenderer(Renderer* pRenderer) noexcept
{
    for (std::vector<Renderer*>::iterator it = m_pRenderers.begin(); it != m_pRenderers.end(); it++)
    {
        if ((*it) == pRenderer)
        {
            std::swap<Renderer*>(m_pRenderers.back(), (*it));
            m_pRenderers.pop_back();
            return;
        }
    }
}

void RenderSystem::addSubModel(SubModel* pSubModel) noexcept
{
    m_pSubModels.insert(
        std::upper_bound(m_pSubModels.begin(), m_pSubModels.end(), pSubModel, isSubModelHasPriorityOverAnother),
        pSubModel);
}

void RenderSystem::updateSubModelPointer(SubModel* newPointerSubModel, SubModel* exPointerSubModel) noexcept
{
    for (std::vector<SubModel*>::iterator it = m_pSubModels.begin(); it != m_pSubModels.end(); ++it)
    {
        if ((*it) == exPointerSubModel)
        {
            *it = newPointerSubModel;
            return;
        }
    }
}

void RenderSystem::removeSubModel(SubModel* pSubModel) noexcept
{
    m_pSubModels.erase(std::remove(m_pSubModels.begin(), m_pSubModels.end(), pSubModel), m_pSubModels.end());
}

void RenderSystem::addCamera(Camera* pCamera) noexcept
{
    m_pCameras.push_back(pCamera);
}

void RenderSystem::updateCameraPointer(Camera* newPointerCamera, Camera* exPointerCamera) noexcept
{
    for (std::vector<Camera*>::iterator it = m_pCameras.begin(); it != m_pCameras.end(); it++)
    {
        if ((*it) == exPointerCamera)
        {
            *it = newPointerCamera;
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

void RenderSystem::updateLightPointer(Light* newPointerLight, Light* exPointerLight) noexcept
{
    for (std::vector<Light*>::iterator it = m_pLights.begin(); it != m_pLights.end(); it++)
    {
        if ((*it) == exPointerLight)
        {
            *it = newPointerLight;
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