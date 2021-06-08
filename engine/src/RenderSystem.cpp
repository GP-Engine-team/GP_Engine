
#include "Engine/ECS/System/RenderSystem.hpp"

#include <algorithm> //std::sort
#include <cstdio>
#include <map> //std::map
#include <memory>

#include <Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp>
#include <Engine/Core/Rendering/Window/WindowGLFW.hpp>
#include <Engine/Core/Tools/BranchPrediction.hpp>
#include <Engine/ECS/Component/AnimationComponent.hpp>
#include <Engine/ECS/Component/Camera.hpp>
#include <Engine/ECS/Component/Light/Light.hpp>
#include <Engine/ECS/Component/Model.hpp>
#include <Engine/ECS/Component/ParticleComponent.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Animation/Animation.hpp>
#include <Engine/Resources/Mesh.hpp>
#include <Engine/Resources/RenderBuffer.hpp>
#include <Engine/Resources/RenderTexture.hpp>
#include <Engine/Resources/Shader.hpp>
#include <GPM/Matrix4.hpp>
#include <GPM/Shape3D/AABB.hpp>
#include <GPM/Shape3D/Sphere.hpp>
#include <GPM/Shape3D/Volume.hpp>
#include <GPM/ShapeRelation/AABBPlane.hpp>
#include <GPM/ShapeRelation/SpherePlane.hpp>
#include <GPM/conversion.hpp>

#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

using namespace GPE;
using namespace GPM;

void RenderSystem::displayBoundingVolume(const SubModel* pSubModel, const ColorRGBA& color) noexcept
{
    switch (pSubModel->pMesh->getBoundingVolumeType())
    {
    case Mesh::EBoundingVolume::SPHERE: {
        const Sphere* pBoudingSphere = static_cast<const Sphere*>(pSubModel->pMesh->getBoundingVolume());

        const Vec3 globalScale = pSubModel->pModel->getOwner().getTransform().getGlobalScale();
        float maxScale = std::max(std::max(std::abs(globalScale.x), std::abs(globalScale.y)), std::abs(globalScale.z));

        const Vector4 posMat = pSubModel->pModel->getOwner().getTransform().getModelMatrix() *
                               Vector4(pBoudingSphere->getCenter(), 1.f).homogenized();
        const Vector3 pos{posMat.x, posMat.y, posMat.z};

        drawDebugSphere(pos, pBoudingSphere->getRadius() * (maxScale * 0.5f), color, 0.f,
                        RenderSystem::EDebugShapeMode::FILL);
    }
    break;

    case Mesh::EBoundingVolume::AABB: {

        const AABB* pAABB = static_cast<const AABB*>(pSubModel->pMesh->getBoundingVolume());

        const Vec3    globalScale = pSubModel->pModel->getOwner().getTransform().getGlobalScale();
        const Vector3 scale(std::abs(globalScale.x) * pAABB->extents.x * 2.f,
                            std::abs(globalScale.y) * pAABB->extents.y * 2.f,
                            std::abs(globalScale.z) * pAABB->extents.z * 2.f);

        const Vector4 posMat =
            pSubModel->pModel->getOwner().getTransform().getModelMatrix() * Vector4(pAABB->center, 1.f).homogenized();
        const Vector3 pos{posMat.x, posMat.y, posMat.z};

        drawDebugCube(pos, Quat::identity(), scale, color, 0.f, RenderSystem::EDebugShapeMode::FILL);
    }
    break;
    default:
        break;
    }
}

void RenderSystem::displayGameObjectRef(const GameObject& go, float dist) noexcept
{
    const Vec3& pos = go.getTransform().getGlobalPosition();
    drawDebugLine(Vec3::zero(), pos + go.getTransform().getVectorRight() * dist, ColorRGB::red());
    drawDebugLine(Vec3::zero(), pos + go.getTransform().getVectorUp() * dist, ColorRGB::green());
    drawDebugLine(Vec3::zero(), pos + go.getTransform().getVectorForward() * dist, ColorRGB::blue());
}

