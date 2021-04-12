/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <vector> //std::vector

#include "Engine/Core/Tools/ClassUtility.hpp"
#include "Engine/ECS/Component/Light/Light.hpp"

// Generated
#include "Generated/PointLight.rfk.h"

namespace GPE RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen) PointLight : public Light
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
            bool  isEnable{true};
        };

    protected:
        float m_constant, m_linear, m_quadratic;

    public:
        PointLight(const PointLight& other) = delete;
        PointLight(PointLight && other)     = default;
        virtual ~PointLight();

        PointLight()        = delete;
        PointLight& operator=(PointLight const& other) = delete;
        PointLight& operator=(PointLight&& other) = default;

        PointLight(class GameObject & owner) noexcept;

        PointLight(class GameObject & owner, const CreateArg& arg) noexcept;

        PointLight(class GameObject & owner, const AmbiantComponent& ambient, const DiffuseComponent& diffuse,
                   const SpecularComponent& specular, float constant, float linear, float quadratic) noexcept;

        void addToLightToUseBuffer(std::vector<LightData> & lb) noexcept override;

        DEFAULT_GETTER_SETTER_BY_REF(Constant, m_constant);
        DEFAULT_GETTER_SETTER_BY_REF(Linear, m_linear);
        DEFAULT_GETTER_SETTER_BY_REF(Quadratic, m_quadratic);

        PointLight_GENERATED
    };

} // namespace )

File_GENERATED
