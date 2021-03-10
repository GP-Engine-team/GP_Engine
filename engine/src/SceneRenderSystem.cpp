#include "Engine/ECS/System/SceneRenderSystem.hpp"

#include <algorithm> //std::sort
#include <cstdio>
#include <map> //std::map
#include <memory>

#include "Engine/Intermediate/GameObject.hpp"
//#include "Engine/Core/System/TimeSystem.hpp"
#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Core/System/SystemsManager.hpp"
#include "Engine/Core/Tools/BranchPrediction.hpp"
#include "Engine/ECS/System/RenderSystem.hpp"
#include "Engine/Resources/Camera.hpp"
#include "Engine/Resources/Light/Light.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/Model.hpp"
#include "Engine/Resources/RenderBuffer.hpp"
#include "Engine/Resources/RenderTexture.hpp"
#include "Engine/Resources/Shader.hpp"
#include "GPM/Matrix4.hpp"
#include "GPM/Shape3D/AABB.hpp"
#include "GPM/Shape3D/Sphere.hpp"
#include "GPM/Shape3D/Volume.hpp"
#include "GPM/ShapeRelation/AABBPlane.hpp"
#include "GPM/ShapeRelation/SpherePlane.hpp"

using namespace GPE;
using namespace GPM;

void SceneRenderSystem::displayBoundingVolume(const SubModel* pSubModel, const ColorRGBA& color) noexcept
{
    switch (pSubModel->pMesh->getBoundingVolumeType())
    {
    case Mesh::EBoundingVolume::SPHERE: {
        const Sphere* pBoudingSphere = static_cast<const Sphere*>(pSubModel->pMesh->getBoundingVolume());

        float maxScale = std::max(std::max(pSubModel->pModel->getOwner().getTransform().getScale().x,
                                           pSubModel->pModel->getOwner().getTransform().getScale().y),
                                  pSubModel->pModel->getOwner().getTransform().getScale().z);

        const Vector3 pos(pSubModel->pModel->getOwner().getTransform().getGlobalPosition() +
                          pBoudingSphere->getCenter() * pSubModel->pModel->getOwner().getTransform().getScale());

        drawDebugSphere(pos, pBoudingSphere->getRadius() * (maxScale / 2.f), color,
                        SceneRenderSystem::EDebugShapeMode::FILL);

        break;
    }

    case Mesh::EBoundingVolume::AABB: {

        const AABB* pAABB = static_cast<const AABB*>(pSubModel->pMesh->getBoundingVolume());

        const Vector3 scale(pSubModel->pModel->getOwner().getTransform().getScale().x * pAABB->getExtI() * 2.f,
                            pSubModel->pModel->getOwner().getTransform().getScale().y * pAABB->getExtJ() * 2.f,
                            pSubModel->pModel->getOwner().getTransform().getScale().z * pAABB->getExtK() * 2.f);

        const Vector3 pos(pSubModel->pModel->getOwner().getTransform().getGlobalPosition() +
                          pAABB->getCenter() * pSubModel->pModel->getOwner().getTransform().getScale());

        drawDebugCube(pos, Quat::identity(), scale, color, SceneRenderSystem::EDebugShapeMode::FILL);
    }
    default:
        break;
    }
}

void SceneRenderSystem::displayGameObjectRef(const GameObject& go, float dist, float size) noexcept
{
    const Vec3& pos = go.getTransform().getGlobalPosition();

    drawDebugSphere(pos + go.getTransform().getVectorRight() * dist, size, ColorRGBA{1.f, 0.f, 0.f, 1.f});

    drawDebugSphere(pos + go.getTransform().getVectorUp() * dist, size, ColorRGBA{0.f, 1.f, 0.f, 1.f});

    drawDebugSphere(pos + go.getTransform().getVectorForward() * dist, size, ColorRGBA{0.f, 0.f, 1.f, 1.f});
}