RenderSystem::RenderSystem() noexcept
{
    Shader* shader = &Engine::getInstance()->resourceManager.add<Shader>(
        "DefaultWithNormalMap", "./resources/shaders/vTextureWithLightAndShadowAndNM.vs",
        "./resources/shaders/fTextureWithLightAndShadowAndNMAndFog.fs", LIGHT_BLIN_PHONG | FOG);

    shader->use();
    shader->setInt("ourTexture", 0);
    shader->setInt("shadowMap", 1);
    shader->setInt("normalMap", 2);

    shader = &Engine::getInstance()->resourceManager.add<Shader>(
        "Default", "./resources/shaders/vTextureWithLightAndShadow.vs",
        "./resources/shaders/fTextureWithLightAndShadowAndFog.fs", LIGHT_BLIN_PHONG | FOG);

    shader->use();
    shader->setInt("ourTexture", 0);
    shader->setInt("shadowMap", 1);

    shader = &Engine::getInstance()->resourceManager.add<Shader>(
        "DefaultWithAnims", "./resources/shaders/vTextureWithLightAndShadowAndAnims.vs",
        "./resources/shaders/fTextureWithLightAndShadowAndFog.fs", LIGHT_BLIN_PHONG | FOG | ANIMATION_MASK);

    shader->use();
    shader->setInt("ourTexture", 0);
    shader->setInt("shadowMap", 1);

    shader = &Engine::getInstance()->resourceManager.add<Shader>(
        "DefaultWithAnimAndNormalMap", "./resources/shaders/vTextureWithLightAndShadowAndNMAndAnims.vs",
        "./resources/shaders/fTextureWithLightAndShadowAndNMAndFog.fs", LIGHT_BLIN_PHONG | FOG | ANIMATION_MASK);

    shader->use();
    shader->setInt("ourTexture", 0);
    shader->setInt("shadowMap", 1);
    shader->setInt("normalMap", 2);

    Engine::getInstance()->resourceManager.add<Shader>("UniqueColor", "./resources/shaders/vSimpleColor.vs",
                                                       "./resources/shaders/fSimpleColor.fs");

    Engine::getInstance()->resourceManager.add<Shader>("ColorMesh", "./resources/shaders/vColorMesh.vs",
                                                       "./resources/shaders/fColorMesh.fs");

    shader = &Engine::getInstance()->resourceManager.add<Shader>("DepthOnly", "./resources/shaders/vDepthOnly.vs",
                                                                 "./resources/shaders/fDepthOnly.fs",
                                                                 PROJECTION_VIEW_MODEL_MATRIX);
    shader->use();
    shader->setInt("depthMap", 0);

    m_sphereMesh = &Engine::getInstance()->resourceManager.add<Mesh>("Sphere", Mesh::createSphere(25, 25));
    m_cubeMesh   = &Engine::getInstance()->resourceManager.add<Mesh>("CubeDebug", Mesh::createCube());
    m_planeMesh  = &Engine::getInstance()->resourceManager.add<Mesh>(
        "Plane", Mesh::createQuad(1.f, 1.f, 1.f, 0, 0, Mesh::Axis::Z));
}

RenderSystem::~RenderSystem() noexcept
{
}

void RenderSystem::renderFurstum(const Frustum& frustum, float size)
{
    const vec3 pos     = m_mainCamera->getOwner().getTransform().getGlobalPosition();
    const vec3 up      = m_mainCamera->getOwner().getTransform().getVectorForward();
    const vec3 rigth   = m_mainCamera->getOwner().getTransform().getVectorRight();
    const vec3 forawrd = m_mainCamera->getOwner().getTransform().getVectorForward();

    const vec3 cLU = Vec3::cross(frustum.leftFace.getNormal(), up);
    drawDebugQuad(pos, frustum.leftFace.getNormal(), Vec3::one() * size, ColorRGBA{1.0, 0.5f, 0.5f, 0.8f});

    const vec3 cRU = Vec3::cross(frustum.rightFace.getNormal(), up);
    drawDebugQuad(pos, frustum.rightFace.getNormal(), Vec3::one() * size, ColorRGBA::red());

    const vec3 cTU = Vec3::cross(frustum.topFace.getNormal(), rigth);
    drawDebugQuad(pos, frustum.topFace.getNormal(), Vec3::one() * size, ColorRGBA::green());

    const vec3 cBU = Vec3::cross(frustum.bottomFace.getNormal(), rigth);
    drawDebugQuad(pos, frustum.bottomFace.getNormal(), Vec3::one() * size, ColorRGBA{0.5f, 1.0f, 0.5f, 0.8f});
}

bool RenderSystem::isOnFrustum(const Frustum& camFrustum, const SubModel* pSubModel) const noexcept
{
    switch (pSubModel->pMesh->getBoundingVolumeType())
    {

    case Mesh::EBoundingVolume::SPHERE: {
        const Sphere* pBoudingSphere = static_cast<const Sphere*>(pSubModel->pMesh->getBoundingVolume());

        const Vec3 globalScale = pSubModel->pModel->getOwner().getTransform().getGlobalScale();
        float maxScale = std::max(std::max(std::abs(globalScale.x), std::abs(globalScale.y)), std::abs(globalScale.z));

        const Vector4 posMat = pSubModel->pModel->getOwner().getTransform().getModelMatrix() *
                               Vector4(pBoudingSphere->getCenter(), 1.f).homogenized();
        const Vector3 pos{posMat.x, posMat.y, posMat.z};

        Sphere globalSphere(pBoudingSphere->getRadius() * (maxScale * 0.5f), pos);

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

        const Vec3    globalScale = pSubModel->pModel->getOwner().getTransform().getGlobalScale();
        const Vector3 scale(std::abs(globalScale.x) * pAABB->extents.x * 2.f,
                            std::abs(globalScale.y) * pAABB->extents.y * 2.f,
                            std::abs(globalScale.z) * pAABB->extents.z * 2.f);

        const Vector4 posMat =
            pSubModel->pModel->getOwner().getTransform().getModelMatrix() * Vector4(pAABB->center, 1.f).homogenized();
        const Vector3 pos{posMat.x, posMat.y, posMat.z};

        AABB globalAABB(pos, scale.x / 2.f, scale.y / 2.f, scale.z / 2.f);

        return (AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.leftFace) &&
                AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.rightFace) &&
                AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.topFace) &&
                AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.bottomFace) &&
                AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.farFace) &&
                AABBPlane::isAABBOnOrForwardPlane(globalAABB, camFrustum.nearFace));
        break;
    }
    default:
        return true;
        break;
    }
}

