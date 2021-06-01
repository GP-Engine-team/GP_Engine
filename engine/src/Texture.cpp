#include "Engine/Resources/Texture.hpp"

#include <utility>

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Parsers/ParserTool.hpp"
#include "Engine/Resources/Importer/Importer.hpp"
#include "glad/glad.h"

using namespace GPE;

Texture::Texture(const LoadArg& arg) noexcept : Texture(readTextureFile(arg.path.c_str()))
{
}

Texture::Texture(const ImportArg& arg) noexcept
{
    setFormat(arg.comp);

    loadInGPU(arg.w, arg.h, arg.pixels.get(), arg.properties);

    Log::getInstance()->log("Texture loaded to VRAM");
}

Texture::Texture(const CreateArg& arg) noexcept : m_format{arg.format}
{
    Texture::loadInGPU(arg.width, arg.height, nullptr, arg.properties);

    Log::getInstance()->log(
        (std::to_string(arg.width) + 'x' + std::to_string(arg.height) + " texture loaded in VRAM").c_str());
}

Texture::~Texture() noexcept
{
    glDeleteTextures(1, &m_id);
    Log::getInstance()->log("Texture released");
}

void Texture::setFormat(int channels)
{
    switch (channels)
    {
    case 1:
        m_format = EFormat::R;
        break;

    case 2:
        m_format = EFormat::RG;
        break;

    case 3:
        m_format = EFormat::RGB;
        break;

    case 4:
        m_format = EFormat::RGBA;
        break;

    default:
        m_format = EFormat::NONE;
        return;
    }
}

bool Texture::checkFormatValidity() const
{
    switch (m_format)
    {
    case EFormat::R:
    case EFormat::RG:
    case EFormat::RGB:
    case EFormat::RGBA:
        return true;
    }

    FUNCT_WARNING((std::string("Unsupported texture format: ") + std::to_string((int)m_format)).c_str());
    return false;
}

bool Texture::loadInGPU(int w, int h, unsigned char* pixels, const RenderProperties& props) noexcept
{
    if (!checkFormatValidity())
    {
        return false;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    static_cast<GLint>(getGLTextureMinFilter(props.textureMinFilter)));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    static_cast<GLint>(getGLTextureMagFilter(props.textureMagFilter)));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(getGLTextureWrap(props.textureWrapS)));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(getGLTextureWrap(props.textureWrapT)));

    GLfloat max_anisotropy; /* don't exceed this value...*/
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);

    const unsigned int finalAniso = (props.anisotropy > max_anisotropy) ? max_anisotropy : props.anisotropy;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, finalAniso);

    glTexImage2D(GL_TEXTURE_2D, 0, getGLFormat(m_format), w, h, 0, getGLInternalFormat(m_format), GL_UNSIGNED_BYTE,
                 pixels);

    if (props.generateMipmaps)
        glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

void Texture::reload(const Texture::ImportArg& m_config)
{
    glDeleteTextures(1, &m_id);
    loadInGPU(m_config.w, m_config.h, m_config.pixels.get(), m_config.properties);
}

GLenum Texture::getGLFormat(EFormat format)
{
    switch (format)
    {
    case GPE::Texture::EFormat::R:
        return GL_R32F;
    case GPE::Texture::EFormat::RG:
        return GL_RG;
    case GPE::Texture::EFormat::RGB:
        return GL_RGB;
    case GPE::Texture::EFormat::RGBA:
        return GL_RGBA;
    default:
        return 0u;
    }
}

GLenum Texture::getGLInternalFormat(EFormat format)
{
    switch (format)
    {
    case GPE::Texture::EFormat::R:
        return GL_RED;
    case GPE::Texture::EFormat::RG:
        return GL_RG;
    case GPE::Texture::EFormat::RGB:
        return GL_RGB;
    case GPE::Texture::EFormat::RGBA:
        return GL_RGBA;
    default:
        return 0u;
    }
}

GLenum Texture::getGLTextureWrap(ETextureWrap format)
{
    switch (format)
    {
    case GPE::Texture::ETextureWrap::CLAMP_TO_EDGE:
        return GL_CLAMP_TO_EDGE;
    case GPE::Texture::ETextureWrap::CLAMP_TO_BORDER:
        return GL_CLAMP_TO_BORDER;
    case GPE::Texture::ETextureWrap::MIRRORED_REPEAT:
        return GL_MIRRORED_REPEAT;
    case GPE::Texture::ETextureWrap::REPEAT:
        return GL_REPEAT;
    case GPE::Texture::ETextureWrap::MIRROR_CLAMP_TO_EDGE:
        return GL_MIRROR_CLAMP_TO_EDGE;
    default:
        return 0u;
    }
}
GLenum Texture::getGLTextureMagFilter(ETextureMagFilter format)
{
    switch (format)
    {
    case GPE::Texture::ETextureMagFilter::NEAREST:
        return GL_NEAREST;
    case GPE::Texture::ETextureMagFilter::LINEAR:
        return GL_LINEAR;
    default:
        return 0u;
    }
}
GLenum Texture::getGLTextureMinFilter(ETextureMinFilter format)
{
    switch (format)
    {
    case GPE::Texture::ETextureMinFilter::NEAREST:
        return GL_NEAREST;
    case GPE::Texture::ETextureMinFilter::LINEAR:
        return GL_LINEAR;
    case GPE::Texture::ETextureMinFilter::NEAREST_MIPMAP_NEAREST:
        return GL_NEAREST_MIPMAP_NEAREST;
    case GPE::Texture::ETextureMinFilter::LINEAR_MIPMAP_NEAREST:
        return GL_LINEAR_MIPMAP_NEAREST;
    case GPE::Texture::ETextureMinFilter::NEAREST_MIPMAP_LINEAR:
        return GL_NEAREST_MIPMAP_LINEAR;
    case GPE::Texture::ETextureMinFilter::LINEAR_MIPMAP_LINEAR:
        return GL_LINEAR_MIPMAP_LINEAR;
    default:
        return 0u;
    }
}

void Texture::resize(int width, int height) noexcept
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexImage2D(GL_TEXTURE_2D, 0, getGLFormat(m_format), width, height, 0, getGLInternalFormat(m_format),
                 GL_UNSIGNED_BYTE, nullptr);
}