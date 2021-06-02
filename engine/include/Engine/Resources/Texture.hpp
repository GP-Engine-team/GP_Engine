/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *  found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Type.hpp"
#include <memory>
#include <string>

// in inl
#include "Engine/Core/Debug/Assert.hpp"
#include <glad/glad.h>

namespace GPE
{
#define PATH_TEXTURE_RESOURCE "./resources/textures/"

class Texture
{
public:
    enum class ETextureMinFilter
    {
        NEAREST,
        LINEAR,
        NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR,
        COUNT
    };

    enum class ETextureMagFilter
    {
        NEAREST,
        LINEAR,
        COUNT
    };

    enum class ETextureWrap
    {
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        MIRRORED_REPEAT,
        REPEAT,
        MIRROR_CLAMP_TO_EDGE,
        COUNT
    };

    enum class EFormat : GLenum
    {
        NONE,
        R,
        RG,
        RGB,
        RGBA,
        COUNT
    };

    enum class ERenderBufferType
    {
        DEPTH_COMPONENT16  = 1,
        DEPTH_COMPONENT24  = 2,
        DEPTH_COMPONENT32F = 3,
        STENCIL_INDEX8     = 4
    };

    struct RenderProperties
    {
        ETextureMinFilter textureMinFilter = ETextureMinFilter::NEAREST_MIPMAP_LINEAR;
        ETextureMagFilter textureMagFilter = ETextureMagFilter::LINEAR;
        ETextureWrap      textureWrapS     = ETextureWrap::REPEAT;
        ETextureWrap      textureWrapT     = ETextureWrap::REPEAT;
        unsigned int      anisotropy       = 8;
        bool              generateMipmaps  = true;
    };

    struct LoadArg
    {
        std::string      path = "";
        RenderProperties properties;
        bool             flipTexture = false;
    };

    struct ImportArg
    {
        int                            w = 0, h = 0, comp = 0, len = 0;
        std::unique_ptr<unsigned char> pixels = nullptr;
        RenderProperties               properties;
    };

    struct CreateArg
    {
        int              width  = 0;
        int              height = 0;
        EFormat          format = EFormat::RGBA;
        RenderProperties properties;
    };

protected:
    GLuint  m_id     = 0u;
    EFormat m_format = EFormat::NONE;

    void setFormat(int channels);
    bool checkFormatValidity() const;

    bool loadInGPU(int w, int h, unsigned char* pixels, const RenderProperties& props) noexcept;

    GLenum getGLFormat(EFormat format);
    GLenum getGLInternalFormat(EFormat format);

    GLenum getGLTextureWrap(ETextureWrap format);
    GLenum getGLTextureMagFilter(ETextureMagFilter format);
    GLenum getGLTextureMinFilter(ETextureMinFilter format);

public:
    Texture()                     = default;
    Texture(const Texture& other) = delete;
    Texture(Texture&& other)      = default;

    Texture(const LoadArg& arg) noexcept;
    Texture(const ImportArg& arg) noexcept;
    Texture(const CreateArg& arg) noexcept;
    ~Texture() noexcept;

    void reload(const Texture::ImportArg& m_config);

    inline unsigned int getID() const noexcept;
    /**
     * @brief bind texture to openGL to be use for the next drawing
     *
     */
    inline void use() const noexcept;

    void resize(int width, int height) noexcept;
};

#include "Texture.inl"

} /*namespace GPE*/