void RenderSystem::sendDataToInitShader(Camera& camToUse, Shader& shader)
{
    if ((shader.getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        std::vector<LightData> lightBuffer;

        for (auto&& light : m_pLights)
        {
            light->addToLightToUseBuffer(lightBuffer);
        }

        shader.setLightBlock(lightBuffer, camToUse.getOwner().getTransform().getGlobalPosition());

        shader.setInt("numberShadowUse", int(m_shadowMaps.size()));

        if (!m_shadowMaps.empty())
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_shadowMaps.front().depthMap);
        }
    }

    if ((shader.getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG && (shader.getFeature() & SKYBOX) != SKYBOX)
    {
        shader.setMat4("projectViewMatrix", camToUse.getProjectionView().e);

        if (m_shadowMaps.size())
        {
            shader.setInt("PCF", m_shadowMaps.front().pOwner->getShadowProperties().PCF);
            shader.setFloat("bias", m_shadowMaps.front().pOwner->getShadowProperties().bias);
            shader.setMat4("lightSpaceMatrix", m_shadowMaps.front().pOwner->getLightSpaceMatrix().e);
        }
        else
        {
            shader.setMat4("lightSpaceMatrix", Mat4::identity().e);
            shader.setInt("PCF", 1);
            shader.setFloat("bias", 0.0f);
        }
    }

    if ((shader.getFeature() & PROJECTION_MATRIX) == PROJECTION_MATRIX)
    {
        shader.setMat4("projectMatrix", camToUse.getProjection().e);
    }

    if ((shader.getFeature() & VIEW_MATRIX) == VIEW_MATRIX)
    {
        shader.setMat4("viewMatrix", camToUse.getView().e);
    }

    if ((shader.getFeature() & FOG) == FOG)
    {
        shader.setBool("fogParams.isEnabled", m_mainCamera->getFogParameter().isEnabled);
        if (m_mainCamera->getFogParameter().isEnabled)
        {
            shader.setVec3("fogParams.color", m_mainCamera->getFogParameter().color.r,
                           m_mainCamera->getFogParameter().color.g, m_mainCamera->getFogParameter().color.b);
            shader.setInt("fogParams.equation", m_mainCamera->getFogParameter().equation);
            switch (m_mainCamera->getFogParameter().equation)
            {
            case 0:
                if (m_mainCamera->getFogParameter().isStartFogEnable)
                    shader.setFloat("fogParams.linearStart", m_mainCamera->getFogParameter().linearStart);
                else
                    shader.setFloat("fogParams.linearStart", m_mainCamera->getNear());

                if (m_mainCamera->getFogParameter().isEndFogEnable)
                    shader.setFloat("fogParams.linearEnd", m_mainCamera->getFogParameter().linearEnd);
                else
                    shader.setFloat("fogParams.linearEnd", m_mainCamera->getFar());
                break;
            default:
                shader.setFloat("fogParams.density", m_mainCamera->getFogParameter().density);
                break;
            }
        }
    }

    if ((shader.getFeature() & SCALE_TIME_ACC) == SCALE_TIME_ACC)
    {
        shader.setFloat("scaledTimeAcc", static_cast<float>(Engine::getInstance()->timeSystem.getAccumulatedTime()));
    }

    if ((shader.getFeature() & UNSCALED_TIME_ACC) == UNSCALED_TIME_ACC)
    {
        shader.setFloat("unscaledTimeAcc",
                        static_cast<float>(Engine::getInstance()->timeSystem.getAccumulatedUnscaledTime()));
    }
}

