/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <memory>

#include "Engine/Resources/ShaderType.hpp"
#include "Engine/Resources/Texture.hpp"
#include "Engine/Resources/Type.hpp"

namespace Engine::Resources
{
struct MaterialAndTextureCreateArg
{
    std::string name{""};
    MaterialComponent comp{{1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, 1.f};
    const char* pathDiffuseTexture{nullptr};
    EWrapType wrapType{EWrapType::REPEAT};
    EFilterType filterType{EFilterType::LINEAR};
    bool flipTexture{true};
    bool loadInGPU{true};
};

struct MaterialCreateArg
{
    std::string name{""};
    MaterialComponent comp{{1.f, 1.f, 1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, 1.f};
    std::unique_ptr<Texture> pTexture{nullptr};
};

class Material
{
protected:
    MaterialComponent m_comp;
    std::shared_ptr<Texture> m_pDiffuseTexturetexture;
    std::string m_name;

public:
    Material(std::string m_name, MaterialComponent comp);

    Material(const MaterialAndTextureCreateArg& arg);
    Material(MaterialCreateArg& arg);

    /**
     * @brief Construct a new Material object after parse mtl files
     *
     * @param arg
     */
    Material(const MaterialAttrib& arg);

    Material() = default;
    Material(const Material& other) = default;
    Material(Material&& other) = default;
    ~Material() = default;
    Material& operator=(Material const& other) = default;
    Material& operator=(Material&& other) = default;

    /**
     * @brief Get the Material Component object
     *
     * @return const MaterialComponent&
     */
    const MaterialComponent& getMaterialComponent() const noexcept
    {
        return m_comp;
    }

    /**
     * @brief Get the pointor to diffuse texture object
     *
     * @return const Texture*
     */
    Texture* getPDiffuseTexture() noexcept
    {
        return m_pDiffuseTexturetexture.get();
    }

    /**
     * @brief Get the Name object
     *
     * @return const std::string&
     */
    const std::string& getName() const noexcept
    {
        return m_name;
    }
};
} /*namespace Engine::Resources*/