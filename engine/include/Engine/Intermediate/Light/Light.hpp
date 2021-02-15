/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>

#include "Engine/Intermediate/Component.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Camera.hpp"
#include "Engine/Resources/Color.hpp"
#include "Engine/Resources/ShaderType.hpp"
#include "GPM/Vector3.hpp"
#include "GPM/Vector4.hpp"

// TODO: Light must be a resource
namespace Engine::Intermediate
{
struct LightCreateArg
{
    const Engine::Resources::AmbiantComponent&  ambient;
    const Engine::Resources::DiffuseComponent&  diffuse;
    const Engine::Resources::SpecularComponent& specular;
};

class Light // TODO: Can be more optimize change information only when light is update
    : public Engine::Intermediate::Component
{
protected:
    Engine::Resources::AmbiantComponent  m_ambientComp;
    Engine::Resources::DiffuseComponent  m_diffuseComp;
    Engine::Resources::SpecularComponent m_specularComp;

public:
    Light(Engine::Intermediate::GameObject& owner, const LightCreateArg& arg);

    Light(Engine::Intermediate::GameObject& owner, const Engine::Resources::AmbiantComponent& ambient,
          const Engine::Resources::DiffuseComponent& diffuse, const Engine::Resources::SpecularComponent& specular);

    Light(const Light& other) = delete;
    Light(Light&& other)      = default;
    virtual ~Light();

    Light()        = delete;
    Light& operator=(Light const& other) = delete;
    Light& operator=(Light&& other) = default;

    virtual void addToLightToUseBuffer(std::vector<Engine::Resources::LightData>& lb) noexcept
    {
        lb.push_back({m_ambientComp,
                      m_diffuseComp,
                      m_specularComp,
                      getGameObject().getTransform().getGlobalPosition(),
                      0.f,
                      0.f,
                      0.f,
                      0.f,
                      0.f,
                      {0.f, 0.f, 0.f},
                      0.f});
    }

    virtual const Engine::Resources::AmbiantComponent& getAmbient() const noexcept
    {
        return m_ambientComp;
    }
    virtual const Engine::Resources::DiffuseComponent& getDiffuse() const noexcept
    {
        return m_diffuseComp;
    }
    virtual const Engine::Resources::SpecularComponent& getSpecular() const noexcept
    {
        return m_specularComp;
    }

    virtual void setGlobalComponent(const Engine::Resources::ColorRGBA& newComponent) noexcept
    {
        m_ambientComp.rgba  = newComponent;
        m_diffuseComp.rgba  = newComponent;
        m_specularComp.rgba = newComponent;
    }

    virtual void setGlobalComponent(const GPM::Vec4& newComponent) noexcept
    {
        m_ambientComp.rgbi  = newComponent;
        m_diffuseComp.rgbi  = newComponent;
        m_specularComp.rgbi = newComponent;
    }

    virtual void setAmbient(const Engine::Resources::AmbiantComponent& newAmbient) noexcept
    {
        m_ambientComp.rgba = newAmbient.rgba;
    }
    virtual void setDiffuse(const Engine::Resources::DiffuseComponent& newDiffuse) noexcept
    {
        m_diffuseComp.rgba = newDiffuse.rgba;
    }
    virtual void setSpecular(const Engine::Resources::SpecularComponent& newSpecular) noexcept
    {
        m_specularComp.rgba = newSpecular.rgba;
    }

    virtual void setAmbient(const GPM::Vec4& newAmbient) noexcept
    {
        m_ambientComp.rgbi = newAmbient;
    }
    virtual void setDiffuse(const GPM::Vec4& newDiffuse) noexcept
    {
        m_diffuseComp.rgbi = newDiffuse;
    }
    virtual void setSpecular(const GPM::Vec4& newSpecular) noexcept
    {
        m_specularComp.rgbi = newSpecular;
    }
};
} /*namespace Engine::Intermediate*/
