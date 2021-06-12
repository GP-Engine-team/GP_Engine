/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <string>
#include <vector>

#include <Engine/Resources/ShaderType.hpp>
#include <Engine/Serialization/xml/xmlLoader.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>
#include <GPM/Vector3.hpp> //Vec3

namespace GPE
{
// feature mask
#define LIGHT_BLIN_PHONG (1 << 0)
#define SKYBOX (1 << 1)
#define AMBIANTE_COLOR_ONLY (1 << 2)
#define SCALE_TIME_ACC (1 << 3)
#define UNSCALED_TIME_ACC (1 << 4)
#define FOG (1 << 5)

#define PROJECTION_VIEW_MODEL_MATRIX (1 << 6)
#define PROJECTION_MATRIX (1 << 7)
#define VIEW_MATRIX (1 << 8)
#define VIEW_MODEL_MATRIX (1 << 9)
#define ANIMATION_MASK (1 << 10)

// class Shader;
//
// template <>
// void load(XmlLoader& context, Shader*& data, const rfk::Field& info);
//
// template <>
// void load(XmlLoader& context, Shader*& data, const XmlLoader::LoadInfo& info);
//
// template <>
// void save(XmlSaver& context, Shader* const& data, const rfk::Field& info);
//
// template <>
// void save(XmlSaver& context, Shader* const& data, const XmlSaver::SaveInfo& info);

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

    struct Attribute
    {
        GLenum type;
    };

    struct Uniform
    {
        GLenum type;
        GLint  location;
        GLint  size;
    };

protected:
    uint16_t     m_featureMask         = 0; // feature is shader interger into shader like light, blur etc....
    unsigned int m_lightsUniformBuffer = 0; // TODO: no sens to have id of uniform light in any shaders

    unsigned int m_id = 0;

    std::map<std::string, Attribute> m_attributes;
    std::map<std::string, Uniform>   m_uniforms;

public:
    /**
     * @brief Construct a new Shader object and generates the shader. If feature is added, the feature is added
     * definitely to the shader Use getFeature() to cheack the feature use
     *
     * @param vertexPath
     * @param fragmentPath
     * @param featureMask           : LIGHT_BLIN_PHONG | FEATURE_2 | [...]
     */
    Shader(const char* vertexPath, const char* fragmentPath, uint16_t featureMask = 0);

    Shader(const Shader& other) = delete;
    void operator=(const Shader&) = delete;
    Shader(Shader&& other)        = delete;
    void operator=(Shader&&) = delete;
    ~Shader() noexcept;

    void reload(const char* vertexPath, const char* fragmentPath, uint16_t featureMask = 0);

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

    inline unsigned int getID() const noexcept;
    inline uint16_t     getFeature() const noexcept;

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
    static bool loadFile(const char* vertexPath, std::string& vertexCode, const char* fragmentPath,
                         std::string& fragmentCode);

    /**
     * @brief Compile
     *
     * @param vertexCode
     * @param fragmentCode
     */
    static unsigned int compile(std::string& vertexCode, std::string& fragmentCode);
    static void         compileVertex(std::string& vertexCode);
    static void         compileFragment(std::string& fragmentCode);

    /**
     * @brief Get all uniform and store it in m_uniforms
     */
    void updateUniformList();

    /**
     * @brief Use log function for checking shader compilation/linking errors
     *
     * @param shader
     * @param type
     */
    static bool checkCompileErrors(unsigned int shader, EType type);

    static unsigned int loadAndCompile(const char* vertexPath, const char* fragmentPath, uint16_t featureMask = 0);
    void                release();
};

#include "Shader.inl"

} /*namespace GPE*/
