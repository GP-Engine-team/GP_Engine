#include "Engine/Resources/Material.hpp"
#include <Engine/Resources/Shader.hpp>

using namespace GPE;

unsigned int Material::materialCount = 0;

void Material::updateUniform()
{
    for (auto&& [key, value] : m_pShader->getUniforms())
    {
        switch (value.type)
        {
        case GL_FLOAT:
            floatUniforms[key] = 0.f;
            break;

        case GL_INT:
            intUniforms[key] = 0;
            break;

        default:
            // List of type here : https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
            Log::getInstance()->logWarning(stringFormat("Uniform format with value : %d, not implemented", value.type));
            break;
        }
    }
}