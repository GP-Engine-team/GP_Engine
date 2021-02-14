/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/Type.hpp"
#include <string>

namespace Engine::Resources
{
#define PATH_TEXTURE_RESOURCE "./resources/textures/"

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

struct TextureCreateArg
{
    const char* path;
    EWrapType wrapType = EWrapType::REPEAT;
    EFilterType filterType = EFilterType::LINEAR;
    bool flipTexture = true;
    bool loadInGPU = true;
};

class Texture
{
protected:
    unsigned int m_id = 0;
    bool m_isLoadInGPU = true;

    unsigned char* m_pixels{nullptr};
    int m_w = 0, m_h = 0;
    unsigned char m_comp = 0; // RGB = 3 / RGBA = 4
    EFilterType m_filterType = EFilterType::LINEAR;
    EWrapType m_wrapType = EWrapType::REPEAT;
    std::string m_path = "";

protected:
    /**
     * @brief inverse pixels of image
     *
     * @param char
     */
    void hFlip();

public:
    Texture() = default;
    Texture(const Texture& other) = delete; // TODO;
    Texture(Texture&& other) = default;     // TODO;

    Texture(const char* path, bool flipTexture = true, bool loadInGPU = true);
    Texture(const TextureCreateArg& arg);
    virtual ~Texture();

    unsigned int getID() const
    {
        return m_id;
    }
    const unsigned char* getPixels() const
    {
        return m_pixels;
    } // read only
    unsigned char* getPixels()
    {
        return m_pixels;
    } // read/write

    /**
     * @brief Load texture and Mesh from CPU to GPU. This operation can be slow but display element more faster.
     *
     */
    virtual void loadInGPU();

    void unloadFromGPU();

    /**
     * @brief return true if texture is load in GPU and ready to use
     *
     * @return true
     * @return false
     */
    bool isLoadInGPU() const noexcept
    {
        return m_isLoadInGPU;
    }

    /**
     * @brief bind texture to openGL to be use for the next drawing
     *
     */
    void use() const noexcept;

    Size getSize() const noexcept
    {
        return {m_w, m_h};
    }

    // Texture& operator=(Texture const &other); //TODO:
};
} /*namespace Engine::Resources*/