namespace GPE
{
unsigned int Shader::getID() const noexcept
{
    return m_id;
};

uint16_t Shader::getFeature() const noexcept
{
    return m_featureMask;
}

inline void Shader::sendData(const char* name, bool data) const noexcept
{
    glUniform1i(glGetUniformLocation(m_id, name), (int)data);
}

inline void Shader::sendData(const char* name, int data) const noexcept
{
    glUniform1i(glGetUniformLocation(m_id, name), data);
}

inline void Shader::sendData(const char* name, float data) const noexcept
{
    glUniform1f(glGetUniformLocation(m_id, name), data);
}

inline void Shader::sendData(const char* name, const GPM::Vec2& data) const noexcept
{
    glUniform2f(glGetUniformLocation(m_id, name), data.x, data.y);
}

inline void Shader::sendData(const char* name, const GPM::Vec3& data) const noexcept
{
    glUniform3f(glGetUniformLocation(m_id, name), data.x, data.y, data.z);
}

inline void Shader::sendData(const char* name, const GPM::Vec4& data) const noexcept
{
    glUniform4f(glGetUniformLocation(m_id, name), data.x, data.y, data.z, data.w);
}

inline void Shader::sendData(const char* name, const GPM::Mat3& data) const noexcept
{
    glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, data.e);
}

inline void Shader::sendData(const char* name, const GPM::Mat4& data) const noexcept
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, data.e);
}

inline void Shader::sendData(const char* name, const GPM::Vec3* data, size_t count) const noexcept
{
    glUniform3fv(glGetUniformLocation(m_id, name), count, data->e);
}

inline void Shader::sendData(const char* name, const GPM::Vec4* data, size_t count) const noexcept
{
    glUniform4fv(glGetUniformLocation(m_id, name), count, data->e);
}

inline void Shader::sendData(const char* name, const GPM::Mat3* data, size_t count) const noexcept
{
    glUniformMatrix3fv(glGetUniformLocation(m_id, name), count, GL_FALSE, data->e);
}

inline void Shader::sendData(const char* name, const GPM::Mat4* data, size_t count) const noexcept
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), count, GL_FALSE, data->e);
}
} // namespace GPE