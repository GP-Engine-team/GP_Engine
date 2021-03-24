/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>

#include "Engine/Core/Tools/ClassUtility.hpp"
#include "Engine/ECS/Component/Light/Light.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/ShaderType.hpp"
#include "GPM/Vector3.hpp"

// in inl
#include <string>

// Generated
#include "Generated/DirectionalLight.rfk.h"

namespace GPE RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen()) DirectionalLight : public Light
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
        DirectionalLight(DirectionalLight && other)     = default;
        virtual ~DirectionalLight();

        DirectionalLight()        = delete;
        DirectionalLight& operator=(DirectionalLight const& other) = delete;
        DirectionalLight& operator=(DirectionalLight&& other) = default;

        DirectionalLight(GameObject & owner, const CreateArg& arg) noexcept;

        DirectionalLight(GameObject & owner, const GPM::Vec3& direction, const AmbiantComponent& ambient,
                         const DiffuseComponent& diffuse, const SpecularComponent& specular) noexcept;

        void addToLightToUseBuffer(std::vector<LightData> & lb) noexcept final;

        DEFAULT_GETTER_SETTER_BY_REF(Direction, m_direction);

        DirectionalLight_GENERATED
    };

} // namespace )

File_GENERATED
