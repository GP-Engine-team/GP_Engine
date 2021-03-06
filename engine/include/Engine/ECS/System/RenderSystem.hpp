﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <functional> //std::function
#include <vector>     //std::vector

#include <Engine/Resources/ResourcesManager.hpp>
#include <Engine/Resources/ResourcesManagerType.hpp>
#include <Engine/Resources/ShadowMap.hpp>
#include <GPM/Transform.hpp>

namespace GPE
{
class Renderer;
class Light;
class Camera;
struct Frustum;
class Model;
class Shader;
class ParticleComponent;

class RenderSystem
{
public:
    enum class EDebugShapeMode
    {
        POINT = GL_POINT,
        LINE  = GL_LINE,
        FILL  = GL_FILL
    };

    enum class EDebugDrawShapeMode
    {
        POINTS    = GL_POINTS,
        LINES     = GL_LINES,
        TRAINGLES = GL_TRIANGLES
    };

    struct DebugShape
    {
        const Mesh*         shape                  = nullptr;
        GPM::Transform      transform              = {};
        ColorRGBA           color                  = ColorRGBA{1.f, 0.f, 0.f, 0.5f};
        EDebugShapeMode     mode                   = EDebugShapeMode::FILL;
        bool                enableBackFaceCullling = true;
        EDebugDrawShapeMode drawMode               = EDebugDrawShapeMode::TRAINGLES;
        float               duration               = 0.f;
    };

    /**
     * @brief Color is duplicate to optimize send data to GPU
     */
    struct DebugLine
    {
        struct Point
        {
            GPM::Vec3 pos;
            ColorRGB  col = ColorRGB::black();

            Point(const GPM::Vec3& in_pos, const ColorRGB& in_col = ColorRGB::black()) : pos{in_pos}, col{in_col}
            {
            }
        };

        Point pt1;
        Point pt2;
    };

    using RenderPipeline =
        std::function<void(RenderSystem&, std::vector<Renderer*>&, std::vector<SubModel*>&, std::vector<SubModel*>&,
                           std::vector<Camera*>&, std::vector<Light*>&, std::vector<ParticleComponent*>&,
                           std::vector<DebugShape>&, std::vector<DebugLine::Point>&, std::vector<ShadowMap>&, Camera*)>;

protected:
    inline static constexpr size_t  m_maxNbBones = 200;
    std::vector<Renderer*>          m_pRenderers;
    std::vector<SubModel*>          m_pOpaqueSubModels;
    std::vector<SubModel*>          m_pTransparenteSubModels;
    std::vector<Camera*>            m_pCameras;
    std::vector<Light*>             m_pLights;
    std::vector<ParticleComponent*> m_pParticleComponents;
    std::vector<DebugShape>         m_debugShape;
    std::vector<DebugLine::Point>   m_debugLine;
    std::vector<ShadowMap>          m_shadowMaps;

protected:
    unsigned int m_w = 0, m_h = 0;
    Camera*      m_mainCamera = nullptr;

    unsigned int m_currentShaderID                  = 0u;
    unsigned int m_currentTextureID                 = 0u;
    unsigned int m_currentMaterialID                = 0u;
    unsigned int m_currentMeshID                    = 0u;
    Shader*      m_currentPShaderUse                = nullptr;
    bool         m_currentBackFaceCullingModeEnable = false;

    Mesh* m_planeMesh  = nullptr;
    Mesh* m_sphereMesh = nullptr;
    Mesh* m_cubeMesh   = nullptr;

public:
    RenderSystem() noexcept;
    ~RenderSystem() noexcept;

    void tryToBindShader(Shader& shader);
    void tryToBindMaterial(Shader& shader, Material& material);
    void tryToBindTexture(unsigned int textureID);
    void tryToBindMesh(unsigned int meshID);
    void tryToSetBackFaceCulling(bool useBackFaceCulling);

    /**
     * @brief Try to affect the first camera found by the system in the scene.
     * @return
     */
    void    setDefaultMainCamera() noexcept;
    void    setMainCamera(Camera* newMainCamera) noexcept;
    Camera* getMainCamera() noexcept;

