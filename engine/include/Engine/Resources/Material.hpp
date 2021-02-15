/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <memory> //std::unique_ptr, std::shared_ptr
#include <string> //std::string

#include "Engine/Resources/ShaderType.hpp"
#include "Engine/Resources/Texture.hpp"
#include "Engine/Resources/Type.hpp"

namespace Engine::Resources
{
class Material
{
public:
    struct CreateArgWithTexture
    {
        std::string          name{""};
        MaterialComponent    comp{{1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, 1.f};
        const char*          pathDiffuseTexture{nullptr};
        Texture::EWrapType   wrapType{Texture::EWrapType::REPEAT};
        Texture::EFilterType filterType{Texture::EFilterType::LINEAR};
        bool                 flipTexture{true};
        bool                 loadInGPU{true};
    };

    struct CreateArg
    {
        std::string              name{""};
        MaterialComponent        comp{{1.f, 1.f, 1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, 1.f};
        std::unique_ptr<Texture> pTexture{nullptr};
    };

protected:
    MaterialComponent        m_comp;
    std::shared_ptr<Texture> m_pDiffuseTexture;
    std::string              m_name;

public:
    Material(const CreateArgWithTexture& arg);
    Material(CreateArg& arg);

    /**
     * @brief Construct a new Material object after parse mtl files
     *
     * @param arg
     */
    Material(const MaterialAttrib& arg);

    Material()                      = default;
    Material(const Material& other) = default;
    Material(Material&& other)      = default;
    ~Material()                     = default;
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
        return m_pDiffuseTexture.get();
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