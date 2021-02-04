/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Resources/ResourcesManager.hpp"

namespace Engine::Resources
{
    class Scene
    {
        friend class SceneManager;

        private:
    
        protected:

        ResourcesManager<int> m_resourceManager;

        constexpr
        void loadResource(ResourcesManager<int>& resourceManager) noexcept {};
    
        public:
    
        constexpr inline
        Scene (ResourcesManager<int>& resourceManager) noexcept
        {
            loadResource(resourceManager);
        }
    
        //TODO: Scene can be create by default ?
        inline
        ~Scene () noexcept				                = delete;

        //TODO: Scene can be copiable ? How manage resource
        constexpr inline
        Scene (const Scene& other) noexcept			    = delete;
    
        //TODO: Scene can be move ? How manage resource
        constexpr inline
        Scene (Scene&& other) noexcept				    = delete;
        
        //TODO: Scene can be capiable ? How manage resource
        constexpr inline
        Scene& operator=(Scene const& other) noexcept		= delete;
    
        //TODO: Scene can be move ? How manage resource
        constexpr inline
        Scene& operator=(Scene && other) noexcept			= delete;    
    };
} /*namespace Engine::Resources*/
