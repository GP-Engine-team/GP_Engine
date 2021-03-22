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
        return nullptr;
    }

    return &it->second;
}

template <class LType>
template <typename... Args>
LType& ResourcesManager<LType>::add(std::string key, Args&&... args) noexcept(std::is_nothrow_constructible_v<LType>)
{
    // auto for pair of iterator of LType and bool

    LType* ptr = get(key);

    if (ptr)
    {
        Log::getInstance()->logWarning(std::string("resource insert with same key as an element existing : ") + key +
                                       ". Resource type : " + typeid(LType).name());
        return *ptr;
    }
    else
    {
        auto rst =
            m_resources.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(args...));

        return rst.first->second;
    }
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
