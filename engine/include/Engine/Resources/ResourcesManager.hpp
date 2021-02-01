/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <unordered_map>
#include <string>
#include <utility>
#include <typeinfo>
#include "Engine/Core/Debug/Log.hpp"

namespace Engine::Resources
{
    //Technical inspired about understanding example found from stackOverFlow : https://stackoverflow.com/questions/27941661/generating-one-class-member-per-variadic-template-argument/53112843#53112843?newreg=f97a957ca4ca467bab0d0ce1cc6ea7b2

    //Variadic template class
    template<class LType, class... RType> class ResourcesManager;

    //Resources of one type. Will be created for each differente type.
    template<class LType>
    class ResourcesManager<LType>
    {
        public:

        ResourcesManager ()                                = default;
        ResourcesManager (const ResourcesManager& other)  = delete;
        ResourcesManager (ResourcesManager&& other)       = default;
        ~ResourcesManager ()                               = default;

        protected:

        /**
         * @brief return element of index
         * 
         * @param key 
         * @return LType& 
         */
        LType* get(const std::string& key)
        {    
            //iterator of LType
            auto it = resource_.find(key);
            if (it == resource_.end())
            {
                return nullptr; 
            }

            return &it->second;
        }

        /**
         * @brief add element with argument of constructor in parameter
         * 
         * @tparam Args 
         * @param args 
         * @return unsigned int 
         */
        template<typename ...Args>
        LType& add(std::string key, Args&&... args)
        {
            //auto for pair of iterator of LType and bool
            auto rst = resource_.emplace(  std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(args...));

            if (rst.second == false)
            {
                Engine::Core::Debug::SLog::logError(std::string("resource insert with same key as an element existing : ") + key + ". Resource type : " + typeid(LType).name());
                exit(1);
            }

            return rst.first->second;
        }

        void remove(const std::string& key)
        {
            resource_.erase(key);
        }

        void clearResource  () noexcept
        {
            resource_.clear();
        }

        private:

        std::unordered_map <std::string, LType> resource_;

    };

    //Global resource manager.
    //create a class hierarchy where each base class is a specialization for one of the types
    //For example : resourceManager<int, float, std::string> wille create :
    //resourceManager<int>, resourceManager<float> and resourceManager<std::string>.
    //resourceManager<int, float, std::string> inherited about these 3 class and can access to each element
    
    template<class LType, class... RType>
    class ResourcesManager
        : public ResourcesManager<LType>, public ResourcesManager<RType...>
    {
        public:

            #pragma region constructor/destructor

            ResourcesManager ()                                = default;
            ResourcesManager (const ResourcesManager& other)  = delete;
            ResourcesManager (ResourcesManager&& other)       = default;
            ~ResourcesManager ()                               = default;

            #pragma endregion //!constructor/destructor

            #pragma region methods
            #pragma endregion //!methods

            #pragma region accessor

            /**
             * @brief Return resource. Example : get<Texture>(0)
             * 
             * @tparam T 
             * @param key 
             * @return T& 
             */
            template<class T>
            T* get(const std::string& key)
            {
                return ResourcesManager<T>::get(key);
            }

            #pragma endregion //!accessor

            template<class T>
            void remove(const std::string& key)
            {
                ResourcesManager<T>::remove(key);
            }

            #pragma region mutator

            /**
             * @brief Add resource. Example : add<Texture>(Texture())
             * 
             * @tparam T 
             * @tparam Args 
             * @param args 
             * @return unsigned int 
             */
            template<class T, typename... Args>
            T& add(const std::string& key, Args&&... args)
            {
                return ResourcesManager<T>::add(key, std::forward<Args>(args)...);
            }

            template<class T>
            void clearContenorOf  () noexcept
            {
                ResourcesManager<T>::clearResource();
            }

            #pragma endregion //!mutator

        protected:

        private:

    };

    template<class LType, class... RType>
    class ResourcesManagerWithGlobalUsage 
        : public ResourcesManager<LType, RType...>
    {
        public:

            #pragma region constructor/destructor

            ResourcesManagerWithGlobalUsage ()                                = default;
            ResourcesManagerWithGlobalUsage (const ResourcesManagerWithGlobalUsage& other)  = delete;
            ResourcesManagerWithGlobalUsage (ResourcesManagerWithGlobalUsage&& other)       = default;
            ~ResourcesManagerWithGlobalUsage ()                               = default;

            #pragma endregion //!constructor/destructor

            #pragma region methods

            void use    () noexcept;

            #pragma endregion //!methods

            static ResourcesManagerWithGlobalUsage<LType, RType...>* getResourceManagerUse() noexcept;

        protected:

        static ResourcesManagerWithGlobalUsage<LType, RType...>* resourceManagerToUse; //pointor to be in nullptr by default

        private:

    };

    template<class LType, class... RType>
    ResourcesManagerWithGlobalUsage<LType, RType...>* ResourcesManagerWithGlobalUsage<LType, RType...>::resourceManagerToUse {nullptr};

    template<class LType, class... RType>
    ResourcesManagerWithGlobalUsage<LType, RType...>* ResourcesManagerWithGlobalUsage<LType, RType...>::getResourceManagerUse() noexcept
    {
        return resourceManagerToUse; 
    }

    template<class LType, class... RType>
    void ResourcesManagerWithGlobalUsage<LType, RType...>::use () noexcept
    {
        resourceManagerToUse = this;
    }
}// namespaceEngine::Resources