SceneRenderSystem::SceneRenderSystem() noexcept
{
    m_localResources.add<Shader>("UniqueColor", "./resources/shaders/vSimpleColor.vs",
                                 "./resources/shaders/fSimpleColor.fs");

    m_localResources.add<Shader>("PostProcess", "./resources/shaders/vPostProcess.vs",
                                 "./resources/shaders/fPostProcess.fs");

    m_localResources.add<Mesh>("ScreenPlan", Mesh::createQuad(1.f, 1.f, 1.f, 0, 0, Mesh::Axis::NEG_Z));
    m_localResources.add<Mesh>("Sphere", Mesh::createSphere(5, 5));
    m_localResources.add<Mesh>("Cube", Mesh::createCube());
    m_localResources.add<Mesh>("Plane", Mesh::createQuad(1.f, 1.f, 1.f, 0, 0, Mesh::Axis::Z));

    SystemsManager::getInstance()->renderSystem.addSceneRenderSystem(this);
}

SceneRenderSystem::~SceneRenderSystem() noexcept
{
    SystemsManager::getInstance()->renderSystem.removeSceneRenderSystem(this);
}

bool SceneRenderSystem::isOnFrustum(const Frustum& camFrustum, const SubModel* pSubModel) const noexcept
{
    switch (pSubModel->pMesh->getBoundingVolumeType())
    {

    case Mesh::EBoundingVolume::SPHERE: {
        const Sphere* pBoudingSphere = static_cast<const Sphere*>(pSubModel->pMesh->getBoundingVolume());

        float maxScale = std::max(std::max(pSubModel->pModel->getOwner().getTransform().getScale().x,
                                           pSubModel->pModel->getOwner().getTransform().getScale().y),
                                  pSubModel->pModel->getOwner().getTransform().getScale().z);

        Sphere globalSphere(pBoudingSphere->getRadius() * (maxScale / 2.f),
                            pSubModel->pModel->getOwner().getTransform().getGlobalPosition() +
                                pBoudingSphere->getCenter() * pSubModel->pModel->getOwner().getTransform().getScale());

        return (SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.leftFace) &&
                SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.rightFace) &&
                SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.topFace) &&
                SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.bottomFace) &&
                SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.farFace) &&
                SpherePlane::isSphereOnOrForwardPlaneCollided(globalSphere, camFrustum.nearFace));

        break;
    }

    case Mesh::EBoundingVolume::AABB: {

        const AABB* pAABB = static_cast<const AABB*>(pSubModel->pMesh->getBoundingVolume());

        const Vector3 scale(pSubModel->pModel->getOwner().getTransform().getScale().x * pAABB->getExtI() * 2.f,
                            pSubModel->pModel->getOwner().getTransform().getScale().y * pAABB->getExtJ() * 2.f,
                            pSubModel->pModel->getOwner().getTransform().getScale().z * pAABB->getExtK() * 2.f);

        const Vector3 pos(pSubModel->pModel->getOwner().getTransform().getGlobalPosition() +
                          pAABB->getCenter() * pSubModel->pModel->getOwner().getTransform().getScale());

        AABB globalAABB(pos, scale.x / 2.f, scale.y / 2.f, scale.z / 2.f);

        return (AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.leftFace) &&
                AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.rightFace) &&
                AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.topFace) &&
                AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.bottomFace) &&
                AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.farFace) &&
                AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.nearFace));
    }
    default:
        return true;
        break;
    }
}

