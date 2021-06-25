#include "Engine/Resources/Shader.hpp"

#include <fstream>
#include <sstream>

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Parsers/ShaderParser.hpp"
#include "Engine/Resources/Color.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Resources/Importer/Importer.hpp>

using namespace GPM;
using namespace GPE;
using namespace std;

// TODO:OpenGL must coincide with the openGl context
static const char* versionHeaderStr = R"(#version 460 core

)";

static const char* lightBlinPhongVertexShaderStr = R"(out vec3 fragPos;

out vec3 normal;

uniform mat4 model;
uniform mat4 projectViewModelMatrix;
uniform mat3 inverseModelMatrix;

void computFragPos(vec3 aPos)
{
    fragPos = vec3(model * vec4(aPos, 1.0));
}

void computNormal(vec3 aNormal)
{
    normal = normalize(inverseModelMatrix * aNormal);
}
)";

static const char* lightBlinPhongFragmentShaderStr = R"(#define MAX_LIGHT_NUMBER 15

#define LIGHT               0.f
#define POINT_LIGHT         1.f
#define SPOT_LIGHT          2.f
#define DIRECITONNAL_LIGHT  3.f

in vec3 fragPos;
in vec3 normal;
uniform vec3 viewPos;

uniform int numberLightUse;
struct Light {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    vec3 pos;
    float type;

    //Point arg
    float constant;
    float linear;
    float quadratic;

    //Spot arg
    float cutOffExponent;
    vec3 direction;
    float cutOff;
};

struct Material {
    vec4    ambient;
    vec4    diffuse;
    vec4    specular;
    float   shininess;
    float   opacity;
};

uniform Material material;

layout(std140) uniform lightBlock
{
    Light lightBuffer[MAX_LIGHT_NUMBER];
};

vec3 computAmbiante(vec4 ambientComponent, vec4 ambientMaterial)
{
    return ambientComponent.w * ambientComponent.xyz * ambientMaterial.xyz * ambientMaterial.w;
}

vec3 computDiffuse(vec4 diffuseComponent, vec4 diffuseMaterial, vec3 lightDir)
{
    float diff = max(dot(lightDir, normal), 0.0);
    return diffuseComponent.w * diff * diffuseComponent.xyz * diffuseMaterial.xyz * diffuseMaterial.w;
}

vec3 computSpecular(vec4 specularComponent, vec4 specularMaterial, vec3 lightDir, vec3 viewPos, float shininess)
{
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    return specularComponent.w * spec * specularComponent.xyz * specularMaterial.xyz * specularMaterial.w;
}

float computLightAttenuationWithDistance(vec3 lightPos, float lightConstant, float lightLinear, float lightQuadratic)
{
    float distance    = length(lightPos - fragPos);
    return 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));
}

float computSpotLightIntensity(vec3 lightDir, vec3 lightDirection, float cutOff, float cutOffExp)
{
    float theta = dot(lightDir, normalize(-lightDirection)); 
    float epsilon = (cutOff - cutOffExp);
    return clamp((theta - cutOffExp) / epsilon, 0.0, 1.0);
}

//FragColor = texture(ourTexture, TexCoord) * vec4(computLight([...]), 1.0);
vec3 computLight()
{
    vec3 result;
    result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < numberLightUse; i++)
    {
        vec3 lightDir;

        if(lightBuffer[i].type == DIRECITONNAL_LIGHT)
        {
            lightDir = lightBuffer[i].pos;
        }
        else
        {
            lightDir = normalize(lightBuffer[i].pos - fragPos);
        }

        vec3 amb = computAmbiante(lightBuffer[i].ambient, material.ambient);
        vec3 diff = computDiffuse(lightBuffer[i].diffuse, material.diffuse, lightDir);
        vec3 spec = material.shininess == 0.0 ? vec3(0.0, 0.0, 0.0) : computSpecular(lightBuffer[i].specular, material.specular, lightDir, viewPos, material.shininess);
            
        if(lightBuffer[i].type == DIRECITONNAL_LIGHT)
        {
            result += amb + diff + spec;
        }
        else if(lightBuffer[i].type == POINT_LIGHT)
        {
            float attenuation = computLightAttenuationWithDistance(lightBuffer[i].pos, lightBuffer[i].constant, lightBuffer[i].linear, lightBuffer[i].quadratic);
            result += (amb + diff + spec) * attenuation;
        }
        else if(lightBuffer[i].type == SPOT_LIGHT)
        {
            float intensity = computSpotLightIntensity(lightDir, lightBuffer[i].direction, lightBuffer[i].cutOff, lightBuffer[i].cutOffExponent);
            float attenuation = computLightAttenuationWithDistance(lightBuffer[i].pos, lightBuffer[i].constant, lightBuffer[i].linear, lightBuffer[i].quadratic);
            result += (amb + (diff + spec) * intensity) * attenuation;
        }
    }

    return result;
}

)";

static const char* colorFragmentShaderStr = R"(
uniform vec4 Color;
)";

static const char* timeScaledAccVertexShaderStr = R"(
uniform float scaledTimeAcc;
)";

