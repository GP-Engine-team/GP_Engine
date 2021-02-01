 /*
  * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
  * This file is subject to the LGNU license terms in the LICENSE file
  *	found in the top-level directory of this distribution.
  */
 
 #pragma once

 #include <unordered_map>
 #include <string>

 #include "Engine/Resources/Scene.hpp"
 
 namespace Engine::Resources
 {
    enum ELoadType : uint8_t
    {
        SUBSTITUTE                  = (1u << 1), //Replace previous scene by the new
        ADDITIONAL                  = (1u << 2), //Combine both scene
        KEEP_IN_MEMORY              = (1u << 3), //Usefull is you want that the previous scene load instantanitly. But reserve memory in RAM and in GPU unused
        BYPASS_RESOUCE_RECYCLING    = (1u << 4) //Usefull if you know that resource is not reused in other scene and avoid to check it
    };

    class SceneManager
    {
        private:
    
        protected:

        std::unordered_map<std::string, Scene> m_scenes;
    
        public:
    
        SceneManager () noexcept = default;
    
        SceneManager (const SceneManager& other) noexcept = default;
    
        SceneManager (SceneManager&& other) noexcept = default;
    
        ~SceneManager () noexcept = default;
    
        SceneManager& operator=(SceneManager const& other) noexcept = default;
    
        SceneManager& operator=(SceneManager && other) noexcept = default;
        
        void loadNewScene(std::string sceneID, ELoadType loadType = ELoadType::SUBSTITUTE);
    };

 } /*namespace Engine::Resources*/
 