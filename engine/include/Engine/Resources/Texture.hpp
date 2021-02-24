/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Type.hpp"
#include <string>

//in inl
#include "Engine/Core/Debug/Assert.hpp"
#include "glad/glad.h"

namespace GPE
{
#define PATH_TEXTURE_RESOURCE "./resources/textures/"

class Texture
{
public:
    enum class EWrapType
    {
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        MIRRORED_REPEAT,
        REPEAT
    };

    enum class EFilterType
    {
        NEAREST,
        LINEAR
    };

    struct CreateArg
    {
        const char* path;
        EWrapType   wrapType    = EWrapType::REPEAT;
        EFilterType filterType  = EFilterType::LINEAR;
        bool        flipTexture = true;
        bool        loadInGPU   = true;
    };

protected:
    unsigned int m_id          = 0;
    bool         m_isLoadInGPU = true;

    unsigned char* m_pixels{nullptr};
    int            m_w = 0, m_h = 0;
    unsigned char  m_comp       = 0; // RGB = 3 / RGBA = 4
    EFilterType    m_filterType = EFilterType::LINEAR;
    EWrapType      m_wrapType   = EWrapType::REPEAT;
    std::string    m_path       = "";

protected:
    /**
     * @brief inverse pixels of image
     *
     * @param char
     */
    void hFlip() noexcept;

public:
    Texture()                     = default;
    Texture(const Texture& other) = delete;
    Texture(Texture&& other)      = default;

    Texture(const char* path, bool flipTexture = true, bool loadInGPU = true) noexcept;
    Texture(const CreateArg& arg) noexcept;
    virtual ~Texture() noexcept;

    inline unsigned int         getID() const noexcept;

    inline const unsigned char* getPixels() const noexcept;

    inline unsigned char* getPixels() noexcept;

    /**
     * @brief Load texture and Mesh from CPU to GPU. This operation can be slow but display element more faster.
     *
     */
    virtual void loadInGPU() noexcept;

    void unloadFromGPU() noexcept;

    /**
     * @brief return true if texture is load in GPU and ready to use
     *
     * @return true
     * @return false
     */
    inline bool isLoadInGPU() const noexcept;

    /**
     * @brief bind texture to openGL to be use for the next drawing
     *
     */
    inline void use() const noexcept;

    inline Size getSize() const noexcept;
};

#include "Texture.inl"

} /*namespace GPE*/