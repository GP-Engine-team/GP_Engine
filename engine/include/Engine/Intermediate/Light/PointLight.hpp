/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string> //std::string
#include <vector> //std::vectorA

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Intermediate/Light/Light.hpp"
#include "GPM/Vector3.hpp"

namespace Engine::Intermediate
{
class PointLight : public Light
{
public:
    struct CreateArg
    {
        const Engine::Resources::AmbiantComponent&  ambient;
        const Engine::Resources::DiffuseComponent&  diffuse;
        const Engine::Resources::SpecularComponent& specular;

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

    PointLight(Engine::Intermediate::GameObject& owner, const CreateArg& arg);

    PointLight(Engine::Intermediate::GameObject& owner, const Engine::Resources::AmbiantComponent& ambient,
               const Engine::Resources::DiffuseComponent& diffuse, const Engine::Resources::SpecularComponent& specular,
               float constant, float linear, float quadratic);

    PointLight(Engine::Intermediate::GameObject& owner, const std::vector<std::string>& params);

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