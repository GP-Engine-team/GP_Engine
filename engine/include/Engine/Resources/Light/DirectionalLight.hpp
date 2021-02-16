/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Light/Light.hpp"
#include "Engine/Resources/ShaderType.hpp"
#include "GPM/Vector3.hpp"

//in inl
#include <string>

namespace GPE
{
class DirectionalLight : public Light
{
public:
    struct CreateArg
    {
        const GPM::Vec3&         direction;
        const AmbiantComponent&  ambient;
        const DiffuseComponent&  diffuse;
        const SpecularComponent& specular;
    };

protected:
    GPM::Vec3 m_direction;

public:
    DirectionalLight(const DirectionalLight& other) = delete;
    DirectionalLight(DirectionalLight&& other)      = default;
    virtual ~DirectionalLight()                     = default;

    DirectionalLight()        = delete;
    DirectionalLight& operator=(DirectionalLight const& other) = delete;
    DirectionalLight& operator=(DirectionalLight&& other) = default;

    inline
    DirectionalLight(GameObject& owner, const CreateArg& arg) noexcept;

    inline
    DirectionalLight(GameObject& owner, const GPM::Vec3& direction, const AmbiantComponent& ambient,
                     const DiffuseComponent& diffuse, const SpecularComponent& specular) noexcept;

    virtual void addToLightToUseBuffer(std::vector<LightData>& lb) noexcept override;

    inline void setDirection(const GPM::Vec3& newDirection) noexcept;
};

#include "DirectionalLight.inl"

} /*namespace GPE*/