void RenderSystem::sendModelDataToShader(Camera& camToUse, Shader& shader, const Mat4& modelMatrix)
{
    if ((shader.getFeature() & SKYBOX) == SKYBOX)
    {
        Mat4 view = camToUse.getView();

        // suppress translation
        view.c[3].xyz = {0.f, 0.f, 0.f};
        shader.setMat4("projectionView", (camToUse.getProjection() * view).e);
    }

    if ((shader.getFeature() & VIEW_MODEL_MATRIX) == VIEW_MODEL_MATRIX)
    {
        shader.setMat4("viewModelMatrix", (camToUse.getView() * modelMatrix).e);
    }

    if ((shader.getFeature() & PROJECTION_VIEW_MODEL_MATRIX) == PROJECTION_VIEW_MODEL_MATRIX)
    {
        shader.setMat4("projectViewModelMatrix", (camToUse.getProjectionView() * modelMatrix).e);
    }

    if ((shader.getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG && (shader.getFeature() & SKYBOX) != SKYBOX)
    {
        Mat3 inverseModelMatrix3(toMatrix3(modelMatrix.inversed()).transposed());

        shader.setMat4("model", modelMatrix.e);
        shader.setMat3("inverseModelMatrix", inverseModelMatrix3.e);
    }
}

void RenderSystem::drawModelPart(const SubModel& subModel)
{
    glDrawElements(GL_TRIANGLES, subModel.pMesh->getVerticesCount(), GL_UNSIGNED_INT, 0);
}

void RenderSystem::tryToBindShader(Shader& shader)
{
    if (m_currentShaderID == shader.getID())
        return;

    glUseProgram(shader.getID());

    m_currentShaderID   = shader.getID();
    m_currentPShaderUse = &shader;

    sendDataToInitShader(*m_mainCamera, shader);
}

void RenderSystem::tryToBindMaterial(Shader& shader, Material& material)
{
    if (m_currentMaterialID == material.getID())
        return;

    if ((shader.getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        shader.setMaterialBlock(material.getComponent());

        if (material.getNormalMapTexture())
        {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, material.getNormalMapTexture()->getID());
        }
    }

    if ((shader.getFeature() & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
    {
        shader.setVec4("Color", material.getComponent().ambient.kr, material.getComponent().ambient.kg,
                       material.getComponent().ambient.kb, material.getComponent().ambient.ki);
    }

    m_currentMaterialID = material.getID();
}

void RenderSystem::tryToBindTexture(unsigned int textureID)
{
    if (m_currentTextureID == textureID)
        return;

    glActiveTexture(GL_TEXTURE0);
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

void RenderSystem::setDefaultMainCamera() noexcept
{
    setMainCamera(m_pCameras.empty() ? nullptr : m_pCameras.at(0));
}

void RenderSystem::setMainCamera(Camera* newMainCamera) noexcept
{
    m_mainCamera = newMainCamera;
}

Camera* RenderSystem::getMainCamera() noexcept
{
    return m_mainCamera;
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

void RenderSystem::renderDebugShape(Camera& observer) noexcept
{
    if (!m_debugShape.empty())
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        const Shader* shaderToUse = Engine::getInstance()->resourceManager.get<Shader>("UniqueColor");
        glUseProgram(shaderToUse->getID());

        for (auto&& shape : m_debugShape)
        {
            glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(shape.mode));
            tryToSetBackFaceCulling(shape.enableBackFaceCullling);

            shaderToUse->setMat4("projectViewModelMatrix", (observer.getProjectionView() * shape.transform.model).e);

            shaderToUse->setVec4("Color", shape.color.r, shape.color.g, shape.color.b, shape.color.a);

            tryToBindMesh(shape.shape->getID());

            glDrawArrays(static_cast<GLenum>(shape.drawMode), 0, shape.shape->getVerticesCount());
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    resetCurrentRenderPassKey();
}

void RenderSystem::renderDebugLine(Camera& observer) noexcept
{
    // Draw debug line
    if (!m_debugLine.empty())
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const Shader* shaderToUse = Engine::getInstance()->resourceManager.get<Shader>("ColorMesh");
        glUseProgram(shaderToUse->getID());
        shaderToUse->setMat4("projectViewMatrix", observer.getProjectionView().e);

        tryToSetBackFaceCulling(false);
        glEnable(GL_LINE_SMOOTH);

        GLuint lineVAO, lineVBO;
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, m_debugLine.size() * sizeof(m_debugLine[0]), m_debugLine.data(), GL_STATIC_DRAW);

        // Pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(m_debugLine[0]),
                              (GLvoid*)offsetof(DebugLine::Point, pos));

        // Color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(m_debugLine[0]),
                              (GLvoid*)offsetof(DebugLine::Point, col));

        glDrawArrays(GL_LINES, 0, GLsizei(m_debugLine.size() * 2u));

        glDisable(GL_LINE_SMOOTH);
        glDeleteVertexArrays(1, &lineVAO);
        glDeleteBuffers(1, &lineVBO);
    }
    resetCurrentRenderPassKey();
}

void RenderSystem::renderFrustumCulling(FrustumRenderStats& stats) noexcept
{
    if (!m_mainCamera)
        return;

    Frustum camFrustum = m_mainCamera->getFrustum();

    // DEBUG
    for (auto&& pSubModel : m_pOpaqueSubModels)
    {
        stats.totalTriangleRequest += pSubModel->pMesh->getVerticesCount();
        stats.totalMeshRequest++;
        if (!isOnFrustum(camFrustum, pSubModel))
        {
            displayBoundingVolume(pSubModel, ColorRGBA{1.f, 0.f, 0.f, 0.2f});
            continue;
        }

        stats.numberTriangleDraw += pSubModel->pMesh->getVerticesCount();
        stats.numberMeshDraw++;
        displayBoundingVolume(pSubModel, ColorRGBA{1.f, 1.f, 0.f, 0.2f});
    }

    /*Display transparent element*/

    for (auto&& pSubModel : m_pTransparenteSubModels)
    {
        stats.totalTriangleRequest += pSubModel->pMesh->getVerticesCount();
        stats.totalMeshRequest++;
        if (!isOnFrustum(camFrustum, pSubModel))
        {
            displayBoundingVolume(pSubModel, ColorRGBA{1.f, 0.f, 0.f, 0.2f});
            continue;
        }

        stats.numberTriangleDraw += pSubModel->pMesh->getVerticesCount();
        stats.numberMeshDraw++;
        displayBoundingVolume(pSubModel, ColorRGBA{1.f, 1.f, 0.f, 0.2f});
    }
}

RenderSystem::RenderPipeline RenderSystem::defaultRenderPipeline() const noexcept
{
    return [](RenderSystem& rs, std::vector<Renderer*>& pRenderers, std::vector<SubModel*>& pOpaqueSubModels,
              std::vector<SubModel*>& pTransparenteSubModels, std::vector<Camera*>& pCameras,
              std::vector<Light*>& pLights, std::vector<ParticleComponent*>& pParticleComponents,
              std::vector<DebugShape>& debugShape, std::vector<DebugLine::Point>& debugLines,
              std::vector<ShadowMap>& shadowMaps, Camera* pMainCamera) {
        if (!pMainCamera)
            return;

        rs.shadowMapPipeline();

        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glClearColor(0.3f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Frustum camFrustum = pMainCamera->getFrustum();

        /*Display opaque element*/
        {
            glDisable(GL_BLEND);

            for (auto&& pSubModel : pOpaqueSubModels)
            {
                if (!rs.isOnFrustum(camFrustum, pSubModel))
                {
                    continue;
                }

                rs.tryToBindShader(*pSubModel->pShader);
                rs.tryToBindMaterial(*pSubModel->pShader, *pSubModel->pMaterial);
                rs.tryToBindMesh(pSubModel->pMesh->getID());
                rs.tryToBindTexture(pSubModel->pMaterial->getDiffuseTexture()->getID());
                rs.tryToSetBackFaceCulling(pSubModel->enableBackFaceCulling);

                // Animations
                if (pSubModel->isAnimated())
                {
                    auto& transforms = pSubModel->getFinalBonesTransforms();
                    glUniformMatrix4fv(glGetUniformLocation(pSubModel->pShader->getID(), "finalBonesMatrices"),
                                       transforms.size(), GL_FALSE, (GLfloat*)transforms.data());
                }
                else if ((pSubModel->pShader->getFeature() & ANIMATION_MASK) ==
                         ANIMATION_MASK) // if shader has animation data but does not play
                {
                    for (int i = 0; i < m_maxNbBones; ++i)
                    {

                        pSubModel->pShader->setMat4(
                            std::string("finalBonesMatrices[" + std::to_string(i) + "]").c_str(),
                            GPM::Matrix4::identity().e);
                    }
                }

                // TODO: To optimize ! Use Draw instanced Array

                rs.sendModelDataToShader(*pMainCamera, *pSubModel->pShader,
                                         pSubModel->pModel->getOwner().getTransform().getModelMatrix());
                rs.drawModelPart(*pSubModel);
            }
        }

        // Draw particles
        {
            for (auto&& particle : pParticleComponents)
            {
                const size_t count = particle->numAliveParticles();
                if (count == 0u)
                    continue;

                glEnable(GL_PROGRAM_POINT_SIZE);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDepthMask(!particle->isTransparente);

                if (particle->getShader())
                {
                    rs.tryToBindShader(*particle->getShader());
                    rs.sendModelDataToShader(*pMainCamera, *particle->getShader(),
                                             particle->getOwner().getTransform().getModelMatrix());

                    if (particle->getTexture())
                    {
                        rs.tryToBindTexture(particle->getTexture()->getID());
                    }
                }
                rs.tryToBindMesh(particle->getMeshID());

                glDrawArrays(GL_POINTS, 0, int(count));
            }
            glDepthMask(GL_TRUE);
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
                    float distance = (pMainCamera->getOwner().getTransform().getGlobalPosition() -
                                      (pSubModel->pModel->getOwner().getTransform().getGlobalPosition()))
                                         .sqrLength();
                    mapElemSortedByDistance[distance] = pSubModel;
                }
            }

            std::map<float, SubModel*>::reverse_iterator rEnd = mapElemSortedByDistance.rend();
            for (std::map<float, SubModel*>::reverse_iterator it = mapElemSortedByDistance.rbegin(); it != rEnd; ++it)
            {
                glDepthMask(it->second->writeInDepth);
                rs.tryToBindShader(*it->second->pShader);
                rs.tryToBindMaterial(*it->second->pShader, *it->second->pMaterial);
                rs.tryToBindMesh(it->second->pMesh->getID());
                rs.tryToBindTexture(it->second->pMaterial->getDiffuseTexture()->getID());
                rs.tryToSetBackFaceCulling(it->second->enableBackFaceCulling);

                // TODO: To optimize ! Use Draw instanced Array

                rs.sendModelDataToShader(*pMainCamera, *it->second->pShader,
                                         it->second->pModel->getOwner().getTransform().getModelMatrix());
                rs.drawModelPart(*it->second);
            };

            glDepthMask(GL_TRUE);
        }

        rs.resetCurrentRenderPassKey();
    };
}