static const char* timeUnscaledAccVertexShaderStr = R"(
uniform float unscaledTimeAcc;
)";

std::vector<Shader::SupportedAttribute> Shader::supportedAttribute{
    {ATTRIB_POSITION_NAME, ATTRIB_POSITION_TYPE},
    {ATTRIB_COLOR_NAME, ATTRIB_COLOR_TYPE},
    {ATTRIB_UV_NAME, ATTRIB_UV_TYPE},
    {ATTRIB_NORMAL_NAME, ATTRIB_NORMAL_TYPE},
    {ATTRIB_TANGEANTE_NAME, ATTRIB_TANGEANTE_TYPE},
    {ATTRIB_BITANGEANTE_NAME, ATTRIB_BITANGEANTE_TYPE},
    {ATTRIB_BONE_NAME, ATTRIB_BONE_TYPE},
    {ATTRIB_SKIN_WEIGHT_NAME, ATTRIB_SKIN_TYPE},
};

Shader::Shader(const char* vertexPath, const char* fragmentPath, uint16_t featureMask)
{
    m_featureMask = featureMask;
    m_id          = loadAndCompile(vertexPath, fragmentPath, featureMask);

    glUseProgram(m_id);
    sendData("ourTexture", 0);
    sendData("shadowMap", 1);
    sendData("normalMap", 2);

    updateUniformList();
}

Shader::~Shader() noexcept
{
    release();
}

void Shader::reload(const char* vertexPath, const char* fragmentPath, uint16_t featureMask)
{
    unsigned int newID = loadAndCompile(vertexPath, fragmentPath, featureMask);
    if (newID)
    {
        glDeleteProgram(m_id);
        m_id          = newID;
        m_featureMask = featureMask;

        glUseProgram(m_id);
        sendData("ourTexture", 0);
        sendData("shadowMap", 1);
        sendData("normalMap", 2);

        updateUniformList();
    }
}

void Shader::use()
{
    glUseProgram(m_id);
}

void Shader::setLightBlock(const std::vector<LightData>& lightBuffer, const Vec3& viewPos) noexcept
{
    if ((m_featureMask & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        if (m_lightsUniformBuffer == 0)
        {
            glGenBuffers(1, &m_lightsUniformBuffer);
        }

        glBindBuffer(GL_UNIFORM_BUFFER, m_lightsUniformBuffer);
        glBufferData(GL_UNIFORM_BUFFER, lightBuffer.size() * sizeof(LightData), lightBuffer.data(), GL_DYNAMIC_DRAW);

        { // try to get uniform block index
            GLuint      blockIndex;
            const char* blockName = "lightBlock";
            blockIndex            = glGetUniformBlockIndex(m_id, blockName);

            if (blockIndex == GL_INVALID_INDEX)
            {
                FUNCT_ERROR((std::string("blockIndex invalid with name : ") + blockName).c_str());
                // exit(0);
            }

            glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, m_lightsUniformBuffer);
        }

        sendData("numberLightUse", static_cast<int>(lightBuffer.size()));
        sendData("viewPos", viewPos);
    }
    else
    {
        Log::getInstance()->logWarning("Shader cannot send light block because it is not a light shader");
    }
}

void Shader::setMaterialBlock(const MaterialComponent& material) const noexcept
{
    if ((m_featureMask & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        sendData("material.ambient", material.ambient.rgbi);
        sendData("material.diffuse", material.diffuse.rgbi);
        sendData("material.specular", material.specular.rgbi);
        sendData("material.shininess", material.shininess);
        sendData("material.opacity", material.opacity);
    }
    else
    {
        Log::getInstance()->logWarning("Shader cannot send Material block because it is not a light shader");
    }
}

bool Shader::loadFile(const char* vertexPath, std::string& vertexCode, const char* fragmentPath,
                      std::string& fragmentCode)
{
    // retrieve the vertex/fragment source code from filePath
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // open files and check error
    vShaderFile.open(vertexPath);
    if (!vShaderFile)
    {
        FUNCT_ERROR((std::string("Impossible to load shader : ") + vertexPath).c_str());
        vShaderFile.close();
        return true;
    }

    fShaderFile.open(fragmentPath);
    if (!vShaderFile)
    {
        FUNCT_ERROR((std::string("Impossible to load shader : ") + fragmentPath).c_str());
        vShaderFile.close();
        fShaderFile.close();
        return true;
    }

    // read file's buffer contents into streams
    std::stringstream vShaderStream;
    std::stringstream fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // convert stream into string
    vertexCode   = vShaderStream.str().c_str();
    fragmentCode = fShaderStream.str().c_str();

    return false;
}

unsigned int Shader::compile(std::string& vertexCode, std::string& fragmentCode)
{
    // compile shaders
    unsigned int vertex, fragment, program;

    // vertex shader
    const char* vShaderCode = vertexCode.c_str();
    vertex                  = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    if (!checkCompileErrors(vertex, EType::VERTEX))
    {
        glDeleteShader(vertex);
        return 0;
    }

    // fragment Shader
    const char* fShaderCode = fragmentCode.c_str();
    fragment                = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    if (!checkCompileErrors(fragment, EType::FRAGMENT))
    {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return 0;
    }

    // shader Program
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    if (!checkCompileErrors(program, EType::PROGRAM))
    {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return 0;
    }
    return program;
}

bool Shader::checkCompileErrors(unsigned int shader, EType type)
{
    int  success;
    char infoLog[1024];
    if (type != EType::PROGRAM)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);

            if (type == EType::VERTEX)
            {
                Log::getInstance()->logError(std::string("Vertex shader compilation error\n") + infoLog);
            }
            else if (type == EType::FRAGMENT)
            {
                Log::getInstance()->logError(std::string("Fragment shader compilation error\n") + infoLog);
            }
            return false;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);

            if (type == EType::VERTEX)
            {
                Log::getInstance()->logError((std::string("Vertex shader linking error\n") + infoLog).c_str());
            }
            else if (type == EType::FRAGMENT)
            {
                Log::getInstance()->logError((std::string("Fragment shader linking error\n") + infoLog).c_str());
            }
            return false;
        }
    }
    return true;
}

