/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <memory> //std::unique_ptr, std::shared_ptr
#include <string> //std::string

#include <Engine/Resources/ShaderType.hpp>
#include <Engine/Resources/Texture.hpp>
#include <Engine/Resources/Type.hpp>

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
        std::string       diffuseTexturePath   = "";
        std::string       normalMapTexturePath = "";
        std::string       shaderPath           = "";
        std::unordered_map<std::string, float> floatUniform;
        std::unordered_map<std::string, int>   intUniform;
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
    std::unordered_map<std::string, float> floatUniforms;
    std::unordered_map<std::string, int>   intUniforms;
    // std::unordered_map<std::string, Vec2>  vec2Uniforms;
    // std::unordered_map<std::string, Vec3>  vec3Uniforms;

protected:
    /**
     * @brief Update the uniform buffers with default value thank's to the shader's uniform
     */
    void updateUniform();

public:
    inline Material(const CreateArg& arg);

    Material()                      = default;
    Material(const Material& other) = default;
    Material(Material&& other)      = default;
    ~Material()                     = default;
    Material& operator=(Material const& other) = default;
    Material& operator=(Material&& other) = default;

    GETTER_SETTER_BY_REF(Component, m_comp);
    GETTER_SETTER_BY_REF(DiffuseTexture, m_pDiffuseTexture);
    GETTER_SETTER_BY_REF(NormalMapTexture, m_pNormalMapTexture);
    GETTER_SETTER_BY_VALUE(Shader, m_pShader);

    inline bool isOpaque() const noexcept;

    inline unsigned int getID() const;

    inline void generateNewMaterial();
};
} /*namespace GPE*/

#include "Material.inl"