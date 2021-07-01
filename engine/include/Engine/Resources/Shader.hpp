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

#include <Engine/Core/Tools/BinaryMask.hpp>
#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/Resources/ShaderType.hpp>
#include <Engine/Serialization/xml/xmlLoader.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>
#include <GPM/Matrix3.hpp> //Mat3
#include <GPM/Matrix4.hpp> //Mat4
#include <GPM/Vector3.hpp> //Vec3
#include <GPM/Vector4.hpp> //Vec4

#include <Engine/Serialization/Serialize.hpp>

#include "Generated/Shader.rfk.h"

namespace GPE RFKNamespace()
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

#define ATTRIB_POSITION_NAME "aPos"
#define ATTRIB_POSITION_TYPE GL_FLOAT_VEC3
#define ATTRIB_POSITION_MASK (1 << 0)

#define ATTRIB_COLOR_NAME "aRGBA"
#define ATTRIB_COLOR_TYPE GL_FLOAT_VEC4
#define ATTRIB_COLOR_MASK (1 << 1)

#define ATTRIB_UV_NAME "aUV"
#define ATTRIB_UV_TYPE GL_FLOAT_VEC2
#define ATTRIB_UV_MASK (1 << 2)

#define ATTRIB_NORMAL_NAME "aNormal"
#define ATTRIB_NORMAL_TYPE GL_FLOAT_VEC3
#define ATTRIB_NORMAL_MASK (1 << 3)

#define ATTRIB_TANGEANTE_NAME "aTangeante"
#define ATTRIB_TANGEANTE_TYPE GL_FLOAT_VEC3
#define ATTRIB_TANGEANTE_MASK (1 << 4)

#define ATTRIB_BITANGEANTE_NAME "aBitangeante"
#define ATTRIB_BITANGEANTE_TYPE GL_FLOAT_VEC3
#define ATTRIB_BITANGEANTE_MASK (1 << 5)

#define ATTRIB_BONE_NAME "aBoneID"
#define ATTRIB_BONE_TYPE GL_INT_VEC4
#define ATTRIB_BONE_MASK (1 << 6)

#define ATTRIB_SKIN_WEIGHT_NAME "aSkinWeight"
#define ATTRIB_SKIN_TYPE GL_FLOAT_VEC4
#define ATTRIB_SKIN_WEIGHT_MASK (1 << 7)

    struct RFKStruct(Serialize()) Attribute
    {
        RFKField(Serialize()) std::string name;
        RFKField(Serialize()) GLenum      type;

        Attribute()
        {
        }

        Attribute(std::string && newName, GLenum && newType) : name{newName}, type{newType}
        {
        }

        Attribute_GENERATED
    };

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

        struct AttributeLocalized
        {
            GLint  location;
            GLenum type;
        };

        struct Uniform
        {
            GLenum type;
            GLint  location;
            GLint  size;
        };

    protected:
        U16BMask     m_attributeMask       = 0; // Attribute of the shader (aPos, aUV...)
        uint16_t     m_featureMask         = 0; // feature is shader interger into shader like light, blur etc....
        unsigned int m_lightsUniformBuffer = 0; // TODO: no sens to have id of uniform light in any shaders

        unsigned int m_id = 0;

        std::map<std::string, AttributeLocalized> m_attributes;
        std::map<std::string, Uniform>            m_uniforms;

    public:
        // List of supported attribut in shader. This list can be upgrad by the user
        static std::vector<Attribute> supportedAttribute;

        // return true if the attribute is in supported liste
        static bool isAttributeSupported(std::string name, GLenum type);

        // return the mask of the supported attribute. Return void mask if attribute is not found
        static U16BMask getSupportedAttributeMask(std::string name, GLenum type);

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
         * @brief activate the shader. Do not check if the shader was previoulsy activated.
         *
         */
        void use();

        /**
         * @brief Few function to set value into shader with uniform functions.
         * Importante /!\ : To use this functions, shader must be active
         *
         * @param name
         * @param value
         */
        inline void sendData(const char* name, bool data) const noexcept;
        inline void sendData(const char* name, int data) const noexcept;
        inline void sendData(const char* name, float data) const noexcept;
        inline void sendData(const char* name, const GPM::Vec2& data) const noexcept;
        inline void sendData(const char* name, const GPM::Vec3& data) const noexcept;
        inline void sendData(const char* name, const GPM::Vec4& data) const noexcept;
        inline void sendData(const char* name, const GPM::Mat3& data) const noexcept;
        inline void sendData(const char* name, const GPM::Mat4& data) const noexcept;
        inline void sendData(const char* name, const GPM::Vec3* data, size_t count) const noexcept;
        inline void sendData(const char* name, const GPM::Vec4* data, size_t count) const noexcept;
        inline void sendData(const char* name, const GPM::Mat3* data, size_t count) const noexcept;
        inline void sendData(const char* name, const GPM::Mat4* data, size_t count) const noexcept;

        void setLightBlock(const std::vector<LightData>& lightBuffer, const GPM::Vec3& viewPos) noexcept;
        void setMaterialBlock(const MaterialComponent& material) const noexcept;

        inline unsigned int getID() const noexcept;
        inline uint16_t     getFeature() const noexcept;

        GETTER_BY_REF(Attribute, m_attributes)
        GETTER_BY_REF(Uniforms, m_uniforms)

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

} // namespace )

#include "Shader.inl"
