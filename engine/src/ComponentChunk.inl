
#include "Engine/Intermediate/ComponentChunk.hpp"

template<typename TStoredComponent>
constexpr inline
Engine::Intermediate::ComponentChunk<TStoredComponent>::ComponentChunk () noexcept
{
    //64KiB = 65,536Ko
    m_component.reserve(65536 / sizeof(TStoredComponent))
}

template<typename TStoredComponent>
template <typename... Args>
TStoredComponent& Engine::Intermediate::ComponentChunk<TStoredComponent>::addComponent(Args&&... args) noexcept
{
    std::unique_lock lock(m_mutex);

    m_component.emplace_back(args);
    return m_component.back();
}

/**
 * The first time we call GetInstance we will lock the storage location
 *      and then we make sure again that the variable is null and then we
 *      set the value.
 */
template<typename TStoredComponent>
Engine::Intermediate::ComponentChunk<TStoredComponent> *Engine::Intermediate::ComponentChunk<TStoredComponent>::getInstance(const std::string& value) noexcept
{
    //double same if to avoid to lock mutex
    if (m_pInstance == nullptr) [[unlikely]]
    {
        std::unique_lock lock(m_mutex);
        if (m_pInstance == nullptr) [[unlikely]]
        {
            m_pInstance = new ComponentChunk<TStoredComponent>(value);
        }
        return m_pInstance;
    }

    std::shared_lock lock(m_mutex);
    return m_pInstance;
}