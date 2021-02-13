/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/IModel.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Texture.hpp"

#include <list>
#include <vector>

namespace Engine::Resources
{
class Model;

struct RenderPassKey
{
    RenderPassKey() = default;
    RenderPassKey(const RenderPassKey& other) = default;
    RenderPassKey(RenderPassKey&& other) = default;
    ~RenderPassKey() = default;
    RenderPassKey& operator=(RenderPassKey const& other) = default;
    RenderPassKey& operator=(RenderPassKey&& other) = default;

    unsigned int textureId{0}; // less important bit
    unsigned int meshId{0};
    unsigned int shaderId{0}; // Most important bit

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
    ModelPart() = default;
    ModelPart(const ModelPart& other) = default;
    ModelPart(ModelPart&& other) = default;
    ~ModelPart() = default;
    ModelPart& operator=(ModelPart const& other) = default;
    ModelPart& operator=(ModelPart&& other) = default;

    const RenderPassKey key;

    Model* pModel{nullptr};
    Engine::Resources::Material* pMaterialToUse{nullptr};

    const bool useBackFaceCulling{false};
    const unsigned int indexStart{0};
    const unsigned int indexCount{0};

    bool operator<(const ModelPart& other) const
    {
        return key < other.key;
    }
};

class Model : public IModel
{
public:
    struct CreateArg
    {
        std::string shaderName;
        std::string materialName;
        std::string meshName;

        bool loadInGPU{true};
        bool enableBackFaceCulling{true};
        bool isOpaque{true};
    };

protected:
    Engine::Resources::Shader* m_pShader;
    std::vector<Engine::Resources::Material>* m_pMaterial; // contain the texture and material data
    std::vector<Engine::Resources::Material*>
        m_pMaterialToUse; // contain pointor to the material to use when model is display.
    Engine::Resources::Mesh* m_pMesh;

    std::string m_shaderName;
    std::string m_materialName;
    std::string m_meshName;

    bool m_enableBackFaceCulling;
    bool m_isOpaque;

protected:
private:
    /**
     * @brief fill pTextureToUse in function of
     *
     */
    void initTextureBufferWithMTLId();

public:
    Model(Engine::Resources::GameObject& refGameObject, const CreateArg& arg);

    Model(const Model& other);
    Model(Model&& other);
    virtual ~Model();

    Model(Engine::Intermediate::GameObject& refGameObject, std::vector<std::string>& params, Engine
          : &ressourcesManager); // load construtor

    Model() = delete;
    Model& operator=(Model const& other) = delete;
    Model& operator=(Model&& other) = delete;

    /**
     * @brief Draw element only if it is load in GPU
     *
     */
    void draw() const noexcept override;

    /**
     * @brief Load texture and Mesh from CPU to GPU. This operation can be slow.
     *
     */
    void loadInGPU() noexcept;
    void unloadFromGPU() noexcept;

    bool isVisibleTowardCamera() const noexcept;

    // nullptr if don't use the shader else set pointor to shader to use
    void useSimpleShader(Engine::Resources : Shader* pShader);

    bool isOpaque() const noexcept
    {
        return m_isOpaque;
    }

    const std::string& getShaderName() const noexcept
    {
        return m_shaderName;
    }
    const std::string& getMaterialName() const noexcept
    {
        return m_materialName;
    }
    const std::string& getMeshName() const noexcept
    {
        return m_meshName;
    }

    Engine::Resources : Shader* getpShader() noexcept
    {
        return m_pShader;
    }
    Engine::Resources : Mesh* getpMesh() noexcept
    {
        return m_pMesh;
    }

    void insertModelPartsOnContenor(std::list<ModelPart>& modelPartContenor) noexcept;

    std::shared_ptr<Engine::Core::Maths::Shape3D::Volume> getpBoudingVolume() const noexcept;

    virtual void sendToShaderModelMatrix() const noexcept;
};
} /*namespace Engine::Resources*/