RenderSystem::RenderPipeline RenderSystem::mousePickingPipeline() const noexcept
{
    return [](RenderSystem& rs, std::vector<Renderer*>& pRenderers, std::vector<SubModel*>& pOpaqueSubModels,
              std::vector<SubModel*>& pTransparenteSubModels, std::vector<Camera*>& pCameras,
              std::vector<Light*>& pLights, std::vector<ParticleComponent*>& pParticleComponents,
              std::vector<RenderSystem::DebugShape>& debugShape, std::vector<RenderSystem::DebugLine::Point>& debugLine,
              std::vector<ShadowMap>& shadowMaps, Camera* pMainCamera) {
        if (!pMainCamera)
            return;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glDisable(GL_MULTISAMPLE);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const Frustum camFrustum = pMainCamera->getFrustum();

        Shader& shaderGameObjectIdentifier =
            *Engine::getInstance()->resourceManager.get<Shader>("gameObjectIdentifier");
        shaderGameObjectIdentifier.use();

        /*Display opaque element*/
        {
            const GLint idLocation = glGetUniformLocation(shaderGameObjectIdentifier.getID(), "id");

            for (auto&& pSubModel : pOpaqueSubModels)
            {
                if ((pSubModel->pShader->getFeature() & SKYBOX) == SKYBOX || !rs.isOnFrustum(camFrustum, pSubModel))
                    continue;

                glUniform1ui(idLocation, pSubModel->pModel->getOwner().getID());

                rs.tryToBindMesh(pSubModel->pMesh->getID());
                rs.tryToSetBackFaceCulling(pSubModel->enableBackFaceCulling);

                shaderGameObjectIdentifier.setMat4(
                    "projectViewModelMatrix",
                    (pMainCamera->getProjectionView() * pSubModel->pModel->getOwner().getTransform().getModelMatrix())
                        .e);
                rs.drawModelPart(*pSubModel);
            }

            for (auto&& pSubModel : pTransparenteSubModels)
            {
                if ((pSubModel->pShader->getFeature() & SKYBOX) == SKYBOX || !rs.isOnFrustum(camFrustum, pSubModel))
                    continue;

                glUniform1ui(glGetUniformLocation(shaderGameObjectIdentifier.getID(), "id"),
                             pSubModel->pModel->getOwner().getID());

                rs.tryToBindMesh(pSubModel->pMesh->getID());
                rs.tryToSetBackFaceCulling(pSubModel->enableBackFaceCulling);

                shaderGameObjectIdentifier.setMat4(
                    "projectViewModelMatrix",
                    (pMainCamera->getProjectionView() * pSubModel->pModel->getOwner().getTransform().getModelMatrix())
                        .e);
                rs.drawModelPart(*pSubModel);
            }
        }

        rs.resetCurrentRenderPassKey();
    };
}