void SceneRenderSystem::sendDataToInitShader(Camera& camToUse, Shader* pCurrentShaderUse)
{
    if ((pCurrentShaderUse->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        std::vector<LightData> lightBuffer;

        for (auto&& light : m_pLights)
        {
            light->addToLightToUseBuffer(lightBuffer);
        }

        pCurrentShaderUse->setLightBlock(lightBuffer, camToUse.getOwner().getTransform().getGlobalPosition());
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

void SceneRenderSystem::sendModelDataToShader(Camera& camToUse, SubModel& subModel)
{
    Shader* pShader = subModel.pShader;

    if ((pShader->getFeature() & SKYBOX) == SKYBOX)
    {
        Mat4 view = camToUse.getView();

        // suppress translation
        view.c[3].xyz = {0.f, 0.f, 0.f};

        // TODO : Replace by getProjectionView();
        pShader->setMat4(
            "projectViewModelMatrix",
            (camToUse.getProjection() * view * subModel.pModel->getOwner().getTransform().getModelMatrix()).e);
        pShader->setMat4("projection", camToUse.getProjection().e);
        pShader->setMat4("view", view.e);
    }
    else
    {
        // TODO : Replace by getProjectionView();
        pShader->setMat4("projectViewModelMatrix", (camToUse.getProjection() * camToUse.getView() *
                                                    subModel.pModel->getOwner().getTransform().getModelMatrix())
                                                       .e);
    }

    if ((pShader->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        Mat3 inverseModelMatrix3(
            toMatrix3(subModel.pModel->getOwner().getTransform().getModelMatrix().inversed()).transposed());

        pShader->setMat4("model", subModel.pModel->getOwner().getTransform().getModelMatrix().e);
        pShader->setMat3("inverseModelMatrix", inverseModelMatrix3.e);
    }

    if ((pShader->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        pShader->setMaterialBlock(subModel.pMaterial->getComponent());
    }

    if ((pShader->getFeature() & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
    {
        pShader->setVec4("Color", subModel.pMaterial->getComponent().ambient.kr,
                         subModel.pMaterial->getComponent().ambient.kg, subModel.pMaterial->getComponent().ambient.kb,
                         subModel.pMaterial->getComponent().ambient.ki);
    }
}

void SceneRenderSystem::drawModelPart(const SubModel& subModel)
{
    glDrawArrays(GL_TRIANGLES, 0, subModel.pMesh->getVerticesCount());
}

void SceneRenderSystem::tryToBindShader(Shader& shader)
{
    if (m_currentShaderID == shader.getID())
        return;

    glUseProgram(shader.getID());

    m_currentShaderID   = shader.getID();
    m_currentPShaderUse = &shader;

    sendDataToInitShader(*m_pCameras[0], m_currentPShaderUse);
}

void SceneRenderSystem::tryToBindTexture(unsigned int textureID)
{
    if (m_currentTextureID == textureID)
        return;

    glBindTexture(GL_TEXTURE_2D, textureID);

    m_currentTextureID = textureID;
}

void SceneRenderSystem::tryToBindMesh(unsigned int meshID)
{
    if (m_currentMeshID == meshID)
        return;

    glBindVertexArray(meshID);

    m_currentMeshID = meshID;
}

void SceneRenderSystem::tryToSetBackFaceCulling(bool useBackFaceCulling)
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

void SceneRenderSystem::resetCurrentRenderPassKey()
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

SceneRenderSystem::RenderPipeline SceneRenderSystem::defaultRenderPipeline() const noexcept
{
    return [](const ResourceManagerType& rm, const LocalResourceManager& rml, SceneRenderSystem& rs,
              std::vector<Renderer*>& pRenderers, std::vector<SubModel*>& pOpaqueSubModels,
              std::vector<SubModel*>& pTransparenteSubModels, std::vector<Camera*>& pCameras,
              std::vector<Light*>& pLights, std::vector<DebugShape>& debugShape, unsigned int renderTextureID)

    {
        glBindFramebuffer(GL_FRAMEBUFFER, renderTextureID);

        Frustum camFrustum = pCameras[0]->getFrustum();

        rs.resetCurrentRenderPassKey();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glClearColor(0.3f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*Display opaque element*/
        {
            glDisable(GL_BLEND);

            for (auto&& pSubModel : pOpaqueSubModels)
            {
                if (!rs.isOnFrustum(camFrustum, pSubModel))
                {
                    rs.displayBoundingVolume(pSubModel, ColorRGBA{1.f, 0.f, 0.f, 0.2f});
                    continue;
                }
                rs.displayBoundingVolume(pSubModel, ColorRGBA{1.f, 1.f, 0.f, 0.2f});

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

                for (auto&& shape : debugShape)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(shape.mode));
                    rs.tryToSetBackFaceCulling(shape.enableBackFaceCullling);

                    shaderToUse->setMat4(
                        "projectViewModelMatrix",
                        (pCameras[0]->getProjection() * pCameras[0]->getView() * shape.transform.model).e);

                    shaderToUse->setVec4("Color", shape.color.r, shape.color.g, shape.color.b, shape.color.a);

                    rs.tryToBindMesh(shape.shape->getID());

                    glDrawArrays(GL_TRIANGLES, 0, shape.shape->getVerticesCount());
                }

                debugShape.clear();
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    };
}

void SceneRenderSystem::draw(const ResourceManagerType& res, RenderPipeline renderPipeline,
                             unsigned int renderTextureID) noexcept
{
    renderPipeline(res, m_localResources, *this, m_pRenderers, m_pOpaqueSubModels, m_pTransparenteSubModels, m_pCameras,
                   m_pLights, m_debugShape, renderTextureID);
}

void SceneRenderSystem::drawDebugSphere(const Vec3& position, float radius, const ColorRGBA& color,
                                        EDebugShapeMode mode, bool enableBackFaceCullling) noexcept
{
    m_debugShape.emplace_back(DebugShape{m_localResources.get<Mesh>("Sphere"),
                                         toTransform(SplitTransform{Quat::identity(), position, Vec3(radius * 2.f)}),
                                         color, mode, enableBackFaceCullling});
}

void SceneRenderSystem::drawDebugCube(const Vec3& position, const Quat& rotation, const Vec3& scale,
                                      const ColorRGBA& color, EDebugShapeMode mode,
                                      bool enableBackFaceCullling) noexcept
{
    m_debugShape.emplace_back(DebugShape{m_localResources.get<Mesh>("Cube"),
                                         toTransform(SplitTransform{rotation, position, scale}), color, mode,
                                         enableBackFaceCullling});
}

void SceneRenderSystem::drawDebugQuad(const Vec3& position, const Vec3& dir, const Vec3& scale, const ColorRGBA& color,
                                      EDebugShapeMode mode, bool enableBackFaceCullling) noexcept
{
    m_debugShape.emplace_back(
        DebugShape{m_localResources.get<Mesh>("Plane"),
                   toTransform(SplitTransform{Quaternion::lookAt(Vec3::zero(), dir), position, scale}), color, mode,
                   enableBackFaceCullling});
}

void SceneRenderSystem::addRenderer(Renderer* pRenderer) noexcept
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

void SceneRenderSystem::updateRendererPointer(Renderer* newPointerRenderer, Renderer* exPointerRenderer) noexcept
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

void SceneRenderSystem::removeRenderer(Renderer* pRenderer) noexcept
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

void SceneRenderSystem::addSubModel(SubModel* pSubModel) noexcept
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

void SceneRenderSystem::updateSubModelPointer(SubModel* newPointerSubModel, SubModel* exPointerSubModel) noexcept
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

void SceneRenderSystem::removeSubModel(SubModel* pSubModel) noexcept
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

void SceneRenderSystem::addCamera(Camera* pCamera) noexcept
{
    m_pCameras.push_back(pCamera);
}

void SceneRenderSystem::updateCameraPointer(Camera* newPointerCamera, Camera* exPointerCamera) noexcept
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

void SceneRenderSystem::removeCamera(Camera* pCamera) noexcept
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

void SceneRenderSystem::addLight(Light* pLight) noexcept
{
    m_pLights.push_back(pLight);
}

void SceneRenderSystem::updateLightPointer(Light* newPointerLight, Light* exPointerLight) noexcept
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

void SceneRenderSystem::removeLight(Light* pLight) noexcept
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
