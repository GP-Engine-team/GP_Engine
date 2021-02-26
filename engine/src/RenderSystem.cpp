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

void displayBoundingShape(const SubModel* pSubModel)
{
    const Volume* pBoudingVolume = pSubModel->pMesh->getBoundingVolume();

    const Sphere* pBoudingSphere = dynamic_cast<const Sphere*>(pBoudingVolume);
    if (pBoudingSphere)
    {
        float maxScale = std::max(std::max(pSubModel->pModel->getOwner().getTransform().getScale().x,
                                           pSubModel->pModel->getOwner().getTransform().getScale().y),
                                  pSubModel->pModel->getOwner().getTransform().getScale().z);

        RenderSystem::getInstance()->drawDebugSphere( pSubModel->pModel->getOwner().getTransform().getGlobalPosition(), pBoudingSphere->getRadius() * maxScale, ColorRGBA{1.f, 1.f, 0.f, 0.5f});
    }
}

RenderSystem::RenderSystem() noexcept
{
    m_localResources.add<Shader>("UniqueColor", "./resources/shaders/vSimpleColor.vs",
                                 "./resources/shaders/fSimpleColor.fs");

    m_localResources.add<Shader>("PostProcess", "./resources/shaders/vPostProcess.vs",
                                 "./resources/shaders/fPostProcess.fs");

    m_localResources.add<Mesh>("ScreenPlan", Mesh::createQuad(1.f, 1.f, 1.f, 0, 0, Mesh::Axis::NEG_Z));
    m_localResources.add<Mesh>("Sphere", Mesh::createSphere(5, 5));
    m_localResources.add<Mesh>("Cube", Mesh::createCube());
    m_localResources.add<Mesh>("Plane", Mesh::createQuad(1.f, 1.f, 1.f, 0, 0, Mesh::Axis::Y, true));
}

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
        return (SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.leftFace) &&
                SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.rightFace) &&
                SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.frontFace) &&
                SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.bottomFace) &&
                SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.topFace) &&
                SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.backFace));
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
    return [](const ResourceManagerType& rm, const LocalResourceManager& rml, RenderSystem& rs,
              std::vector<Renderer*>& pRenderers,
              std::vector<SubModel*>& pOpaqueSubModels, std::vector<SubModel*>& pTransparenteSubModels,
              std::vector<Camera*>& pCameras, std::vector<Light*>& pLights, std::vector<DebugShape>& debugShape)

    {
        int h, w;
        pRenderers[0]->getWindow()->getSize(w, h);

        Frustum camFrustum = pCameras[0]->getFrustum();

        rs.resetCurrentRenderPassKey();

        glViewport(0, 0, w, h);
        glBindFramebuffer(GL_FRAMEBUFFER, rml.get<RenderTexture>("FBO")->getID());
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.3f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*Display opaque element*/
        {
            glDisable(GL_BLEND);

            for (auto&& pSubModel : pOpaqueSubModels)
            {
                if (!rs.isOnFrustum(camFrustum, pSubModel))
                {
                    continue;
                }
                displayBoundingShape(pSubModel);

                rs.tryToBindShader(*pSubModel->pShader);
                rs.tryToBindMesh(pSubModel->pMesh->getID());
                rs.tryToBindTexture(pSubModel->pMaterial->getDiffuseTexture()->getID());
                rs.tryToSetBackFaceCulling(pSubModel->enableBackFaceCulling);

                // TODO: To optimize ! Use Draw instanced Array

                rs.sendModelDataToShader(*pCameras[0], *pSubModel);
                rs.drawModelPart(*pSubModel);
            }
        }

        /*Display transparent element*/
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            std::map<float, SubModel*> mapElemSortedByDistance;

            for (auto&& pSubModel : pTransparenteSubModels)
            {
                if (rs.isOnFrustum(camFrustum, pSubModel))
                {
                    float distance = (pCameras[0]->getOwner().getTransform().getGlobalPosition() -
                                      (pSubModel->pModel->getOwner().getTransform().getGlobalPosition()))
                                         .length2();
                    mapElemSortedByDistance[distance] = pSubModel;
                }
            }

            std::map<float, SubModel*>::reverse_iterator rEnd = mapElemSortedByDistance.rend();
            for (std::map<float, SubModel*>::reverse_iterator it = mapElemSortedByDistance.rbegin(); it != rEnd; ++it)
            {
                displayBoundingShape(it->second);

                rs.tryToBindShader(*it->second->pShader);
                rs.tryToBindMesh(it->second->pMesh->getID());
                rs.tryToBindTexture(it->second->pMaterial->getDiffuseTexture()->getID());
                rs.tryToSetBackFaceCulling(it->second->enableBackFaceCulling);

                // TODO: To optimize ! Use Draw instanced Array

                rs.sendModelDataToShader(*pCameras[0], *it->second);
                rs.drawModelPart(*it->second);
            };
        }

        // Draw debug shape
        {
            if (!debugShape.empty())
            {
                const Shader* shaderToUse = rml.get<Shader>("UniqueColor");
                glUseProgram(shaderToUse->getID());
                shaderToUse->setMat4("view", pCameras[0]->getView().e);
                shaderToUse->setMat4("projection", pCameras[0]->getProjection().e);

                for (auto&& shape : debugShape)
                {
                    shaderToUse->setMat4("model", shape.transform.model.e);
                    shaderToUse->setVec4("Color", shape.color.r, shape.color.g, shape.color.b, shape.color.a);

                    rs.tryToBindMesh(shape.shape->getID());


                    glDrawArrays(GL_TRIANGLES, 0, shape.shape->getVerticesCount());
                }

                debugShape.clear();
            }
        }

        // Render to screen
        {
            glViewport(0, 0, w, h);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glBindVertexArray(rml.get<Mesh>("ScreenPlan")->getID());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(rml.get<Shader>("PostProcess")->getID());
            glUniform1i(glGetUniformLocation(rml.get<Shader>("PostProcess")->getID(), "colorTexture"), 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, rml.get<Texture>("ColorBufferFBO")->getID());

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    };
}