void RenderSystem::shadowMapPipeline() noexcept
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    GLint drawFboId = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
    Shader& shader = *Engine::getInstance()->resourceManager.get<Shader>("DepthOnly");
    shader.use();

    // glCullFace(GL_FRONT);

    for (auto&& shadowMap : m_shadowMaps)
    {
        // 1. first render to depth map
        glViewport(0, 0, shadowMap.width, shadowMap.height);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMap.depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowMap.depthMap);

        const Mat4 pv = shadowMap.pOwner->getLightSpaceMatrix();

        /*Display opaque element*/
        {
            for (auto&& pSubModel : m_pOpaqueSubModels)
            {
                if (!pSubModel->castShadow)
                    continue;

                tryToBindMesh(pSubModel->pMesh->getID());
                tryToSetBackFaceCulling(pSubModel->enableBackFaceCulling);

                const Mat4 pvm = pv * pSubModel->pModel->getOwner().getTransform().getModelMatrix();
                shader.setMat4("projectViewModelMatrix", pvm.e);
                drawModelPart(*pSubModel);
            }

            for (auto&& pSubModel : m_pTransparenteSubModels)
            {
                if (!pSubModel->castShadow)
                    continue;

                tryToBindMesh(pSubModel->pMesh->getID());
                tryToSetBackFaceCulling(pSubModel->enableBackFaceCulling);

                const Mat4 pvm = pv * pSubModel->pModel->getOwner().getTransform().getModelMatrix();
                shader.setMat4("projectViewModelMatrix", pvm.e);

                drawModelPart(*pSubModel);
            }
        }
    }
    // glCullFace(GL_BACK);

    // 2. then render scene as normal with shadow mapping (using depth map)
    glBindFramebuffer(GL_FRAMEBUFFER, drawFboId);
    glViewport(0, 0, m_w, m_h);
}

