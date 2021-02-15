/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <list>
#include <vector>

#include "Engine/Intermediate/Component.hpp"
#include "Engine/Resources/Material.hpp"

namespace Engine::Resources
{

class Mesh;
class Shader;

struct RenderPassKey
{
    RenderPassKey()                           = default;
    RenderPassKey(const RenderPassKey& other) = default;
    RenderPassKey(RenderPassKey&& other)      = default;
    ~RenderPassKey()                          = default;
    RenderPassKey& operator=(RenderPassKey const& other) = default;
    RenderPassKey& operator=(RenderPassKey&& other) = default;

    unsigned int textureId = 0; // less important bit
    unsigned int meshId    = 0;
    unsigned int shaderId  = 0; // Most important bit

    bool operator<(const RenderPassKey& other) const
    {
        if (shaderId < other.shaderId)
        {
            return true;
        }
        else if (meshId < other.meshId)
        {
            return true;
        }

        return textureId < other.textureId;
    }
};

struct ModelPart
{
    ModelPart()                       = default;
    ModelPart(const ModelPart& other) = default;
    ModelPart(ModelPart&& other)      = default;
    ~ModelPart()                      = default;
    ModelPart& operator=(ModelPart const& other) = default;
    ModelPart& operator=(ModelPart&& other) = default;

    const RenderPassKey key;

    class Model*                 pModel         = nullptr;
    Engine::Resources::Material* pMaterialToUse = nullptr;

    const bool         useBackFaceCulling = false;
    const unsigned int indexStart         = 0;
    const unsigned int indexCount         = 0;

    bool operator<(const ModelPart& other) const
    {
        return key < other.key;
    }
};

class Model : public Engine::Intermediate::Component
{
public:
    struct CreateArg
    {
        Shader*                pShader    = nullptr;
        std::vector<Material>* pMaterials = nullptr;
        Mesh*                  pMesh      = nullptr;

        bool loadInGPU             = true;
        bool enableBackFaceCulling = true;
        bool isOpaque              = true;
    };

protected:
    Shader*                m_pShader        = nullptr;
    std::vector<Material>* m_pMaterial      = nullptr; // contain the texture and material data
    std::vector<Material*> m_pMaterialToUse = {};      // contain pointor to the material to use when model is display.
    Mesh*                  m_pMesh          = nullptr;

    bool m_enableBackFaceCulling = true;
    bool m_isOpaque              = true;
    bool m_isLoadInGPU           = false;

private:
    /**
     * @brief fill pTextureToUse in function of
     *
     */
    void initTextureBufferWithMTLId();

public:
    Model(Engine::Intermediate::GameObject& owner, const CreateArg& arg);

    Model(const Model& other) noexcept;
    Model(Model&& other) noexcept;
    virtual ~Model();

    /*
    Model(Engine::Intermediate::GameObject& refGameObject, std::vector<std::string>& params,
          ResourcesManager& ressourcesManager); // load construtor
    */

    Model()        = delete;
    Model& operator=(Model const& other) = delete;
    Model& operator=(Model&& other) = delete;

    /**
     * @brief Load texture and Mesh from CPU to GPU. This operation can be slow.
     *
     */
    void loadInGPU() noexcept;
    void unloadFromGPU() noexcept;

    bool isOpaque() const noexcept
    {
        return m_isOpaque;
    }

    Shader* getpShader() noexcept
    {
        return m_pShader;
    }

    Mesh* getpMesh() noexcept
    {
        return m_pMesh;
    }

    void insertModelPartsOnContenor(std::list<ModelPart>& modelPartContenor) noexcept;

    /**
     * @brief return true if Texture is load in GPU and ready to use
     *
     * @return true
     * @return false
     */
    bool isLoadInGPU() const noexcept
    {
        return m_isLoadInGPU;
    }
};
} /*namespace Engine::Resources*/