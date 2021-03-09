#include "Engine/Resources/Texture.hpp"

#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Parsers/ParserTool.hpp"
#include "glad/glad.h"

using namespace GPE;

Texture::Texture(const LoadArg& arg) noexcept
{
    stbi_set_flip_vertically_on_load(arg.flipTexture);

    int            w, h, comp;
    unsigned char* pixels = stbi_load(arg.path.c_str(), &w, &h, &comp, 0);

    if (pixels == nullptr)
    {
        FUNCT_ERROR((std::string("STBI canno't load image : ") + arg.path).c_str());
        FUNCT_ERROR(std::string("Loading image failed: ") + stbi_failure_reason());
        return;
    }

    Texture::loadInGPU(w, h, comp, arg.textureMinFilter, arg.textureMagFilter, arg.textureWrapS, arg.textureWrapT,
                       pixels);

    Log::log((std::string("Texture \"") + removeUntilFirstSpaceInPath(arg.path.c_str()) + "\" load in GPU").c_str());
    stbi_image_free(pixels);
}

Texture::Texture(const CreateArg& arg) noexcept
    : format{(GLenum)arg.format}
{
    Texture::loadInGPU(arg.width, arg.height, arg.format, arg.textureMinFilter,
                       arg.textureMagFilter, arg.textureWrapS, arg.textureWrapT, nullptr);

    Log::log((std::string("Texture create of ") + std::to_string(arg.width) + '/' + std::to_string(arg.height) +
              " load in GPU").c_str());
}

Texture::~Texture() noexcept
{
    glDeleteTextures(1, &m_id);
    Log::log("Texture release");
}

void Texture::loadInGPU(int w, int h, EFormat comp, ETextureMinFilter textureMinFilter, ETextureMagFilter textureMagFilter,
                        ETextureWrapS textureWrapS, ETextureWrapT textureWrapT, unsigned char* pixels) noexcept
{
    switch (comp)
    {
    case EFormat::R:
    case EFormat::RG:
    case EFormat::RGB:
    case EFormat::RGBA:
        format = (GLenum)comp;
        break;

    default:
        FUNCT_WARNING((std::string("Unsupported texture format (") + std::to_string(format) + ')').c_str());
        return;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(textureMinFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(textureMagFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(textureWrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(textureWrapT));

    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, pixels);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::resize(int width, int height) noexcept
{
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
}