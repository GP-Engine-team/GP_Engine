#include "Engine/Resources/Material.hpp"
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Resources/Shader.hpp>

using namespace GPE;

unsigned int Material::materialCount = 0;

void Material::resetUniform()
{
    uniforms.clear();
    for (auto&& [key, value] : m_pShader->getUniforms())
    {
        switch (value.type)
        {
        case GL_FLOAT:
            uniforms.try_emplace(key, std::make_unique<floatUniform>());
            break;

        case GL_INT:
            uniforms.try_emplace(key, std::make_unique<intUniform>());
            break;

        case GL_FLOAT_VEC2:
            uniforms.try_emplace(key, std::make_unique<vec2Uniform>());
            break;

        case GL_FLOAT_VEC3:
            uniforms.try_emplace(key, std::make_unique<vec3Uniform>());
            break;

        default:
            // List of type here : https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
            Log::getInstance()->logWarning(stringFormat("Uniform format with value : %d, not implemented", value.type));
            break;
        }
    }
}