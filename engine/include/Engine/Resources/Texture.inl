inline unsigned int Texture::getID() const noexcept
{
    return m_id;
}

inline void Texture::use() const noexcept
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}