bool Shader::isAttributeSupported(std::string name, GLenum type)
{
    for (auto&& sAttrib : supportedAttribute)
    {
        if (sAttrib.type == type && sAttrib.name == name)
            return true;
    }
    return false;
}

U16BMask Shader::getSupportedAttributeMask(std::string name, GLenum type)
{
    for (size_t i = 0; i < supportedAttribute.size(); ++i)
    {
        if (supportedAttribute[i].type == type && supportedAttribute[i].name == name)
            return i;
    }
    return 0;
}

void Shader::updateUniformList()
{
    std::vector<GLchar> nameData(256, '\0');
    std::vector<GLenum> properties;
    std::vector<GLint>  values(5);

    // ATTRIBUTE
    {
        GLint numActiveAttribs = 0;
        glGetProgramInterfaceiv(m_id, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
        properties.push_back(GL_NAME_LENGTH);
        properties.push_back(GL_TYPE);
        properties.push_back(GL_LOCATION);
        values.resize(properties.size());
        for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
        {
            glGetProgramResourceiv(m_id, GL_PROGRAM_INPUT, attrib, properties.size(), &properties[0], values.size(),
                                   NULL, &values[0]);

            nameData.resize(values[0]); // The length of the name.

            glGetProgramResourceName(m_id, GL_PROGRAM_INPUT, attrib, nameData.size(), NULL, &nameData[0]);
            std::string name((char*)&nameData[0], nameData.size() - 1);
            m_attributes.emplace(name, Attribute{values[2], static_cast<GLenum>(values[1])});

            // Try to add it to the attribute mask
            m_attributeMask.add(getSupportedAttributeMask(name, values[1]).get());
        }
    }

    // UNIFORM
    {
        GLint numActiveUniforms = 0;
        glGetProgramInterfaceiv(m_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);
        properties.push_back(GL_ARRAY_SIZE);
        values.resize(properties.size());
        for (int unif = 0; unif < numActiveUniforms; ++unif)
        {
            glGetProgramResourceiv(m_id, GL_UNIFORM, unif, properties.size(), &properties[0], values.size(), NULL,
                                   &values[0]);

            nameData.resize(values[0]); // The length of the name.
            glGetProgramResourceName(m_id, GL_UNIFORM, unif, nameData.size(), NULL, &nameData[0]);
            std::string name((char*)&nameData[0], nameData.size() - 1);
            m_uniforms.emplace(name, Uniform{static_cast<GLenum>(values[1]), values[2], values[3]});
        }
    }
}

unsigned int Shader::loadAndCompile(const char* vertexPath, const char* fragmentPath, uint16_t featureMask)
{
    std::string vertexCode;
    std::string fragmentCode;

    if (loadFile(vertexPath, vertexCode, fragmentPath, fragmentCode))
    {
        return 0;
    }

    // parse shader : If #include "path" is found, replace by code
    ShaderParser::parse(vertexCode);
    ShaderParser::parse(fragmentCode);

    // if ((m_featureMask & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    //{
    //    vertexCode.insert(0, lightBlinPhongVertexShaderStr);
    //    fragmentCode.insert(0, lightBlinPhongFragmentShaderStr);
    //}

    if ((featureMask & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
    {
        fragmentCode.insert(0, colorFragmentShaderStr);
    }

    if ((featureMask & SCALE_TIME_ACC) == SCALE_TIME_ACC)
    {
        vertexCode.insert(0, timeScaledAccVertexShaderStr);
    }

    if ((featureMask & UNSCALED_TIME_ACC) == UNSCALED_TIME_ACC)
    {
        vertexCode.insert(0, timeUnscaledAccVertexShaderStr);
    }

    vertexCode.insert(0, versionHeaderStr);
    fragmentCode.insert(0, versionHeaderStr);

    return compile(vertexCode, fragmentCode);
}

void Shader::release()
{
    glDeleteBuffers(1, &m_lightsUniformBuffer);
    glDeleteProgram(m_id);

    Log::getInstance()->log("Release vs and fs");
}