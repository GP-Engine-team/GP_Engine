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
        NEAREST                = GL_NEAREST,
        LINEAR                 = GL_LINEAR,
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
    };

    enum class ETextureMagFilter
    {
        NEAREST = GL_NEAREST,
        LINEAR  = GL_LINEAR,
    };

    enum class ETextureWrapS
    {
        CLAMP_TO_EDGE        = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER      = GL_CLAMP_TO_BORDER,
        MIRRORED_REPEAT      = GL_MIRRORED_REPEAT,
        REPEAT               = GL_REPEAT,
        MIRROR_CLAMP_TO_EDGE = GL_MIRRORED_REPEAT
    };

    enum class ETextureWrapT
    {
        CLAMP_TO_EDGE        = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER      = GL_CLAMP_TO_BORDER,
        MIRRORED_REPEAT      = GL_MIRRORED_REPEAT,
        REPEAT               = GL_REPEAT,
        MIRROR_CLAMP_TO_EDGE = GL_MIRRORED_REPEAT
    };

    enum class EFormat : GLenum
    {
        NONE = 0u,
        R    = GL_R32F,
        RG   = GL_RG,
        RGB  = GL_RGB,
        RGBA = GL_RGBA
    };

    enum class ERenderBufferType
    {
        DEPTH_COMPONENT16  = 1,
        DEPTH_COMPONENT24  = 2,
        DEPTH_COMPONENT32F = 3,
        STENCIL_INDEX8     = 4
    };

    struct LoadArg
    {
        std::string       path             = "";
        ETextureMinFilter textureMinFilter = ETextureMinFilter::NEAREST_MIPMAP_LINEAR;
        ETextureMagFilter textureMagFilter = ETextureMagFilter::LINEAR;
        ETextureWrapS     textureWrapS     = ETextureWrapS::REPEAT;
        ETextureWrapT     textureWrapT     = ETextureWrapT::REPEAT;
        unsigned int      anisotropy       = 8;
        bool              flipTexture      = false;
        bool              generateMipmaps  = true;
    };

    struct ImportArg
    {
        int                            w = 0, h = 0, comp = 0, len = 0;
        std::unique_ptr<unsigned char> pixels           = nullptr;
        ETextureMinFilter              textureMinFilter = ETextureMinFilter::NEAREST_MIPMAP_LINEAR;
        ETextureMagFilter              textureMagFilter = ETextureMagFilter::LINEAR;
        ETextureWrapS                  textureWrapS     = ETextureWrapS::REPEAT;
        ETextureWrapT                  textureWrapT     = ETextureWrapT::REPEAT;
        unsigned int                   anisotropy       = 8;
        bool                           flipTexture      = false;
        bool                           generateMipmaps  = true;
    };

    struct CreateArg
    {
        int               width            = 0;
        int               height           = 0;
        EFormat           format           = EFormat::RGBA;
        ETextureMinFilter textureMinFilter = ETextureMinFilter::NEAREST_MIPMAP_LINEAR;
        ETextureMagFilter textureMagFilter = ETextureMagFilter::LINEAR;
        ETextureWrapS     textureWrapS     = ETextureWrapS::REPEAT;
        ETextureWrapT     textureWrapT     = ETextureWrapT::REPEAT;
        unsigned int      anisotropy       = 8;
    };

protected:
    GLuint  m_id   = 0u;
    EFormat format = EFormat::NONE;

    void setFormat(int channels);
    bool checkFormatValidity() const;

    bool loadInGPU(int w, int h, unsigned int anisotropy, ETextureMinFilter textureMinFilter,
                   ETextureMagFilter textureMagFilter, ETextureWrapS textureWrapS, ETextureWrapT textureWrapT,
                   unsigned char* pixels, bool generateMipmaps = true) noexcept;

public:
    Texture()                     = default;
    Texture(const Texture& other) = delete;
    Texture(Texture&& other)      = default;

    Texture(const LoadArg& arg) noexcept;
    Texture(const ImportArg& arg) noexcept;
    Texture(const CreateArg& arg) noexcept;
    ~Texture() noexcept;

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
