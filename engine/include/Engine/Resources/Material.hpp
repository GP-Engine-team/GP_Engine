/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <memory>        //std::unique_ptr, std::shared_ptr
#include <string>        //std::string
#include <unordered_map> //std::unordered_map

#include <Engine/Resources/ShaderType.hpp>
#include <Engine/Resources/Texture.hpp>
#include <Engine/Resources/Type.hpp>
#include <Engine/Resources/Uniform.hpp>
#include <Engine/Serialization/PathTo.hpp>

#include <Engine/Core/Tools/ClassUtility.hpp>

namespace GPE
{
class Shader;

class Material
{
public:
    struct ImporteArg
    {
        MaterialComponent comp{{1.f, 1.f, 1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, 1.f, 1.f};
        PathTo<Texture>   diffuseTexturePath{""};
        PathTo<Texture>   normalMapTexturePath{""};
        PathTo<Shader>    shaderPath{"Default"};
        std::unordered_map<std::string, std::unique_ptr<IUniform>> uniforms{};
    };

    struct CreateArg
    {
        MaterialComponent comp{{1.f, 1.f, 1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, 1.f, 1.f};
        Texture*          pDiffuseTexture{nullptr};
        Texture*          pNormalMapTexture{nullptr};
        Shader*           pShader{nullptr};
    };

protected:
    static unsigned int materialCount;

protected:
    MaterialComponent m_comp{{1.f, 1.f, 1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, 1.f, 1.f};
    Texture*          m_pDiffuseTexture{nullptr};
    Texture*          m_pNormalMapTexture{nullptr};
    Shader*           m_pShader{nullptr};
    unsigned int      m_ID = 0;

    /**
     * @brief Supported uniform
     */
    std::unordered_map<std::string, std::unique_ptr<IUniform>> uniforms;

protected:
    /**
     * @brief Clear previous uniforms and init it with default value thank's to the shader's uniform
     */
    void resetUniform();

    /**
     * @brief Remove and add uniform thank to it's shader. Keep the common uniforms
     */
    void updateUniform();

public:
    inline Material(const CreateArg& arg);

    Material()
    {
        updateUniform();
    }

    Material(const Material& other) = default;
    Material(Material&& other)      = default;
    ~Material()                     = default;
    Material& operator=(Material const& other) = default;
    Material& operator=(Material&& other) = default;

    GETTER_SETTER_BY_REF(Component, m_comp)
    GETTER_SETTER_BY_CONST_REF(DiffuseTexture, m_pDiffuseTexture)
    GETTER_SETTER_BY_CONST_REF(NormalMapTexture, m_pNormalMapTexture)
    GETTER_BY_VALUE(Shader, m_pShader)
    GETTER_BY_REF(Uniforms, uniforms)

    void setShader(Shader& newShader);

    inline bool isOpaque() const noexcept;

    inline unsigned int getID() const;

    inline void generateNewMaterial();
};
} /*namespace GPE*/

#include "Material.inl"