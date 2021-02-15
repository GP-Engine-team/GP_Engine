/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>

#include "Engine/Intermediate/Light/Light.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "GPM/Vector3.hpp"

namespace Engine::Intermediate
{
struct PointLightCreateArg
{
    const Engine::Resources::AmbiantComponent&  ambient;
    const Engine::Resources::DiffuseComponent&  diffuse;
    const Engine::Resources::SpecularComponent& specular;

    float constant;
    float linear;
    float quadratic;
    bool  isEnable{true};
};

class PointLight : public Light
{
protected:
    float m_constant, m_linear, m_quadratic;

public:
    PointLight() = default;

    PointLight(Engine::Intermediate::GameObject& owner, const PointLightCreateArg& arg);

    PointLight(Engine::Intermediate::GameObject& owner, const Engine::Resources::AmbiantComponent& ambient,
               const Engine::Resources::DiffuseComponent& diffuse, const Engine::Resources::SpecularComponent& specular,
               float constant, float linear, float quadratic, bool isEnable = true);

    PointLight(Engine::Intermediate::GameObject& owner, const std::vector<std::string>& params);

    PointLight(const PointLight& other) = default;
    PointLight(PointLight&& other)      = default;
    virtual ~PointLight()               = default;

    virtual void addToLightToUseBuffer(std::vector<Engine::Resources::LightData>& lb) noexcept override;

    float getLinear()
    {
        return m_linear;
    }
    void setLinear(float linear)
    {
        m_linear = linear;
    }
};
} /*namespace Engine::Intermediate*/