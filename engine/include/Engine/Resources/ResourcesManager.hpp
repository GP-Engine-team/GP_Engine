/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Debug/Log.hpp"
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility>

namespace GPE
{
// Technical inspired about understanding example found from stackOverFlow :
// https://stackoverflow.com/questions/27941661/generating-one-class-member-per-variadic-template-argument/53112843#53112843?newreg=f97a957ca4ca467bab0d0ce1cc6ea7b2

// Variadic template class forward declaration
template <class LType, class... RType>
class ResourcesManager;

/**
 * @brief Resources of one type. Will be created for each differente type.
 * @tparam LType 
*/
template <class LType>
class ResourcesManager<LType>
{
protected:
    std::unordered_map<std::string, LType> m_resources;

public:
    ResourcesManager() noexcept = default;
    ResourcesManager(const ResourcesManager& other) noexcept = delete;
    ResourcesManager& operator=(const ResourcesManager& other) noexcept = delete;
    ResourcesManager(ResourcesManager&& other) noexcept = default;
    ResourcesManager& operator=(ResourcesManager&& other) noexcept = default;
    ~ResourcesManager() noexcept = default;

public:
    /**
     * @brief return element of index
     *
     * @param key
     * @return LType&
     */
    LType* get(const std::string& key) noexcept;

    /**
     * @brief add element with argument of constructor in parameter and key. Resource canno't have the same key
     *
     * @tparam Args
     * @param args
     * @return unsigned int
     */
    template <typename... Args>
    LType& add(std::string key, Args&&... args) noexcept(std::is_nothrow_constructible_v<LType>);

    /**
     * @brief Remove resource with the corresponding key if key is found
     * @param key 
     * @return 
    */
    inline void remove(const std::string& key) noexcept(std::is_nothrow_destructible_v<LType>);

    /**
     * @brief clear the container and all associate resources
     * @return 
    */
    inline void clear() noexcept(std::is_nothrow_destructible_v<LType>);
};

/**
 * @brief wrap multiple specifique resource manager.
 * create a class hierarchy where each base class is a specialization for one of the types
 * For example : resourceManager<int, float, std::string> wille create :
 * resourceManager<int>, resourceManager<float> and resourceManager<std::string>.
 * resourceManager<int, float, std::string> inherited about these 3 class and can access to each element
 * @tparam LType 
 * @tparam ...RType 
*/
template <class LType, class... RType>
class ResourcesManager : public ResourcesManager<LType>, public ResourcesManager<RType...>
{
public:
    ResourcesManager() noexcept = default;
    ResourcesManager(const ResourcesManager& other) noexcept = delete;
    ResourcesManager& operator=(const ResourcesManager& other) noexcept = delete;
    ResourcesManager(ResourcesManager&& other) noexcept = default;
    ResourcesManager& operator=(ResourcesManager&& other) noexcept = default;
    ~ResourcesManager() noexcept = default;

    /**
     * @brief Return resource.
     * @example get<Texture>("AwsomeTexture")
     * @tparam T
     * @param key
     * @return T&
     */
    template <class T>
    inline T* get(const std::string& key) noexcept;

    /**
     * @brief Remove resource with the corresponding key if key is found
     * @example remove<Texture>("AwsomeTexture")
     * @tparam T 
     * @param key 
     * @return 
    */
    template <class T>
    inline void remove(const std::string& key) noexcept(std::is_nothrow_destructible_v<T>);

    /**
     * @brief Add resource.
     * @example add<Texture>("AwsomeTexture", Texture())
     * @tparam T
     * @tparam Args
     * @param args
     * @return unsigned int
     */
    template <class T, typename... Args>
    inline T& add(const std::string& key, Args&&... args) noexcept(std::is_nothrow_constructible_v<T>);

    /**
     * @brief clear the container and all associate resources
     * @example clear<Texture>()
     * @tparam T 
     * @return 
    */
    template <class T>
    inline void clear() noexcept(std::is_nothrow_destructible_v<T>);
};

#include "ResourcesManager.inl"

} // namespace GPE
