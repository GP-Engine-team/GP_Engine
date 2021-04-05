﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

#include "Engine/Resources/ShaderType.hpp"
#include "GPM/Vector3.hpp" //Vec3

namespace GPE
{
// feature mask
#define LIGHT_BLIN_PHONG (1 << 0)
#define SKYBOX (1 << 1)
#define AMBIANTE_COLOR_ONLY (1 << 2)
#define SCALE_TIME_ACC (1 << 3)
#define UNSCALED_TIME_ACC (1 << 4)

// Inspiread about code exemple on learn openGl : https://learnopengl.com/Getting-started/Shaders
class Shader
{
public:
    enum class EType
    {
        FRAGMENT,
        VERTEX,
        PROGRAM
    };

protected:
    unsigned char m_featureMask         = 0; // feature is shader interger into shader like light, blure etc....
    unsigned int  m_lightsUniformBuffer = 0; // TODO: no sens to have id of uniform light in any shaders

    unsigned int m_id         = 0;
    unsigned int m_idVertex   = 0; // TODO: move in fragmentShader class to optimize recycling
    unsigned int m_idFragment = 0; // TODO: move in vertexShader class to optimize recycling

public:
    /**
     * @brief Construct a new Shader object and generates the shader. If feature is added, the feature is added
     * definitely to the shader Use getFeature() to cheack the feature use
     *
     * @param vertexPath
     * @param fragmentPath
     * @param featureMask           : LIGHT_BLIN_PHONG | FEATURE_2 | [...]
     */
    Shader(const char* vertexPath, const char* fragmentPath, unsigned char featureMask = 0);

    // TODO:
    Shader(const char* shaderPath);

    Shader(const Shader& other) = delete;
    void operator=(const Shader&) = delete;
    Shader(Shader&& other)        = delete;
    void operator=(Shader&&) = delete;
    ~Shader() noexcept;

    void reload(const char* vertexPath, const char* fragmentPath, unsigned char featureMask = 0);

    /**
     * @brief activate the shader
     *
     */
    void use();

    /**
     * @brief Few function to set value into shader with uniform functions
     *
     * @param name
     * @param value
     */
    inline void setBool(const char* name, bool value) const noexcept;
    inline void setInt(const char* name, int value) const noexcept;
    inline void setFloat(const char* name, float value) const noexcept;
    inline void setVec3(const char* name, float v1, float v2, float v3) const noexcept;
    inline void setVec4(const char* name, float v1, float v2, float v3, float v4) const noexcept;
    inline void setMat3(const char* name, const float* value) const noexcept;
    inline void setMat4(const char* name, const float* value) const noexcept;
    inline void setpVec3(const char* name, unsigned int count, const float* pV) const noexcept;
    inline void setpVec4(const char* name, unsigned int count, const float* pV) const noexcept;
    void        setLightBlock(const std::vector<LightData>& lightBuffer, const GPM::Vec3& viewPos) noexcept;
    void        setMaterialBlock(const MaterialComponent& material) const noexcept;

    inline unsigned int  getID() const noexcept;
    inline unsigned char getFeature() const noexcept;

private:
    /**
     * @brief Load shaders files and set code into vertexCode and fragmentCode
     *
     * @param vertexPath
     * @param vertexCode
     * @param fragmentPath
     * @param fragmentCode
     * @return bool true is error happend
     */
    bool loadFile(const char* vertexPath, std::string& vertexCode, const char* fragmentPath, std::string& fragmentCode);

    /**
     * @brief Compile
     *
     * @param vertexCode
     * @param fragmentCode
     */
    void compile(std::string& vertexCode, std::string& fragmentCode);
    void compileVertex(std::string& vertexCode);
    void compileFragment(std::string& fragmentCode);

    /**
     * @brief Use log function for checking shader compilation/linking errors
     *
     * @param shader
     * @param type
     */
    void checkCompileErrors(unsigned int shader, EType type);

    void loadAndCompile(const char* vertexPath, const char* fragmentPath, unsigned char featureMask = 0);
    void release();
};

#include "Shader.inl"

} /*namespace GPE*/
