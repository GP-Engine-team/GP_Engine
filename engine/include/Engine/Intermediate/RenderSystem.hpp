/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <functional> //std::function
#include <vector>     //std::vector

#include "Engine/Core/Tools/BranchPrediction.hpp"
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

/**
 * @brief The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over. Thread safe singleton according with link below :
 * @see https://refactoring.guru/fr/design-patterns/singleton/cpp/example
 */
class RenderSystem
{
public:
    enum class EDebugShapeMode
    {
        POINT = GL_POINT,
        LINE  = GL_LINE,
        FILL  = GL_FILL
    };

    struct DebugShape
    {
        const Mesh*     shape                  = nullptr;
        GPM::Transform  transform              = {};
        ColorRGBA       color                  = ColorRGBA{1.f, 0.f, 0.f, 0.5f};
        EDebugShapeMode mode                   = EDebugShapeMode::FILL;
        bool            enableBackFaceCullling = true;
    };

    using LocalResourceManager = ResourcesManager<Mesh, Shader, Texture, RenderBuffer, RenderTexture>;

    using RenderPipeline = std::function<void(const ResourceManagerType&, const LocalResourceManager&, RenderSystem&,
                                              std::vector<Renderer*>&, std::vector<SubModel*>&, std::vector<SubModel*>&,
                                              std::vector<Camera*>&, std::vector<Light*>&, std::vector<DebugShape>&)>;

private:
    static RenderSystem* m_pInstance;

protected:
    LocalResourceManager m_localResources;

    std::vector<Renderer*>  m_pRenderers;
    std::vector<SubModel*>  m_pOpaqueSubModels;
    std::vector<SubModel*>  m_pTransparenteSubModels;
    std::vector<Camera*>    m_pCameras;
    std::vector<Light*>     m_pLights;
    std::vector<DebugShape> m_debugShape;

    unsigned int m_currentShaderID                  = 0;
    unsigned int m_currentTextureID                 = 0;
    unsigned int m_currentMeshID                    = 0;
    Shader*      m_currentPShaderUse                = nullptr;
    bool         m_currentBackFaceCullingModeEnable = false;

protected:
    RenderSystem() noexcept;

    ~RenderSystem() noexcept = default;

public:
    RenderSystem(const RenderSystem& other) noexcept = delete;
    RenderSystem(RenderSystem&& other) noexcept      = delete;
    RenderSystem& operator=(RenderSystem const& other) noexcept = delete;
    RenderSystem& operator=(RenderSystem&& other) noexcept = delete;

public:
    void tryToBindShader(Shader& shader);
    void tryToBindTexture(unsigned int textureID);
    void tryToBindMesh(unsigned int meshID);
    void tryToSetAlphaEnabled(bool alphaEnabled);
    void tryToSetBackFaceCulling(bool useBackFaceCulling);

    void resetCurrentRenderPassKey();

    bool isOnFrustum(const Frustum& camFrustum, const SubModel* pSubModel);
    void drawModelPart(const SubModel& subModel);
    void sendModelDataToShader(Camera& camToUse, SubModel& subModel);
    void sendDataToInitShader(Camera& camToUse, Shader* pCurrentShaderUse);

    RenderPipeline defaultRenderPipeline() const noexcept;
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

public:
    // TODO: Remove this shit and create variadic templated system
    void addRenderer(Renderer* pRenderer) noexcept;

    void updateRendererPointer(Renderer* newPointerRenderer, Renderer* exPointerRenderer) noexcept;

    void removeRenderer(Renderer* pRenderer) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addSubModel(SubModel* pSubModel) noexcept;

    void updateSubModelPointer(SubModel* newPointerSubModel, SubModel* exPointerSubModel) noexcept;

    void removeSubModel(SubModel* pSubModel) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addCamera(Camera* pCamera) noexcept;

    void updateCameraPointer(Camera* newPointerCamera, Camera* exPointerCamera) noexcept;

    void removeCamera(Camera* pCamera) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addLight(Light* pLight) noexcept;

    void updateLightPointer(Light* newPointerLight, Light* exPointerLight) noexcept;

    void removeLight(Light* pLight) noexcept;

    /**
     * @brief This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     *
     * @param value
     * @return RenderSystem*
     */
    static RenderSystem* getInstance() noexcept
    {
        if (unlikely(m_pInstance == nullptr))
        {
            m_pInstance = new RenderSystem();
        }
        return m_pInstance;
    }
};

} /*namespace GPE*/