    void resetCurrentRenderPassKey();

    void renderFurstum(const Frustum& frustum, float size);
    bool isOnFrustum(const Frustum& camFrustum, const SubModel* pSubModel) const noexcept;
    void drawModelPart(const SubModel& subModel);
    void sendModelDataToShader(Camera& camToUse, Shader& shader, const GPM::Mat4& modelMatrix);
    void sendDataToInitShader(Camera& camToUse, Shader& shader);

    RenderPipeline defaultRenderPipeline() const noexcept;
    RenderPipeline mousePickingPipeline() const noexcept;

    void renderDebugShape(Camera& observer) noexcept;
    void renderDebugLine(Camera& observer) noexcept;
    void renderFrustumCulling() noexcept;
    void shadowMapPipeline() noexcept;

    /**
     * @brief Render the scene thanks to the call back set in input. This callback will be used as the render pipeline.
     * @param renderPipeline
     * @return
     */
    void render(RenderPipeline renderPipeline) noexcept;

    /**
     * @brief Update particles (to call once by frame)
     * @param dt
     * @return
     */
    void update(double dt) noexcept;
    void updateDebug(double dt) noexcept;

    void drawDebugSphere(const GPM::Vec3& position, float radius,
                         const ColorRGBA& color = ColorRGBA{0.5f, 0.f, 0.f, 0.5f}, float duration = 0.f,
                         EDebugShapeMode mode = EDebugShapeMode::FILL, bool enableBackFaceCullling = true) noexcept;
    void drawDebugCube(const GPM::Vec3& position, const GPM::Quat& rotation, const GPM::Vec3& scale,
                       const ColorRGBA& color = ColorRGBA{0.5f, 0.f, 0.f, 0.5f}, float duration = 0.f,
                       EDebugShapeMode mode = EDebugShapeMode::FILL, bool enableBackFaceCullling = true) noexcept;
    void drawDebugQuad(const GPM::Vec3& position, const GPM::Vec3& dir, const GPM::Vec3& scale,
                       const ColorRGBA& color = ColorRGBA{0.5f, 0.f, 0.f, 0.5f}, float duration = 0.f,
                       EDebugShapeMode mode = EDebugShapeMode::FILL, bool enableBackFaceCullling = false) noexcept;

    void drawDebugLine(const GPM::Vec3& pt1, const GPM::Vec3& pt2, const ColorRGB& color = ColorRGB::black()) noexcept;

    void displayGameObjectRef(const GameObject& go, float dist = 100.f) noexcept;

    void displayBoundingVolume(const SubModel* pSubModel, const ColorRGBA& color) noexcept;

public:
    void tryToResize(unsigned int w, unsigned int h);

    // TODO: Remove this shit and create variadic templated system
    void addParticleComponent(ParticleComponent& particleComponent) noexcept;

    void updateParticleComponentPointer(ParticleComponent* newPointerParticleComponent,
                                        ParticleComponent* exPointerParticleComponent) noexcept;

    void removeParticleComponent(ParticleComponent& particleComponent) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addRenderer(Renderer& renderer) noexcept;

    void updateRendererPointer(Renderer* newPointerRenderer, Renderer* exPointerRenderer) noexcept;

    void removeRenderer(Renderer& renderer) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addSubModel(SubModel& subModel) noexcept;

    void updateSubModelPointer(SubModel* newPointerSubModel, SubModel* exPointerSubModel) noexcept;

    void removeSubModel(SubModel& subModel) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addCamera(Camera& camera) noexcept;

    void updateCameraPointer(Camera* newPointerCamera, Camera* exPointerCamera) noexcept;

    void removeCamera(Camera& camera) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addLight(Light& light) noexcept;

    void updateLightPointer(Light* newPointerLight, Light* exPointerLight) noexcept;

    void removeLight(Light& light) noexcept;

    void addShadowMap(Light& light) noexcept;
    void removeShadowMap(Light& light) noexcept;

    const ShadowMap* getShadowMap();
};

} /*namespace GPE*/
