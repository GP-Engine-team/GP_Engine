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

    loadInGPU(arg.w, arg.h, arg.anisotropy, arg.textureMinFilter, arg.textureMagFilter, arg.textureWrapS,
              arg.textureWrapT, arg.pixels.get(), arg.generateMipmaps);

    Log::getInstance()->log("Texture loaded to VRAM");
}

Texture::Texture(const CreateArg& arg) noexcept : format{arg.format}
{
    Texture::loadInGPU(arg.width, arg.height, arg.anisotropy, arg.textureMinFilter, arg.textureMagFilter,
                       arg.textureWrapS, arg.textureWrapT, nullptr);

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
        format = EFormat::R;
        break;

    case 2:
        format = EFormat::RG;
        break;

    case 3:
        format = EFormat::RGB;
        break;

    case 4:
        format = EFormat::RGBA;
        break;

    default:
        format = EFormat::NONE;
        return;
    }
}

bool Texture::checkFormatValidity() const
{
    switch (format)
    {
    case EFormat::R:
    case EFormat::RG:
    case EFormat::RGB:
    case EFormat::RGBA:
        return true;
    }

    FUNCT_WARNING((std::string("Unsupported texture format: ") + std::to_string((GLenum)format)).c_str());
    return false;
}

bool Texture::loadInGPU(int w, int h, unsigned int anisotropy, ETextureMinFilter textureMinFilter,
                        ETextureMagFilter textureMagFilter, ETextureWrapS textureWrapS, ETextureWrapT textureWrapT,
                        unsigned char* pixels, bool generateMipmaps) noexcept
{
    if (!checkFormatValidity())
    {
        return false;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(textureMinFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(textureMagFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(textureWrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(textureWrapT));

    GLfloat max_anisotropy; /* don't exceed this value...*/
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);

    anisotropy = (anisotropy > max_anisotropy) ? max_anisotropy : anisotropy;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);

    glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)format, w, h, 0, (GLenum)format, GL_UNSIGNED_BYTE, pixels);

    if (generateMipmaps)
        glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

void Texture::resize(int width, int height) noexcept
{
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)format, width, height, 0, (GLenum)format, GL_UNSIGNED_BYTE, nullptr);
}