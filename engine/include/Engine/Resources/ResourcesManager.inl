#include "Engine/Resources/ResourcesManager.hpp"

template <class LType>
LType* ResourcesManager<LType>::get(const std::string& key) noexcept
{
    // iterator of LType
    auto it = m_resources.find(key);
    if (it == m_resources.end())
    {
        return nullptr;
    }

    return &it->second;
}

template <class LType>
const LType* ResourcesManager<LType>::get(const std::string& key) const noexcept
{
    // iterator of LType
    auto it = m_resources.find(key);
    if (it == m_resources.end())
    {
        Log::getInstance()->logWarning(stringFormat("Resource insert with key \"%s\" doesn't exist", key.c_str()));
        return nullptr;
    }

    return &it->second;
}

template <class LType>
std::unordered_map<std::string, LType>& ResourcesManager<LType>::getAll() noexcept
{
    return m_resources;
}

template <class LType>
const std::string* ResourcesManager<LType>::getKey(const LType* data) const noexcept
{
    for (auto&& res : m_resources)
    {
        if (&res.second == data)
            return &res.first;
    }
    return nullptr;
}

template <class LType>
template <typename... Args>
LType& ResourcesManager<LType>::add(const std::string& key,
                                    Args&&... args) noexcept(std::is_nothrow_constructible_v<LType>)
{
    auto rst = m_resources.try_emplace(key, std::forward<Args>(args)...);
    return rst.first->second;
}

template <class LType>
void ResourcesManager<LType>::remove(const std::string& key) noexcept(std::is_nothrow_destructible_v<LType>)
{
    m_resources.erase(key);
}

template <class LType>
void ResourcesManager<LType>::clear() noexcept(std::is_nothrow_destructible_v<LType>)
{
    m_resources.clear();
}

template <class LType, class... RType>
template <class T>
T* ResourcesManager<LType, RType...>::get(const std::string& key) noexcept
{
    return ResourcesManager<T>::get(key);
}

template <class LType, class... RType>
template <class T>
const T* ResourcesManager<LType, RType...>::get(const std::string& key) const noexcept
{
    return ResourcesManager<T>::get(key);
}

template <class LType, class... RType>
template <class T>
std::unordered_map<std::string, T>& ResourcesManager<LType, RType...>::getAll() noexcept
{
    return ResourcesManager<T>::getAll();
}

template <class LType, class... RType>
template <class T>
const std::string* ResourcesManager<LType, RType...>::getKey(const T* data) const noexcept
{
    return ResourcesManager<T>::getKey(data);
}

template <class LType, class... RType>
template <class T>
void ResourcesManager<LType, RType...>::remove(const std::string& key) noexcept(std::is_nothrow_destructible_v<T>)
{
    ResourcesManager<T>::remove(key);
}

template <class LType, class... RType>
template <class T, typename... Args>
T& ResourcesManager<LType, RType...>::add(const std::string& key,
                                          Args&&... args) noexcept(std::is_nothrow_constructible_v<T>)
{
    return ResourcesManager<T>::add(key, std::forward<Args>(args)...);
}

template <class LType, class... RType>
template <class T>
void ResourcesManager<LType, RType...>::clear() noexcept(std::is_nothrow_destructible_v<T>)
{
    ResourcesManager<T>::clear();
}

template <class LType, class... RType>
void ResourcesManager<LType, RType...>::clearAll() noexcept
{
    ResourcesManager<LType>::clear();

    // Recursivity except for the last temaplte element
    if constexpr (sizeof...(RType) > 1)
    {
        ResourcesManager<RType...>::clearAll();
    }
}