/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>

#include "Engine/Core/Tools/ClassUtility.hpp"
#include "Engine/ECS/Component/Light/PointLight.hpp"

// Generated
#include "Generated/SpotLight.rfk.h"

namespace GPE RFKNamespace()
{
    class RFKClass(Serialize(), Inspect(), ComponentGen) SpotLight : public PointLight
    {
    public:
        struct CreateArg
        {
            AmbiantComponent  ambient  = AmbiantComponent{0.f, 0.f, 0.f, 1.f};
            DiffuseComponent  diffuse  = DiffuseComponent{0.5f, 0.5f, 0.5f, 1.f};
            SpecularComponent specular = SpecularComponent{0.5f, 0.5f, 0.5f, 1.f};

            float constant  = 1.f;
            float linear    = 0.09f;
            float quadratic = 0.032f;

            float cutOff         = 45.f; // in degres
            float cutOffExponent = 50.f; // in degres;
        };

    protected:
        RFKField(Inspect(), Serialize()) float m_cutOff; // specifies the spotlight's radius.
        RFKField(Inspect(), Serialize()) float m_cutOffExponent;

    public:
        SpotLight(const SpotLight& other) = delete;
        SpotLight(SpotLight && other)     = default;
        virtual ~SpotLight();

        SpotLight()        = default;
        SpotLight& operator=(SpotLight const& other) = delete;
        SpotLight& operator=(SpotLight&& other) = default;

        SpotLight(class GameObject & owner) noexcept;

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
        SpotLight(class GameObject & owner, const AmbiantComponent& ambient, const DiffuseComponent& diffuse,
                  const SpecularComponent& specular, float constant, float linear, float quadratic, float cutOff,
                  float cutOffExponent);

        SpotLight(class GameObject & owner, const CreateArg& arg);

        void addToLightToUseBuffer(std::vector<LightData> & lb) noexcept final;

        DEFAULT_GETTER_SETTER_BY_REF(CutOff, m_cutOff);
        DEFAULT_GETTER_SETTER_BY_REF(CutOffExponent, m_cutOffExponent);

        SpotLight_GENERATED
    };

} // namespace )

File_GENERATED