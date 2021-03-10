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
#include "Engine/Resources/Type.hpp"
#include "GPM/Vector3.hpp"
#include "GPM/Vector4.hpp"

// in inl
#include "Engine/Resources/Scene.hpp"

namespace GPE
{
class Light // TODO: Can be more optimize change information only when light is update
    : public Component
{
public:
    struct CreateArg
    {
        const AmbiantComponent&  ambient;
        const DiffuseComponent&  diffuse;
        const SpecularComponent& specular;
    };

protected:
    AmbiantComponent  m_ambientComp;
    DiffuseComponent  m_diffuseComp;
    SpecularComponent m_specularComp;

public:
    inline Light(GameObject& owner, const CreateArg& arg);

    inline Light(GameObject& owner, const AmbiantComponent& ambient, const DiffuseComponent& diffuse,
                 const SpecularComponent& specular);

    Light(const Light& other) = delete;
    Light(Light&& other)      = default;
    inline virtual ~Light();

    Light()        = delete;
    Light& operator=(Light const& other) = delete;
    Light& operator=(Light&& other) = default;

    virtual void addToLightToUseBuffer(std::vector<LightData>& lb) noexcept
    {
        lb.push_back({m_ambientComp,
                      m_diffuseComp,
                      m_specularComp,
                      getOwner().getTransform().getGlobalPosition(),
                      0.f,
                      0.f,
                      0.f,
                      0.f,
                      0.f,
                      {0.f, 0.f, 0.f},
                      0.f});
    }

    inline const AmbiantComponent&  getAmbient() const noexcept;
    inline const DiffuseComponent&  getDiffuse() const noexcept;
    inline const SpecularComponent& getSpecular() const noexcept;

    inline void setGlobalComponent(const ColorRGBA& newComponent) noexcept;
    inline void setGlobalComponent(const GPM::Vec4& newComponent) noexcept;

    inline void setAmbient(const AmbiantComponent& newAmbient) noexcept;
    inline void setDiffuse(const DiffuseComponent& newDiffuse) noexcept;
    inline void setSpecular(const SpecularComponent& newSpecular) noexcept;

    inline void setAmbient(const GPM::Vec4& newAmbient) noexcept;
    inline void setDiffuse(const GPM::Vec4& newDiffuse) noexcept;
    inline void setSpecular(const GPM::Vec4& newSpecular) noexcept;
};

#include "Light.inl"

} /*namespace GPE*/
