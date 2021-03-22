#include "Engine/Resources/Shader.hpp"

#include <fstream>
#include <sstream>

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Parsers/ShaderParser.hpp"
#include "Engine/Resources/Color.hpp"

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
    float diff = max(dot(normal, lightDir), 0.0);
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

Shader::Shader(const char* vertexPath, const char* fragmentPath, unsigned char featureMask)
    : m_featureMask(featureMask), m_lightsUniformBuffer(0)
{
    std::string vertexCode;
    std::string fragmentCode;

    removeUntilFirstSpace(vertexPath, m_nameVertex);
    removeUntilFirstSpace(fragmentPath, m_nameFragment);
    if (loadFile(vertexPath, vertexCode, fragmentPath, fragmentCode))
    {
        return;
    }

    // parse shader : If #include "path" is found, replace by code
    ShaderParser::parse(vertexCode);
    ShaderParser::parse(fragmentCode);

    if ((m_featureMask & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        vertexCode.insert(0, lightBlinPhongVertexShaderStr);
        fragmentCode.insert(0, lightBlinPhongFragmentShaderStr);
    }

    if ((m_featureMask & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
    {
        fragmentCode.insert(0, colorFragmentShaderStr);
    }

    if ((m_featureMask & SCALE_TIME_ACC) == SCALE_TIME_ACC)
    {
        vertexCode.insert(0, timeScaledAccVertexShaderStr);
    }

    if ((m_featureMask & UNSCALED_TIME_ACC) == UNSCALED_TIME_ACC)
    {
        vertexCode.insert(0, timeUnscaledAccVertexShaderStr);
    }

    vertexCode.insert(0, versionHeaderStr);
    fragmentCode.insert(0, versionHeaderStr);

    compile(vertexCode, fragmentCode);

    Log::getInstance()->log((std::string("Load and compile shaders \"") + m_nameVertex.c_str() + "\" shader and \"" +
                             m_nameFragment + "\" shader done")
                                .c_str());
}

Shader::~Shader() noexcept
{
    glDeleteBuffers(1, &m_lightsUniformBuffer);
    glDeleteShader(m_id);

    Log::getInstance()->log(
        (std::string("Release ") + m_nameVertex.c_str() + ".vs and " + m_nameFragment + ".fs").c_str());
}

void Shader::use()
{
    int idShaderUse;
    glGetIntegerv(GL_CURRENT_PROGRAM, &idShaderUse);

    if ((unsigned int)idShaderUse != m_id)
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
                exit(0);
            }

            glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, m_lightsUniformBuffer);
        }

        setInt("numberLightUse", static_cast<int>(lightBuffer.size()));
        setVec3("viewPos", viewPos.x, viewPos.y, viewPos.z);
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
        setVec4("material.ambient", material.ambient.rgbi.x, material.ambient.rgbi.y, material.ambient.rgbi.z,
                material.ambient.rgbi.w);
        setVec4("material.diffuse", material.diffuse.rgbi.x, material.diffuse.rgbi.y, material.diffuse.rgbi.z,
                material.diffuse.rgbi.w);
        setVec4("material.specular", material.specular.rgbi.x, material.specular.rgbi.y, material.specular.rgbi.z,
                material.specular.rgbi.w);
        setFloat("material.shininess", material.shininess);
        setFloat("material.opacity", material.opacity);
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

void Shader::compile(std::string& vertexCode, std::string& fragmentCode)
{
    // compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    const char* vShaderCode = vertexCode.c_str();
    vertex                  = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, EType::VERTEX);

    // fragment Shader
    const char* fShaderCode = fragmentCode.c_str();
    fragment                = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, EType::FRAGMENT);

    // shader Program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    checkCompileErrors(m_id, EType::PROGRAM);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::checkCompileErrors(unsigned int shader, EType type)
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
                Log::getInstance()->logError(std::string("Shader name's \"") + m_nameVertex + "\" compilation error\n" +
                                             infoLog);
            }
            else if (type == EType::FRAGMENT)
            {
                Log::getInstance()->logError(std::string("Shader name's \"") + m_nameFragment +
                                             "\" compilation error\n" + infoLog);
            }
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
                Log::getInstance()->logError(
                    (std::string("Shader name's \"") + m_nameVertex.c_str() + "\" linking error.\n" + infoLog).c_str());
            }
            else if (type == EType::FRAGMENT)
            {
                Log::getInstance()->logError(
                    (std::string("Shader name's \"") + m_nameFragment.c_str() + "\" linking error.\n" + infoLog)
                        .c_str());
            }
        }
    }
}

void Shader::removeUntilFirstSpace(const char* path, std::string& shaderName)
{
    std::string src(path);

    std::size_t cursStart = src.find_last_of('/');
    if (cursStart == std::string::npos)
        cursStart = 0;
    cursStart += 1;
    std::size_t cursEnd = src.find_last_of('.');

    shaderName = src.substr(cursStart, cursEnd - cursStart);
}
