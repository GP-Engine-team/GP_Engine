#include "Engine/Resources/Texture.hpp"

#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Parsers/ParserTool.hpp"
#include "glad/glad.h"

using namespace GPE;

Texture::Texture(const char* path, bool flipTexture, bool loadInGPU) : m_id(0), m_isLoadInGPU(false), m_path(path)
{
    m_filterType = EFilterType::LINEAR;
    m_wrapType   = EWrapType::REPEAT;

    stbi_set_flip_vertically_on_load(flipTexture);
    m_pixels = stbi_load(path, &m_w, &m_h, (int*)&m_comp, 0);

    if (m_pixels == nullptr)
    {
        FUNCT_ERROR((std::string("STBI canno't load image : ") + path).c_str());
        FUNCT_ERROR(std::string("Loading image failed: ") + stbi_failure_reason());
        return;
    }

    parseNameInPath(path);

    if (loadInGPU)
        Texture::loadInGPU();

    Log::log((std::string("Texture \"") + m_path + "\" load").c_str());
}

Texture::Texture(const CreateArg& arg) : m_id(0), m_isLoadInGPU(false)
{
    m_filterType = arg.filterType;
    m_wrapType   = arg.wrapType;

    stbi_set_flip_vertically_on_load(arg.flipTexture);
    m_pixels = stbi_load(arg.path, &m_w, &m_h, (int*)&m_comp, 0);

    if (m_pixels == nullptr)
    {
        FUNCT_ERROR((std::string("STBI canno't load image : ") + arg.path).c_str());
        return;
    }

    parseNameInPath(arg.path);

    if (arg.loadInGPU)
        Texture::loadInGPU();
}

Texture::~Texture()
{
    if (m_pixels != nullptr)
        stbi_image_free(m_pixels);

    if (m_isLoadInGPU)
        unloadFromGPU();

    Log::log((std::string("Texture \"") + m_path + "\" release").c_str());
}

void Texture::loadInGPU()
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

    GLint wrap, filter;

    switch (m_wrapType)
    {
    case EWrapType::CLAMP_TO_BORDER:
        wrap = GL_CLAMP_TO_BORDER;
        break;

    case EWrapType::CLAMP_TO_EDGE:
        wrap = GL_CLAMP_TO_EDGE;
        break;

    case EWrapType::MIRRORED_REPEAT:
        wrap = GL_MIRRORED_REPEAT;
        break;

    case EWrapType::REPEAT:
        wrap = GL_REPEAT;
        break;

    default:
        wrap = GL_CLAMP_TO_EDGE;
        break;
    }

    switch (m_filterType)
    {
    case EFilterType::LINEAR:
        filter = GL_LINEAR;
        break;

    case EFilterType::NEAREST:
        filter = GL_NEAREST;
        break;

    default:
        filter = GL_LINEAR;
        break;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glBindTexture(GL_TEXTURE_2D, 0);
    m_isLoadInGPU = true;

    Log::log(std::string("Texture load in GPU").c_str());
}

void Texture::unloadFromGPU()
{
    if (!m_isLoadInGPU)
    {
        FUNCT_WARNING(std::string("Texture isn't load in GPU").c_str());
        return;
    }

    glDeleteTextures(1, &m_id);
    m_isLoadInGPU = false;
}

void Texture::hFlip()
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

void Texture::use() const noexcept
{
    GPE_ASSERT(isLoadInGPU(), "Try to use texture not load in GPU");

    glBindTexture(GL_TEXTURE_2D, m_id);
}