/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/Component.hpp"
#include "Engine/Intermediate/GameObject.hpp"

namespace Engine::Resources
{
    class IModel : public Engine::Intermediate::Component
    {
        protected : 

            bool m_isLoadInGPU = false;

        public:
    
            IModel(Engine::Intermediate::GameObject& refGameObject)
                : Engine::Intermediate::Component(refGameObject), 
                m_isLoadInGPU                            {false}
            {}

            IModel(const IModel &other)                             = default;
            virtual ~IModel()                                       = default;
            IModel()                                                = delete;
            IModel &operator=(IModel const &other)                  = delete;
            IModel &operator=(IModel &&other)                       = delete;

            /**
             * @brief Draw element only if it is load in GPU
             * 
             */
            virtual void draw () const noexcept = 0;

            /**
             * @brief Load texture and Mesh from CPU to GPU. This operation can be slow.
             * 
             */            
            virtual void loadInGPU()      noexcept  = 0;
            virtual void unloadFromGPU()  noexcept  = 0;

            /**
             * @brief return true if Texture is load in GPU and ready to use
             * 
             * @return true 
             * @return false 
             */
            bool isLoadInGPU () const noexcept { return m_isLoadInGPU; }
    };
} /*namespace Engine\Resources*/