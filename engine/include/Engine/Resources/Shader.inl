#include "Engine/Resources/Shader.hpp"

unsigned int Shader::getID() const noexcept
{
    return m_id;
};

uint16_t Shader::getFeature() const noexcept
{
    return m_featureMask;
}

void Shader::setBool(const char* name, bool value) const noexcept
{
    glUniform1i(glGetUniformLocation(m_id, name), (int)value);
}

void Shader::setInt(const char* name, int value) const noexcept
{
    glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::setFloat(const char* name, float value) const noexcept
{
    glUniform1f(glGetUniformLocation(m_id, name), value);
}

void Shader::setVec3(const char* name, float v1, float v2, float v3) const noexcept
{
    glUniform3f(glGetUniformLocation(m_id, name), v1, v2, v3);
}

void Shader::setVec4(const char* name, float v1, float v2, float v3, float v4) const noexcept
{
    glUniform4f(glGetUniformLocation(m_id, name), v1, v2, v3, v4);
}

void Shader::setMat3(const char* name, const float* value) const noexcept
{
    glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, value);
}

void Shader::setMat4(const char* name, const float* value) const noexcept
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, value);
}

void Shader::setpVec3(const char* name, unsigned int count, const float* pV) const noexcept
{
    glUniform3fv(glGetUniformLocation(m_id, name), count, pV);
}

void Shader::setpVec4(const char* name, unsigned int count, const float* pV) const noexcept
{
    glUniform4fv(glGetUniformLocation(m_id, name), count, pV);
}