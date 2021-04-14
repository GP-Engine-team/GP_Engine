#include "Engine/ECS/System/SceneRenderSystem.hpp"

#include <algorithm> //std::sort
#include <cstdio>
#include <map> //std::map
#include <memory>

#include "Engine/Intermediate/GameObject.hpp"
//#include "Engine/Core/System/TimeSystem.hpp"
#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Core/Tools/BranchPrediction.hpp"
#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/ECS/Component/Light/Light.hpp"
#include "Engine/ECS/Component/Model.hpp"
#include "Engine/ECS/System/RenderSystem.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Resources/Mesh.hpp"
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

        const Vector3 scale(pSubModel->pModel->getOwner().getTransform().getScale().x * pAABB->extents.x * 2.f,
                            pSubModel->pModel->getOwner().getTransform().getScale().y * pAABB->extents.y * 2.f,
                            pSubModel->pModel->getOwner().getTransform().getScale().z * pAABB->extents.z * 2.f);

        const Vector3 pos(pSubModel->pModel->getOwner().getTransform().getGlobalPosition() +
                          pAABB->center * pSubModel->pModel->getOwner().getTransform().getScale());

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
    Engine::getInstance()->resourceManager.add<Shader>("UniqueColor", "./resources/shaders/vSimpleColor.vs",
                                                       "./resources/shaders/fSimpleColor.fs");

    m_sphereMesh = &Engine::getInstance()->resourceManager.add<Mesh>("Sphere", Mesh::createSphere(5, 5));
    m_cubeMesh   = &Engine::getInstance()->resourceManager.add<Mesh>("CubeDebug", Mesh::createCube());
    m_planeMesh  = &Engine::getInstance()->resourceManager.add<Mesh>(
        "Plane", Mesh::createQuad(1.f, 1.f, 1.f, 0, 0, Mesh::Axis::Z));

    Engine::getInstance()->renderSystem.addSceneRenderSystem(this);
}

