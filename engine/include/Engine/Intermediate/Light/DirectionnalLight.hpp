/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>

#include "GPM/Vector3.hpp"
#include "Engine/Intermediate/Light/Light.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/ShaderType.hpp"

namespace Engine::Intermediate
{
    struct DirectionnalLightCreateArg
    {
        const GPM::Vec3&                        direction;
        const Engine::Resources::AmbiantComponent&             ambient; 
        const Engine::Resources::DiffuseComponent&             diffuse;
        const Engine::Resources::SpecularComponent&            specular;
        bool isEnable                                           {true};

    };

    class DirectionnalLight
        : public Light
    {
        protected:

            GPM::Vec3 m_direction;

        public:
    
            DirectionnalLight ()						            = default;

            DirectionnalLight (Engine::Intermediate::GameObject & owner, const DirectionnalLightCreateArg& arg);

                        
            DirectionnalLight ( Engine::Intermediate::GameObject &                       owner,
                                const GPM::Vec3&                       direction,
                                const Engine::Resources::AmbiantComponent&            ambient, 
                                const Engine::Resources::DiffuseComponent&            diffuse, 
                                const Engine::Resources::SpecularComponent&           specular,
                                bool isEnable = true);

            DirectionnalLight (const DirectionnalLight& other)		    = default;
            DirectionnalLight (DirectionnalLight&& other)			    = default;
            virtual ~DirectionnalLight ()				                = default;
    
            virtual void addToLightToUseBuffer(std::vector<Engine::Resources::LightData>& lb) noexcept override;

            void setDirection( const  GPM::Vec3& newDirection) noexcept { m_direction = newDirection.normalized();}    
    };
} /*namespace Engine::Intermediate*/