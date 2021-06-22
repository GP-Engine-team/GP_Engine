#include "Engine/Resources/Material.hpp"
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/Resources/Shader.hpp>

using namespace GPE;

unsigned int Material::materialCount = 0;

std::unique_ptr<IUniform> createUniform(GLenum type)
{
    switch (type)
    {
    case GL_FLOAT:
        return std::make_unique<floatUniform>();
    case GL_INT:
        return std::make_unique<intUniform>();
    case GL_FLOAT_VEC2:
        return std::make_unique<vec2Uniform>();
    case GL_FLOAT_VEC3:
        return std::make_unique<vec3Uniform>();

    default:
        // List of type here : https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
        Log::getInstance()->logWarning(stringFormat("Uniform format with value : %d, not implemented", type));
        return nullptr;
    }
}

void Material::resetUniform()
{
    uniforms.clear();
    for (auto&& [key, value] : m_pShader->getUniforms())
    {
        if (std::unique_ptr<IUniform> ptr = createUniform(value.type))
        {
            uniforms.try_emplace(key, std::move(ptr));
        }
    }
}

void Material::updateUniform()
{
    std::map<std::string, Shader::Uniform>                     copyShadersUniforms = m_pShader->getUniforms();
    std::unordered_map<std::string, std::unique_ptr<IUniform>> copyUniform         = std::move(uniforms);
    uniforms.clear();

    for (auto&& itSU = copyShadersUniforms.begin(); itSU != copyShadersUniforms.end();)
    {
        auto&& itU = copyUniform.find(itSU->first);
        if (itU != copyUniform.end())
        {
            // Add item that already exist
            uniforms.emplace(itU->first, std::move(itU->second));
            copyUniform.erase(itU);
            itSU = copyShadersUniforms.erase(itSU);
        }
        else
        {
            // Add item that dosn't exist
            if (std::unique_ptr<IUniform> ptr = createUniform(itSU->second.type))
            {
                uniforms.try_emplace(itSU->first, std::move(ptr));
            }
            ++itSU;
        }
    }
}