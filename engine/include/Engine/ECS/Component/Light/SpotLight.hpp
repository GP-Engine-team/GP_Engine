/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>

#include "Engine/Core/Tools/ClassUtility.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/ECS/Component/Light/PointLight.hpp"
#include "GPM/Vector3.hpp"

namespace GPE
{
class SpotLight : public PointLight
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

        float cutOff;
        float cutOffExponent;
    };

protected:
    float m_cutOff; // specifies the spotlight's radius.
    float m_cutOffExponent;

public:
    SpotLight(const SpotLight& other) = delete;
    SpotLight(SpotLight&& other)      = default;
    virtual ~SpotLight()              = default;

    SpotLight()        = delete;
    SpotLight& operator=(SpotLight const& other) = delete;
    SpotLight& operator=(SpotLight&& other) = default;

    /**
     * @brief Construct a new Spot Light object
     *
     * @param pos
     * @param ambient
     * @param diffuse
     * @param specular
     * @param constant
     * @param linear
     * @param quadratic
     * @param direction
     * @param cutOff            : in degres : specifies the spotlight's radius
     * @param cutOffExponent    : in degres : specifies the spotlight's radius attenuation
     * @param name
     */
    SpotLight(GameObject& owner, const AmbiantComponent& ambient, const DiffuseComponent& diffuse,
              const SpecularComponent& specular, float constant, float linear, float quadratic, float cutOff,
              float cutOffExponent);

    SpotLight(GameObject& owner, CreateArg arg);

    void addToLightToUseBuffer(std::vector<LightData>& lb) noexcept final;

    DEFAULT_GETTER_SETTER_BY_REF(CutOff, m_cutOff);
    DEFAULT_GETTER_SETTER_BY_REF(CutOffExponent, m_cutOffExponent);
};
} /*namespace GPE*/
