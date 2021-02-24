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

namespace GPE
{
class Material
{
public:
    struct CreateArg
    {
        std::string       name{""};
        MaterialComponent comp{{1.f, 1.f, 1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, 1.f};
        Texture*          pTexture{nullptr};
    };

protected:
    std::string       m_name{""};
    MaterialComponent m_comp{{1.f, 1.f, 1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, 1.f};
    Texture*          m_pDiffuseTexture{nullptr};

public:
    inline Material(const CreateArg& arg);

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
    inline const MaterialComponent& getMaterialComponent() const noexcept;

    /**
     * @brief Get the pointor to diffuse texture object
     *
     * @return const Texture*
     */
    inline const Texture* getDiffuseTexture() const noexcept;

    /**
     * @brief Get the Name object
     *
     * @return const std::string&
     */
    inline const std::string& getName() const noexcept;
};

#include "Material.inl"

} /*namespace GPE*/