void RenderSystem::render(RenderPipeline renderPipeline) noexcept
{
    if (m_mainCamera == nullptr)
    {
        GPE_ASSERT(!m_pCameras.empty(), "Empty main camera");
        m_mainCamera = m_mainCamera ? m_mainCamera : m_pCameras[0];
    }

    renderPipeline(*this, m_pRenderers, m_pOpaqueSubModels, m_pTransparenteSubModels, m_pCameras, m_pLights,
                   m_pParticleComponents, m_debugShape, m_debugLine, m_shadowMaps, m_mainCamera);
}

void RenderSystem::updateDebug(double dt) noexcept
{
    // Update debug shape life time
    for (auto&& it = m_debugShape.begin(); it != m_debugShape.end();)
    {
        if ((it->duration -= float(dt)) <= 0.f)
            it = m_debugShape.erase(it);
        else
            ++it;
    }

    m_debugLine.clear();
}

void RenderSystem::update(double dt) noexcept
{
    // Update particle
    for (auto&& particle : m_pParticleComponents)
    {
        particle->update(dt);
    }
}

void RenderSystem::drawDebugSphere(const Vec3& position, float radius, const ColorRGBA& color, float duration,
                                   EDebugShapeMode mode, bool enableBackFaceCullling) noexcept
{
    m_debugShape.emplace_back(
        DebugShape{m_sphereMesh, toTransform(SplitTransform{Quat::identity(), position, Vec3(radius * 2.f)}), color,
                   mode, enableBackFaceCullling, EDebugDrawShapeMode::TRAINGLES, duration});
}

void RenderSystem::drawDebugCube(const Vec3& position, const Quat& rotation, const Vec3& scale, const ColorRGBA& color,
                                 float duration, EDebugShapeMode mode, bool enableBackFaceCullling) noexcept
{
    m_debugShape.emplace_back(DebugShape{m_cubeMesh, toTransform(SplitTransform{rotation, position, scale}), color,
                                         mode, enableBackFaceCullling, EDebugDrawShapeMode::TRAINGLES, duration});
}

void RenderSystem::drawDebugQuad(const Vec3& position, const Vec3& dir, const Vec3& scale, const ColorRGBA& color,
                                 float duration, EDebugShapeMode mode, bool enableBackFaceCullling) noexcept
{
    m_debugShape.emplace_back(
        DebugShape{m_planeMesh, toTransform(SplitTransform{Quaternion::lookAt(Vec3::zero(), dir), position, scale}),
                   color, mode, enableBackFaceCullling, EDebugDrawShapeMode::TRAINGLES, duration});
}

void RenderSystem::drawDebugLine(const GPM::Vec3& pt1, const GPM::Vec3& pt2, const ColorRGB& color) noexcept
{
    m_debugLine.emplace_back(pt1, color);
    m_debugLine.emplace_back(pt2, color);
}

void RenderSystem::addParticleComponent(ParticleComponent& particleComponent) noexcept
{
    m_pParticleComponents.push_back(&particleComponent);
}

void RenderSystem::updateParticleComponentPointer(ParticleComponent* newPointerParticleComponent,
                                                  ParticleComponent* exPointerParticleComponent) noexcept
{
    const std::vector<ParticleComponent*>::const_iterator end{m_pParticleComponents.end()};
    for (std::vector<ParticleComponent*>::iterator it = m_pParticleComponents.begin(); it != end; it++)
    {
        if ((*it) == exPointerParticleComponent)
        {
            *it = newPointerParticleComponent;
            return;
        }
    }
}

void RenderSystem::removeParticleComponent(ParticleComponent& particleComponent) noexcept
{
    const std::vector<ParticleComponent*>::const_iterator end{m_pParticleComponents.end()};
    for (std::vector<ParticleComponent*>::iterator it = m_pParticleComponents.begin(); it != end; it++)
    {
        if ((*it) == &particleComponent)
        {
            std::swap<ParticleComponent*>(m_pParticleComponents.back(), (*it));
            m_pParticleComponents.pop_back();
            return;
        }
    }
}

