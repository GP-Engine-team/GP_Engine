/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <functional> //std::function
#include <vector>     //std::vector

#include "Engine/Resources/ResourcesManager.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"
#include "GPM/Transform.hpp"

namespace GPE
{
class Renderer;
class Light;
class Camera;
struct Frustum;
class Model;
class Shader;

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
    };

    struct DebugLine
    {
        GPM::Vec3 pt1;
        GPM::Vec3 pt2;
        float     width;
        ColorRGBA color  = ColorRGBA{1.f, 0.f, 0.f, 0.5f};
        bool      smooth = true;
    };

    using RenderPipeline =
        std::function<void(const ResourceManagerType&, RenderSystem&, std::vector<Renderer*>&, std::vector<SubModel*>&,
                           std::vector<SubModel*>&, std::vector<Camera*>&, std::vector<Light*>&,
                           std::vector<DebugShape>&, std::vector<DebugLine>&, Camera&)>;

protected:
    std::vector<Renderer*>  m_pRenderers;
    std::vector<SubModel*>  m_pOpaqueSubModels;
    std::vector<SubModel*>  m_pTransparenteSubModels;
    std::vector<Camera*>    m_pCameras;
    std::vector<Light*>     m_pLights;
    std::vector<DebugShape> m_debugShape;
    std::vector<DebugLine>  m_debugLine;
    Camera*                 m_mainCamera = nullptr;

    unsigned int m_currentShaderID                  = 0;
    unsigned int m_currentTextureID                 = 0;
    unsigned int m_currentMaterialID                = 0;
    unsigned int m_currentMeshID                    = 0;
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

    void setMainCamera(Camera& newMainCamera) noexcept;

    void resetCurrentRenderPassKey();

    bool isOnFrustum(const Frustum& camFrustum, const SubModel* pSubModel) const noexcept;
    void drawModelPart(const SubModel& subModel);
    void sendModelDataToShader(Camera& camToUse, Shader& shader, SubModel& subModel);
    void sendDataToInitShader(Camera& camToUse, Shader* pCurrentShaderUse);

    RenderPipeline defaultRenderPipeline() const noexcept;
    RenderPipeline gameObjectIdentifierPipeline() const noexcept;
    void           draw(const ResourceManagerType& res, RenderPipeline renderPipeline) noexcept;

    void drawDebugSphere(const GPM::Vec3& position, float radius,
                         const ColorRGBA& color = ColorRGBA{0.5f, 0.f, 0.f, 0.5f},
                         EDebugShapeMode mode = EDebugShapeMode::FILL, bool enableBackFaceCullling = true) noexcept;
    void drawDebugCube(const GPM::Vec3& position, const GPM::Quat& rotation, const GPM::Vec3& scale,
                       const ColorRGBA& color = ColorRGBA{0.5f, 0.f, 0.f, 0.5f},
                       EDebugShapeMode mode = EDebugShapeMode::FILL, bool enableBackFaceCullling = true) noexcept;
    void drawDebugQuad(const GPM::Vec3& position, const GPM::Vec3& dir, const GPM::Vec3& scale,
                       const ColorRGBA& color = ColorRGBA{0.5f, 0.f, 0.f, 0.5f},
                       EDebugShapeMode mode = EDebugShapeMode::FILL, bool enableBackFaceCullling = true) noexcept;

    void drawDebugLine(const GPM::Vec3& pt1, const GPM::Vec3& pt2, float width = 1.f,
                       const ColorRGBA& color = ColorRGBA{0.5f, 0.f, 0.f, 0.5f}, bool smooth = true) noexcept;

    void displayGameObjectRef(const GameObject& go, float dist = 100.f, float size = 10.f) noexcept;

    void displayBoundingVolume(const SubModel* pSubModel, const ColorRGBA& color) noexcept;

public:
    // TODO: Remove this shit and create variadic templated system
    void addRenderer(Renderer* renderer) noexcept;

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
};

} /*namespace GPE*/
