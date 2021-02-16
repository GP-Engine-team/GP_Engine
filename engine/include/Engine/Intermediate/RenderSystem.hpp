/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <shared_mutex> //std::shared_mutex
#include <vector>       //std::vector

#include "Engine/Core/Tools/BranchPrediction.hpp"

namespace GPE
{
    class Light;
    class Camera;
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
    /**
     * The Singleton's constructor/destructor should always be private to
     * prevent direct construction/desctruction calls with the `new`/`delete`
     * operator.
     */
private:
    static RenderSystem*     m_pInstance;
    static std::shared_mutex m_mutex;

protected:
    std::vector<Model*>  m_pModels;
    std::vector<Camera*> m_pCameras;
    std::vector<Light*>  m_pLights;

    unsigned int               m_currentShaderId                  = 0;
    unsigned int               m_currentTextureId                 = 0;
    unsigned int               m_currentMeshId                    = 0;
    Shader* m_currentPShaderUse                = nullptr;
    bool                       m_currentBackFaceCullingModeEnable = false;

protected:
    void tryToBindShader(Shader* pShader);
    void tryToBindTexture(unsigned int textureId);
    void tryToBindMesh(unsigned int meshId);
    void tryToSetAlphaEnabled(bool alphaEnabled);
    void tryToSetBackFaceCulling(bool useBackFaceCulling);

    void resetCurrentRenderPassKey();

    inline RenderSystem() noexcept = default;
    ~RenderSystem() noexcept       = default;

public:
    RenderSystem(const RenderSystem& other) noexcept = delete;
    RenderSystem(RenderSystem&& other) noexcept      = delete;
    RenderSystem& operator=(RenderSystem const& other) noexcept = delete;
    RenderSystem& operator=(RenderSystem&& other) noexcept = delete;

    void draw() noexcept;

    /**
     * @brief Add Model (using key word this) on the physic system. This object will be updated by the physic system
     *
     * @param pModel
     */
    // TODO: Remove this shit and create variadic templated system
    void addModel(Model* pModel) noexcept;

    void updateModelPointer(Model* newPointerModel,
                            Model* exPointerModel) noexcept;

    void removeModel(Model* pModel) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addCamera(Camera* pCamera) noexcept;

    void updateCameraPointer(Camera* newPointerCamera,
                             Camera* exPointerCamera) noexcept;

    void removeCamera(Camera* pCamera) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addLight(Light* pLight) noexcept;

    void updateLightPointer(Light* newPointerLight,
                            Light* exPointerLight) noexcept;

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
        // double same if to avoid to lock mutex
        if (unlikely(m_pInstance == nullptr))
        {
            std::unique_lock lock(m_mutex);
            if (unlikely(m_pInstance == nullptr))
            {
                m_pInstance = new RenderSystem();
            }
            return m_pInstance;
        }

        std::shared_lock lock(m_mutex);
        return m_pInstance;
    }
};

} /*namespace GPE*/