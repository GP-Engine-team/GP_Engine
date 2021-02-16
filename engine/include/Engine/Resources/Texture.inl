#include "Engine/Resources/Texture.hpp"

unsigned int Texture::getID() const noexcept
{
    return m_id;
}

const unsigned char* Texture::getPixels() const noexcept
{
    return m_pixels;
}

unsigned char* Texture::getPixels() noexcept
{
    return m_pixels;
}

bool Texture::isLoadInGPU() const noexcept
{
    return m_isLoadInGPU;
}

Size Texture::getSize() const noexcept
{
    return {m_w, m_h};
}

void Texture::use() const noexcept
{
    GPE_ASSERT(isLoadInGPU(), "Try to use texture not load in GPU");
    glBindTexture(GL_TEXTURE_2D, m_id);
}
