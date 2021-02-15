/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Intermediate/Light/Light.hpp"
#include "Engine/Resources/ShaderType.hpp"
#include "GPM/Vector3.hpp"

namespace Engine::Intermediate
{
struct DirectionnalLightCreateArg
{
    const GPM::Vec3&                            direction;
    const Engine::Resources::AmbiantComponent&  ambient;
    const Engine::Resources::DiffuseComponent&  diffuse;
    const Engine::Resources::SpecularComponent& specular;
};

class DirectionnalLight : public Light
{
protected:
    GPM::Vec3 m_direction;

public:
    DirectionnalLight(const DirectionnalLight& other) = delete;
    DirectionnalLight(DirectionnalLight&& other)      = default;
    virtual ~DirectionnalLight()                      = default;

    DirectionnalLight()        = delete;
    DirectionnalLight& operator=(DirectionnalLight const& other) = delete;
    DirectionnalLight& operator=(DirectionnalLight&& other) = default;

    DirectionnalLight(Engine::Intermediate::GameObject& owner, const DirectionnalLightCreateArg& arg);

    DirectionnalLight(Engine::Intermediate::GameObject& owner, const GPM::Vec3& direction,
                      const Engine::Resources::AmbiantComponent&  ambient,
                      const Engine::Resources::DiffuseComponent&  diffuse,
                      const Engine::Resources::SpecularComponent& specular);

    virtual void addToLightToUseBuffer(std::vector<Engine::Resources::LightData>& lb) noexcept override;

    void setDirection(const GPM::Vec3& newDirection) noexcept
    {
        m_direction = newDirection.normalized();
    }
};
} /*namespace Engine::Intermediate*/