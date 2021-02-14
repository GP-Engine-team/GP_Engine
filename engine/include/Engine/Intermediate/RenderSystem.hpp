/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <shared_mutex> //std::shared_mutex
#include <vector>       //std::vector

//TODO : Why this class don't work in forward declaration ????
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Camera.hpp"
#include "Engine/Resources/Model.hpp"

namespace Engine::Intermediate
{

/**
 * @brief The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over. Thread safe singleton according with link below :
 * @see https://refactoring.guru/fr/design-patterns/singleton/cpp/example
 */
class RendererSystem
{
    /**
     * The Singleton's constructor/destructor should always be private to
     * prevent direct construction/desctruction calls with the `new`/`delete`
     * operator.
     */
private:
    static RendererSystem*   m_pInstance;
    static std::shared_mutex m_mutex;

protected:
    std::vector<Engine::Resources::Model*> m_pModels;
    std::vector<Engine::Resources::Camera*> m_pCameras;

    unsigned int m_currentShaderId                  = 0;
    unsigned int m_currentTextureId                 = 0;
    unsigned int m_currentMeshId                    = 0;
    Engine::Resources::Shader* m_currentPShaderUse                = nullptr;
    bool         m_currentBackFaceCullingModeEnable = false;

protected:
    void tryToBindShader(Engine::Resources::Shader* pShader);
    void tryToBindTexture(unsigned int textureId);
    void tryToBindMesh(unsigned int meshId);
    void tryToSetAlphaEnabled(bool alphaEnabled);
    void tryToSetBackFaceCulling(bool useBackFaceCulling);

    void resetCurrentRenderPassKey();

    inline RendererSystem() noexcept = default;
    ~RendererSystem() noexcept = default;

public:
    RendererSystem(const RendererSystem& other) noexcept = delete;
    RendererSystem(RendererSystem&& other) noexcept      = delete;
    RendererSystem& operator=(RendererSystem const& other) noexcept = delete;
    RendererSystem& operator=(RendererSystem&& other) noexcept = delete;

    void draw() noexcept;

    /**
     * @brief Add Model (using key word this) on the physic system. This object will be updated by the physic system
     *
     * @param pModel
     */
    void addModel(Engine::Resources::Model* pModel) noexcept;

    void updateModelPointer(Engine::Resources::Model* newPointerModel,
                            Engine::Resources::Model* exPointerModel) noexcept;

    void removeModel(Engine::Resources::Model* pModel) noexcept;



    void addCamera(Engine::Resources::Camera* pCamera) noexcept;

    void updateCameraPointer(Engine::Resources::Camera* newPointerCamera,
                             Engine::Resources::Camera* exPointerCamera) noexcept;

    void removeCamera(Engine::Resources::Camera* pCamera) noexcept;

    /**
     * @brief This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     *
     * @param value
     * @return RendererSystem*
     */
    static RendererSystem* getInstance() noexcept
    {
        // double same if to avoid to lock mutex
        if (m_pInstance == nullptr)
            [[unlikely]]
            {
                std::unique_lock lock(m_mutex);
                if (m_pInstance == nullptr)
                    [[unlikely]]
                    {
                        m_pInstance = new RendererSystem();
                    }
                return m_pInstance;
            }

        std::shared_lock lock(m_mutex);
        return m_pInstance;
    }
};

} /*namespace Engine::Intermediate*/