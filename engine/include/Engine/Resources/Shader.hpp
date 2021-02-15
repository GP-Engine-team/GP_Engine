/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once


#include <glad/glad.h>
#include <string>
#include <vector>

#include "GPM/Vector3.hpp" //Vec3
#include "Engine/Resources/ShaderType.hpp"

namespace Engine::Resources
{

enum class EShaderType
{
    FRAGMENT,
    VERTEX,
    PROGRAM
};

// feature mask
#define LIGHT_BLIN_PHONG 1    // 2^0, bit 1
#define SKYBOX 2              // 2^1, bit 2
#define AMBIANTE_COLOR_ONLY 4 // 2^2, bit 4
#define SCALE_TIME_ACC 8      // 2^3, bit 8
#define UNSCALED_TIME_ACC 16  // 2^4, bit 16

// Inspiread about code exemple on learn openGl : https://learnopengl.com/Getting-started/Shaders
class Shader
{
protected:
    const unsigned char m_featureMask; // feature is shader interger into shader like light, blure etc....
    unsigned int m_lightsUniformBuffer;

    std::string m_nameFragment;
    std::string m_nameVertex;

    unsigned int m_id;

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
    Shader(const Shader& other) = delete;
    Shader(Shader&& other) = default;
    ~Shader();

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
    void setBool(const char* name, bool value) const;
    void setInt(const char* name, int value) const;
    void setFloat(const char* name, float value) const;
    void setVec3(const char* name, float v1, float v2, float v3) const;
    void setVec4(const char* name, float v1, float v2, float v3, float v4) const;
    void setMat3(const char* name, const float* value) const;
    void setMat4(const char* name, const float* value) const;
    void setpVec3(const char* name, unsigned int count, const float* pV) const;
    void setpVec4(const char* name, unsigned int count, const float* pV) const;
    void setLightBlock(const std::vector<LightData>& lightBuffer, const GPM::Vec3& viewPos);
    void setMaterialBlock(const MaterialComponent& material);

    unsigned int getIdProgramm() const noexcept
    {
        return m_id;
    };

    /**
     * @brief Get the Feature object
     *
     * @return const std::vector<E_ShaderFeature>&
     */
    unsigned char getFeature()
    {
        return m_featureMask;
    }

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

    /**
     * @brief Use log function for checking shader compilation/linking errors
     *
     * @param shader
     * @param type
     */
    void checkCompileErrors(unsigned int shader, EShaderType type);

    /**
     * @brief parse the name of image integer in path
     *
     * @param path
     * @param shaderName
     */
    void parseName(const char* path, std::string& shaderName);
};

inline void Shader::setBool(const char* name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_id, name), (int)value);
}

inline void Shader::setInt(const char* name, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name), value);
}

inline void Shader::setFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name), value);
}

inline void Shader::setVec3(const char* name, float v1, float v2, float v3) const
{
    glUniform3f(glGetUniformLocation(m_id, name), v1, v2, v3);
}

inline void Shader::setVec4(const char* name, float v1, float v2, float v3, float v4) const
{
    glUniform4f(glGetUniformLocation(m_id, name), v1, v2, v3, v4);
}

inline void Shader::setMat3(const char* name, const float* value) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, value);
}

inline void Shader::setMat4(const char* name, const float* value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, value);
}

inline void Shader::setpVec3(const char* name, unsigned int count, const float* pV) const
{
    glUniform3fv(glGetUniformLocation(m_id, name), count, pV);
}

inline void Shader::setpVec4(const char* name, unsigned int count, const float* pV) const
{
    glUniform4fv(glGetUniformLocation(m_id, name), count, pV);
}
} /*namespace Engine::Resources*/