SceneRenderSystem::~SceneRenderSystem() noexcept
{
    Engine::getInstance()->renderSystem.removeSceneRenderSystem(this);
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

        const Vector3 scale(pSubModel->pModel->getOwner().getTransform().getScale().x * pAABB->extents.x * 2.f,
                            pSubModel->pModel->getOwner().getTransform().getScale().y * pAABB->extents.y * 2.f,
                            pSubModel->pModel->getOwner().getTransform().getScale().z * pAABB->extents.z * 2.f);

        const Vector3 pos(pSubModel->pModel->getOwner().getTransform().getGlobalPosition() +
                          pAABB->center * pSubModel->pModel->getOwner().getTransform().getScale());

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

void SceneRenderSystem::sendModelDataToShader(Camera& camToUse, Shader& shader, SubModel& subModel)
{
    if ((shader.getFeature() & SKYBOX) == SKYBOX)
    {
        Mat4 view = camToUse.getView();

        // suppress translation
        view.c[3].xyz = {0.f, 0.f, 0.f};

        shader.setMat4(
            "projectViewModelMatrix",
            (camToUse.getProjection() * view * subModel.pModel->getOwner().getTransform().getModelMatrix()).e);
        shader.setMat4("projection", camToUse.getProjection().e);
        shader.setMat4("view", view.e);
    }
    else
    {
        shader.setMat4("projectViewModelMatrix",
                       (camToUse.getProjectionView() * subModel.pModel->getOwner().getTransform().getModelMatrix()).e);
    }

    if ((shader.getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        Mat3 inverseModelMatrix3(
            toMatrix3(subModel.pModel->getOwner().getTransform().getModelMatrix().inversed()).transposed());

        shader.setMat4("model", subModel.pModel->getOwner().getTransform().getModelMatrix().e);
        shader.setMat3("inverseModelMatrix", inverseModelMatrix3.e);
    }
}

void SceneRenderSystem::drawModelPart(const SubModel& subModel)
{
    glDrawElements(GL_TRIANGLES, subModel.pMesh->getVerticesCount(), GL_UNSIGNED_INT, 0);
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

void SceneRenderSystem::tryToBindMaterial(Shader& shader, Material& material)
{
    if (m_currentMaterialID == material.getID())
        return;

    if ((shader.getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        shader.setMaterialBlock(material.getComponent());
    }

    if ((shader.getFeature() & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
    {
        shader.setVec4("Color", material.getComponent().ambient.kr, material.getComponent().ambient.kg,
                       material.getComponent().ambient.kb, material.getComponent().ambient.ki);
    }

    m_currentMaterialID = material.getID();
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
    return [](const ResourceManagerType& rm, SceneRenderSystem& rs, std::vector<Renderer*>& pRenderers,
              std::vector<SubModel*>& pOpaqueSubModels, std::vector<SubModel*>& pTransparenteSubModels,
              std::vector<Camera*>& pCameras, std::vector<Light*>& pLights, std::vector<DebugShape>& debugShape,
              std::vector<DebugLine>& debugLines)

    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glClearColor(0.3f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Frustum camFrustum = pCameras[0]->getFrustum();

        rs.resetCurrentRenderPassKey();

        /*Display opaque element*/
        {
            glDisable(GL_BLEND);

            for (auto&& pSubModel : pOpaqueSubModels)
            {
                if (!rs.isOnFrustum(camFrustum, pSubModel))
                {
                    // rs.displayBoundingVolume(pSubModel, ColorRGBA{1.f, 0.f, 0.f, 0.2f});
                    continue;
                }
                // rs.displayBoundingVolume(pSubModel, ColorRGBA{1.f, 1.f, 0.f, 0.2f});

                rs.tryToBindShader(*pSubModel->pShader);
                rs.tryToBindMaterial(*pSubModel->pShader, *pSubModel->pMaterial);
                rs.tryToBindMesh(pSubModel->pMesh->getID());
                rs.tryToBindTexture(pSubModel->pMaterial->getDiffuseTexture()->getID());
                rs.tryToSetBackFaceCulling(pSubModel->enableBackFaceCulling);

                // TODO: To optimize ! Use Draw instanced Array

                rs.sendModelDataToShader(*pCameras[0], *pSubModel->pShader, *pSubModel);
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
                                         .sqrLength();
                    mapElemSortedByDistance[distance] = pSubModel;
                }
            }

            std::map<float, SubModel*>::reverse_iterator rEnd = mapElemSortedByDistance.rend();
            for (std::map<float, SubModel*>::reverse_iterator it = mapElemSortedByDistance.rbegin(); it != rEnd; ++it)
            {
                rs.tryToBindShader(*it->second->pShader);
                rs.tryToBindMaterial(*it->second->pShader, *it->second->pMaterial);
                rs.tryToBindMesh(it->second->pMesh->getID());
                rs.tryToBindTexture(it->second->pMaterial->getDiffuseTexture()->getID());
                rs.tryToSetBackFaceCulling(it->second->enableBackFaceCulling);

                // TODO: To optimize ! Use Draw instanced Array

                rs.sendModelDataToShader(*pCameras[0], *it->second->pShader, *it->second);
                rs.drawModelPart(*it->second);
            };
        }

        // Draw debug shape
        {
            if (!debugShape.empty())
            {
                const Shader* shaderToUse = Engine::getInstance()->resourceManager.get<Shader>("UniqueColor");
                glUseProgram(shaderToUse->getID());

                for (auto&& shape : debugShape)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(shape.mode));
                    rs.tryToSetBackFaceCulling(shape.enableBackFaceCullling);

                    shaderToUse->setMat4("projectViewModelMatrix",
                                         (pCameras[0]->getProjectionView() * shape.transform.model).e);

                    shaderToUse->setVec4("Color", shape.color.r, shape.color.g, shape.color.b, shape.color.a);

                    rs.tryToBindMesh(shape.shape->getID());

                    glDrawArrays(static_cast<GLenum>(shape.drawMode), 0, shape.shape->getVerticesCount());
                }

                debugShape.clear();
            }

            // Draw debug line
            if (!debugLines.empty())
            {
                const Shader* shaderToUse = Engine::getInstance()->resourceManager.get<Shader>("UniqueColor");
                glUseProgram(shaderToUse->getID());
                rs.tryToSetBackFaceCulling(false);

                for (auto&& line : debugLines)
                {
                    GLfloat lineSeg[] = {
                        line.pt1.x, line.pt1.y, line.pt1.z, // first vertex
                        line.pt2.x, line.pt2.y, line.pt2.z  // second vertex
                    };

                    GLuint lineVAO, lineVBO;
                    glGenVertexArrays(1, &lineVAO);
                    glGenBuffers(1, &lineVBO);
                    glBindVertexArray(lineVAO);
                    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(lineSeg), &lineSeg, GL_STATIC_DRAW);
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

                    shaderToUse->setMat4("projectViewModelMatrix", pCameras[0]->getProjectionView().e);

                    shaderToUse->setVec4("Color", line.color.r, line.color.g, line.color.b, line.color.a);

                    if (line.smooth)
                        glEnable(GL_LINE_SMOOTH);
                    else
                        glDisable(GL_LINE_SMOOTH);

                    glBindVertexArray(lineVAO);
                    glLineWidth(line.width);
                    glDrawArrays(GL_LINES, 0, 2);

                    glLineWidth(1.0f);
                    glDisable(GL_LINE_SMOOTH);

                    glDeleteVertexArrays(1, &lineVAO);
                    glDeleteBuffers(1, &lineVBO);
                }

                debugLines.clear();
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    };
}

void SceneRenderSystem::draw(const ResourceManagerType& res, RenderPipeline renderPipeline) noexcept
{
    renderPipeline(res, *this, m_pRenderers, m_pOpaqueSubModels, m_pTransparenteSubModels, m_pCameras, m_pLights,
                   m_debugShape, m_debugLine);
}

void SceneRenderSystem::drawDebugSphere(const Vec3& position, float radius, const ColorRGBA& color,
                                        EDebugShapeMode mode, bool enableBackFaceCullling) noexcept
{
    m_debugShape.emplace_back(DebugShape{m_sphereMesh,
                                         toTransform(SplitTransform{Quat::identity(), position, Vec3(radius * 2.f)}),
                                         color, mode, enableBackFaceCullling});
}

void SceneRenderSystem::drawDebugCube(const Vec3& position, const Quat& rotation, const Vec3& scale,
                                      const ColorRGBA& color, EDebugShapeMode mode,
                                      bool enableBackFaceCullling) noexcept
{
    m_debugShape.emplace_back(DebugShape{m_cubeMesh, toTransform(SplitTransform{rotation, position, scale}), color,
                                         mode, enableBackFaceCullling});
}

void SceneRenderSystem::drawDebugQuad(const Vec3& position, const Vec3& dir, const Vec3& scale, const ColorRGBA& color,
                                      EDebugShapeMode mode, bool enableBackFaceCullling) noexcept
{
    m_debugShape.emplace_back(
        DebugShape{m_planeMesh, toTransform(SplitTransform{Quaternion::lookAt(Vec3::zero(), dir), position, scale}),
                   color, mode, enableBackFaceCullling});
}

void SceneRenderSystem::drawDebugLine(const GPM::Vec3& pt1, const GPM::Vec3& pt2, float width, const ColorRGBA& color,
                                      bool smooth) noexcept
{
    m_debugLine.emplace_back(DebugLine{pt1, pt2, width, color, smooth});
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
    renderArg.colorBuffers.emplace_back(
        &Engine::getInstance()->resourceManager.add<Texture>("ColorBufferFBO", textureArg));
    renderArg.depthBuffer = &Engine::getInstance()->resourceManager.add<RenderBuffer>("depthBufferFBO", depthBufferArg);
    Engine::getInstance()->resourceManager.add<RenderTexture>("FBO", renderArg);
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
        auto it = std::find(m_pOpaqueSubModels.begin(), m_pOpaqueSubModels.end(), pSubModel);
        if (it != m_pOpaqueSubModels.end())
            m_pOpaqueSubModels.erase(it);
    }
    else
    {
        auto it = std::find(m_pTransparenteSubModels.begin(), m_pTransparenteSubModels.end(), pSubModel);
        if (it != m_pOpaqueSubModels.end())
            m_pTransparenteSubModels.erase(it);
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