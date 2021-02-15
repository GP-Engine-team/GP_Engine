/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>

#include "GPM/Vector3.hpp"
#include "Engine/Intermediate/Light/PointLight.hpp"
#include "Engine/Intermediate/GameObject.hpp"

namespace Engine::Intermediate
{
    struct SpotLightCreateArg
    {
        const Engine::Resources::AmbiantComponent&             ambient; 
        const Engine::Resources::DiffuseComponent&             diffuse;
        const Engine::Resources::SpecularComponent&            specular;

        float                                                   constant; 
        float                                                   linear;
        float                                                   quadratic;

        float                                                   cutOff;
        float                                                   cutOffExponent;
        bool                                                    isEnable {true};

    };

    class SpotLight
        : public PointLight
    {
        protected:
    
            float m_cutOff;              //specifies the spotlight's radius.
            float m_cutOffExponent;

        public:

            SpotLight ()						= default;

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
            SpotLight(Engine::Intermediate::GameObject& owner,
                        const Engine::Resources::AmbiantComponent&            ambient, 
                        const Engine::Resources::DiffuseComponent&            diffuse, 
                        const Engine::Resources::SpecularComponent&           specular,
                        float                                                  constant, 
                        float                                                  linear, 
                        float                                                  quadratic,
                        float                                                  cutOff,
                        float                                                  cutOffExponent,
                        bool                                                   isEnable = true);

            SpotLight(Engine::Intermediate::GameObject& owner, SpotLightCreateArg arg);
            
            SpotLight (const SpotLight& other)		= default;
            SpotLight (SpotLight&& other)			= default;
            virtual ~SpotLight ()				    = default;

            virtual void addToLightToUseBuffer(std::vector<Engine::Resources::LightData>& lb) noexcept override;   
    };
} /*namespace Engine::Intermediate*/