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

namespace GPE
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
    Material* pMaterialToUse = nullptr;

    const bool         useBackFaceCulling = false;
    const unsigned int indexStart         = 0;
    const unsigned int indexCount         = 0;

    bool operator<(const ModelPart& other) const
    {
        return key < other.key;
    }
};

class Model : public Component
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
    void initTextureBufferWithMTLId() noexcept;

public:
    Model(GameObject& owner, const CreateArg& arg);

    Model(const Model& other) noexcept;
    Model(Model&& other) noexcept;
    virtual ~Model();

    /*
    Model(GameObject& refGameObject, std::vector<std::string>& params,
          ResourcesManager& ressourcesManager); // load construtor
    */

    Model()        = delete;
    Model& operator=(Model const& other) = delete;
    Model& operator=(Model&& other) = delete;

    /**
     * @brief Load texture and Mesh from CPU to GPU. This operation can be slow.
     *
     */
    inline void loadInGPU() noexcept;

    inline void unloadFromGPU() noexcept;

    inline bool isOpaque() const noexcept;

    inline Shader* getpShader() noexcept;

    inline Mesh* getpMesh() noexcept;

    /**
     * @brief return true if Texture is load in GPU and ready to use
     *
     * @return true
     * @return false
     */
    inline bool isLoadInGPU() const noexcept;

    void insertModelPartsOnContenor(std::list<ModelPart>& modelPartContenor) noexcept;
};

#include "Model.inl"

} /*namespace GPE*/