void RenderSystem::draw(const ResourceManagerType& res, RenderPipeline renderPipeline) noexcept
{
    renderPipeline(res, m_localResources, *this, m_pRenderers, m_pOpaqueSubModels, m_pTransparenteSubModels, m_pCameras, m_pLights,
                   m_debugShape);
}

void RenderSystem::drawDebugSphere(Vec3 position, float radius, ColorRGBA color) noexcept
{
    m_debugShape.emplace_back(DebugShape{
        m_localResources.get<Mesh>("Sphere"), GPM::toTransform(SplitTransform{Quat::identity(), position, Vec3(radius)}), color});
}

void RenderSystem::drawDebugCube(GPM::Vec3 position, GPM::Quat rotation, GPM::Vec3 scale, ColorRGBA color) noexcept
{
    m_debugShape.emplace_back(DebugShape{m_localResources.get<Mesh>("Cube"),
                                         GPM::toTransform(SplitTransform{rotation, position, scale}), color});
}

void RenderSystem::drawDebugQuad(GPM::Vec3 position, GPM::Quat rotation, GPM::Vec3 scale, ColorRGBA color) noexcept
{
    m_debugShape.emplace_back(DebugShape{m_localResources.get<Mesh>("Plane"),
                                         GPM::toTransform(SplitTransform{rotation, position, scale}), color});
}

void RenderSystem::addRenderer(Renderer* pRenderer) noexcept
{
    m_pRenderers.push_back(pRenderer);

    int h, w;
    pRenderer->getWindow()->getSize(w, h);

    Texture::CreateArg textureArg;
    textureArg.width  = w;
    textureArg.height = h;

    RenderBuffer::CreateArg depthBufferArg;
    depthBufferArg.width          = w;
    depthBufferArg.height         = h;
    depthBufferArg.internalFormat = RenderBuffer::EInternalFormat::DEPTH_COMPONENT24;

    RenderTexture::CreateArg renderArg;
    renderArg.colorBuffers.emplace_back(&m_localResources.add<Texture>("ColorBufferFBO", textureArg));
    renderArg.depthBuffer = &m_localResources.add<RenderBuffer>("depthBufferFBO", depthBufferArg);
    m_localResources.add<RenderTexture>("FBO", renderArg);
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
    if (pSubModel->pMaterial->isOpaque())
    {
        m_pOpaqueSubModels.insert(std::upper_bound(m_pOpaqueSubModels.begin(), m_pOpaqueSubModels.end(), pSubModel,
                                                   isSubModelHasPriorityOverAnother),
                                  pSubModel);
    }
    else
    {
        m_pTransparenteSubModels.emplace_back(pSubModel);
    }
}

void RenderSystem::updateSubModelPointer(SubModel* newPointerSubModel, SubModel* exPointerSubModel) noexcept
{
    if (newPointerSubModel->pMaterial->isOpaque())
    {
        for (std::vector<SubModel*>::iterator it = m_pOpaqueSubModels.begin(); it != m_pOpaqueSubModels.end(); ++it)
        {
            if ((*it) == exPointerSubModel)
            {
                *it = newPointerSubModel;
                return;
            }
        }
    }
    else
    {
        for (std::vector<SubModel*>::iterator it = m_pTransparenteSubModels.begin();
             it != m_pTransparenteSubModels.end(); ++it)
        {
            if ((*it) == exPointerSubModel)
            {
                *it = newPointerSubModel;
                return;
            }
        }
    }
}

void RenderSystem::removeSubModel(SubModel* pSubModel) noexcept
{
    if (pSubModel->pMaterial->isOpaque())
    {
        m_pOpaqueSubModels.erase(std::remove(m_pOpaqueSubModels.begin(), m_pOpaqueSubModels.end(), pSubModel),
                                 m_pOpaqueSubModels.end());
    }
    else
    {
        m_pTransparenteSubModels.erase(
            std::remove(m_pTransparenteSubModels.begin(), m_pTransparenteSubModels.end(), pSubModel),
            m_pTransparenteSubModels.end());
    }
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