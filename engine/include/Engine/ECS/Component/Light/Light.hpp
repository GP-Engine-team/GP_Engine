﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>

#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Color.hpp"
#include "Engine/Resources/ShaderType.hpp"
#include "Engine/Resources/Type.hpp"
#include "GPM/Vector3.hpp"
#include "GPM/Vector4.hpp"

// in inl
#include "Engine/Resources/Scene.hpp"

// Generated
#include "Generated/Light.rfk.h"

namespace GPE RFKNamespace()
{
    // TODO: Can be more optimize change information only when light is update
    class RFKClass(Serialize(), ComponentGen) Light : public Component
    {
    public:
        struct CreateArg
        {
            AmbiantComponent  ambient  = AmbiantComponent{0.f, 0.f, 0.f, 1.f};
            DiffuseComponent  diffuse  = DiffuseComponent{0.5f, 0.5f, 0.5f, 1.f};
            SpecularComponent specular = SpecularComponent{0.5f, 0.5f, 0.5f, 1.f};
        };

        struct RFKStruct(Serialize()) ShadowProperties
        {
            RFKField(Serialize()) bool         isEnable             = false;
            RFKField(Serialize()) float        shadowMapSampleScale = 4.f;
            RFKField(Serialize()) unsigned int PCF                  = 3;
            RFKField(Serialize()) float        bias                 = 0.05f;
            RFKField(Serialize()) float        near                 = 0.1f;
            RFKField(Serialize()) float        far                  = 1000.f;
            RFKField(Serialize()) float        size                 = 1000.f;

            ShadowProperties_GENERATED
        };

    protected:
        RFKField(Serialize()) AmbiantComponent  m_ambientComp;
        RFKField(Serialize()) DiffuseComponent  m_diffuseComp;
        RFKField(Serialize()) SpecularComponent m_specularComp;

        RFKField(Serialize()) ShadowProperties m_shadowProperties;

        virtual void updateToSystem() noexcept override;

    public:
        inline Light(GameObject & owner, const CreateArg& arg);

        inline Light(GameObject & owner, const AmbiantComponent& ambient, const DiffuseComponent& diffuse,
                     const SpecularComponent& specular);

        inline virtual ~Light();

        Light() = default;

        void moveTowardScene(Scene & newOwner) final
        {
            getOwner().pOwnerScene->sceneRenderer.removeLight(*this);
            newOwner.sceneRenderer.addLight(*this);
        }

        virtual void addToLightToUseBuffer(std::vector<LightData> & lb) noexcept
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
        inline const ShadowProperties&  getShadowProperties() const noexcept;

        inline void setGlobalComponent(const ColorRGBA& newComponent) noexcept;
        inline void setGlobalComponent(const GPM::Vec4& newComponent) noexcept;

        inline void setAmbient(const AmbiantComponent& newAmbient) noexcept;
        inline void setDiffuse(const DiffuseComponent& newDiffuse) noexcept;
        inline void setSpecular(const SpecularComponent& newSpecular) noexcept;

        inline void setAmbient(const GPM::Vec4& newAmbient) noexcept;
        inline void setDiffuse(const GPM::Vec4& newDiffuse) noexcept;
        inline void setSpecular(const GPM::Vec4& newSpecular) noexcept;

        virtual GPM::Mat4 getLightSpaceMatrix() noexcept;

        virtual void inspect(InspectContext & context);

        void setShadowActive(bool newState) noexcept;

        Light_GENERATED
    };
} // namespace )

#include "Light.inl"