void RenderSystem::addRenderer(Renderer& renderer) noexcept
{
    m_pRenderers.push_back(&renderer);

    int h, w;
    renderer.getWindow()->getSize(w, h);

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

void RenderSystem::updateRendererPointer(Renderer* newPointerRenderer, Renderer* exPointerRenderer) noexcept
{
    const std::vector<Renderer*>::const_iterator end{m_pRenderers.end()};
    for (std::vector<Renderer*>::iterator it = m_pRenderers.begin(); it != end; it++)
    {
        if ((*it) == exPointerRenderer)
        {
            *it = newPointerRenderer;
            return;
        }
    }
}

void RenderSystem::removeRenderer(Renderer& renderer) noexcept
{
    const std::vector<Renderer*>::const_iterator end{m_pRenderers.end()};
    for (std::vector<Renderer*>::iterator it = m_pRenderers.begin(); it != end; it++)
    {
        if ((*it) == &renderer)
        {
            std::swap<Renderer*>(m_pRenderers.back(), (*it));
            m_pRenderers.pop_back();
            return;
        }
    }
}

void RenderSystem::addSubModel(SubModel& subModel) noexcept
{
    if (subModel.isTransparent)
    {
        // Will be sorted by distance, not by draw call type
        m_pTransparenteSubModels.emplace_back(&subModel);
    }
    else
    {
        m_pOpaqueSubModels.insert(std::upper_bound(m_pOpaqueSubModels.begin(), m_pOpaqueSubModels.end(), &subModel,
                                                   isSubModelHasPriorityOverAnother),
                                  &subModel);
    }
}

void RenderSystem::updateSubModelPointer(SubModel* newPointerSubModel, SubModel* exPointerSubModel) noexcept
{
    const std::vector<SubModel*>::const_iterator end{
        newPointerSubModel->pMaterial->isOpaque() ? m_pOpaqueSubModels.end() : m_pTransparenteSubModels.end()};

    for (std::vector<SubModel*>::iterator it = m_pTransparenteSubModels.begin(); it != end; ++it)
    {
        if ((*it) == exPointerSubModel)
        {
            *it = newPointerSubModel;
            return;
        }
    }
}

void RenderSystem::removeSubModel(SubModel& subModel) noexcept
{
    if (subModel.isTransparent)
    {
        auto it = std::find(m_pTransparenteSubModels.begin(), m_pTransparenteSubModels.end(), &subModel);
        if (it != m_pTransparenteSubModels.end())
            m_pTransparenteSubModels.erase(it);
    }
    else
    {
        auto it = std::find(m_pOpaqueSubModels.begin(), m_pOpaqueSubModels.end(), &subModel);
        if (it != m_pOpaqueSubModels.end())
            m_pOpaqueSubModels.erase(it);
    }
}

void RenderSystem::addCamera(Camera& camera) noexcept
{
    m_pCameras.emplace_back(&camera);
    m_mainCamera = &camera;
}

void RenderSystem::updateCameraPointer(Camera* newPointerCamera, Camera* exPointerCamera) noexcept
{
    const std::vector<Camera*>::const_iterator end{m_pCameras.end()};
    for (std::vector<Camera*>::iterator it = m_pCameras.begin(); it != end; it++)
    {
        if ((*it) == exPointerCamera)
        {
            *it = newPointerCamera;
            return;
        }
    }
}

void RenderSystem::removeCamera(Camera& camera) noexcept
{
    const std::vector<Camera*>::const_iterator end{m_pCameras.end()};
    for (std::vector<Camera*>::iterator it = m_pCameras.begin(); it != end; it++)
    {
        if ((*it) == &camera)
        {
            std::swap<Camera*>(m_pCameras.back(), (*it));
            m_pCameras.pop_back();
            return;
        }
    }
}

void RenderSystem::tryToResize(unsigned int w, unsigned int h)
{
    if (w == m_w && h == m_h)
        return;

    m_w = w;
    m_h = h;

    for (auto&& shadowMap : m_shadowMaps)
    {
        shadowMap.resize(w, h);
    }

    for (auto&& camera : m_pCameras)
    {
        camera->setAspect(Camera::computeAspect(w, h));
    }
}

void RenderSystem::addLight(Light& light) noexcept
{
    m_pLights.push_back(&light);
}

void RenderSystem::updateLightPointer(Light* newPointerLight, Light* exPointerLight) noexcept
{
    const std::vector<Light*>::const_iterator end{m_pLights.end()};
    for (std::vector<Light*>::iterator it = m_pLights.begin(); it != end; it++)
    {
        if ((*it) == exPointerLight)
        {
            *it = newPointerLight;
            return;
        }
    }
}

void RenderSystem::removeLight(Light& light) noexcept
{
    std::vector<Light*>::const_iterator end{m_pLights.end()};
    for (std::vector<Light*>::iterator it = m_pLights.begin(); it != end; it++)
    {
        if ((*it) == &light)
        {
            std::swap<Light*>(m_pLights.back(), (*it));
            m_pLights.pop_back();
            return;
        }
    }
}

void RenderSystem::addShadowMap(Light& light) noexcept
{
    // TODO : Enable user to use multiple shadow map
    if (m_shadowMaps.empty())
    {
        m_shadowMaps.emplace_back(light);
        m_shadowMaps.front().resize(m_w, m_h);
    }
    else
    {
        Log::getInstance()->logWarning("Multiple shadow map not implemented for now");
    }
}

void RenderSystem::removeShadowMap(Light& light) noexcept
{
    std::vector<ShadowMap>::const_iterator end{m_shadowMaps.end()};
    for (std::vector<ShadowMap>::iterator it = m_shadowMaps.begin(); it != end; it++)
    {
        if (it->pOwner == &light)
        {
            std::swap<ShadowMap>(m_shadowMaps.back(), (*it));
            m_shadowMaps.pop_back();
            return;
        }
    }
}

const ShadowMap* RenderSystem::getShadowMap()
{
    return m_shadowMaps.empty() ? nullptr : &m_shadowMaps.front();
}