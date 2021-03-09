/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string> //std::string
#include <vector> //std::vectorA

#include "Engine/Core/Tools/ClassUtility.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Light/Light.hpp"
#include "GPM/Vector3.hpp"

namespace GPE
{
class PointLight : public Light
{
public:
    struct CreateArg
    {
        const AmbiantComponent&  ambient;
        const DiffuseComponent&  diffuse;
        const SpecularComponent& specular;

        float constant;
        float linear;
        float quadratic;
        bool  isEnable{true};
    };

protected:
    float m_constant, m_linear, m_quadratic;

public:
    PointLight(const PointLight& other) = delete;
    PointLight(PointLight&& other)      = default;
    virtual ~PointLight()               = default;

    PointLight()        = delete;
    PointLight& operator=(PointLight const& other) = delete;
    PointLight& operator=(PointLight&& other) = default;

    PointLight(GameObject& owner, const CreateArg& arg) noexcept;

    PointLight(GameObject& owner, const AmbiantComponent& ambient, const DiffuseComponent& diffuse,
               const SpecularComponent& specular, float constant, float linear, float quadratic) noexcept;

    void addToLightToUseBuffer(std::vector<LightData>& lb) noexcept override;

    DEFAULT_GETTER_SETTER_BY_REF(Constant, m_constant);
    DEFAULT_GETTER_SETTER_BY_REF(Linear, m_linear);
    DEFAULT_GETTER_SETTER_BY_REF(Quadratic, m_quadratic);
};
} /*namespace GPE*/
