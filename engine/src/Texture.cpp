#include "Engine/Resources/Texture.hpp"

#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Parsers/ParserTool.hpp"
#include "glad/glad.h"

using namespace GPE;

Texture::Texture(const char* path, bool flipTexture, bool loadInGPU) noexcept
    : m_id(0), m_isLoadInGPU(false), m_path(path)
{
    stbi_set_flip_vertically_on_load(flipTexture);
    m_pixels = stbi_load(path, &m_w, &m_h, (int*)&m_comp, 0);

    if (m_pixels == nullptr)
    {
        FUNCT_ERROR((std::string("STBI canno't load image : ") + path).c_str());
        FUNCT_ERROR(std::string("Loading image failed: ") + stbi_failure_reason());
        return;
    }

    removeUntilFirstSpaceInPath(path);

    if (loadInGPU)
        Texture::loadInGPU();

    Log::log((std::string("Texture \"") + m_path + "\" load").c_str());
}

Texture::Texture(const CreateArg& arg) noexcept : m_id(0), m_isLoadInGPU(false)
{
    m_textureMinFilter = arg.textureMinFilter;
    m_textureMagFilter = arg.textureMagFilter;
    m_textureWrapS     = arg.textureWrapS;
    m_textureWrapT     = arg.textureWrapT;

    stbi_set_flip_vertically_on_load(arg.flipTexture);
    m_pixels = stbi_load(arg.path, &m_w, &m_h, (int*)&m_comp, 0);

    if (m_pixels == nullptr)
    {
        FUNCT_ERROR((std::string("STBI canno't load image : ") + arg.path).c_str());
        return;
    }

    removeUntilFirstSpaceInPath(arg.path);

    if (arg.loadInGPU)
        Texture::loadInGPU();
}

Texture::~Texture() noexcept
{
    if (m_pixels != nullptr)
        stbi_image_free(m_pixels);

    if (m_isLoadInGPU)
        unloadFromGPU();

    Log::log((std::string("Texture \"") + m_path + "\" release").c_str());
}

void Texture::loadInGPU() noexcept
{
    if (m_isLoadInGPU)
    {
        FUNCT_WARNING(std::string("Texture already load in GPU").c_str());
        return;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    if (m_comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_w, m_h, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pixels);
    else if (m_comp == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_w, m_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
    }
    else
    {
        FUNCT_WARNING(std::string("Texture component unsuppported with component : ") + std::to_string(m_comp));
        exit(1);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_textureMinFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_textureMagFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_textureWrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_textureWrapT));

    glBindTexture(GL_TEXTURE_2D, 0);
    m_isLoadInGPU = true;

    Log::log(std::string("Texture load in GPU").c_str());
}

void Texture::unloadFromGPU() noexcept
{
    if (!m_isLoadInGPU)
    {
        FUNCT_WARNING(std::string("Texture isn't load in GPU").c_str());
        return;
    }

    glDeleteTextures(1, &m_id);
    m_isLoadInGPU = false;
}

void Texture::hFlip() noexcept
{
    int wComp = m_w * m_comp;

    for (int i = 0; i < m_h / 2; i++)
    {
        for (int j = 0; j < wComp; j++)
        {
            std::swap(m_pixels[(wComp * (m_h - 1 - i)) + j], m_pixels[(wComp * i) + j]);
        }
    }
}