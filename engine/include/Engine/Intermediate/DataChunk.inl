#include "Engine/Intermediate/DataChunk.hpp"

template <typename TStoredComponent, int TSize>
constexpr inline DataChunk<TStoredComponent, TSize>::DataChunk() noexcept
{
    m_components.reserve(TSize / sizeof(TStoredComponent));
}

template <typename TStoredComponent, int TSize>
template <typename... Args>
TStoredComponent& DataChunk<TStoredComponent, TSize>::addComponent(Args&&... args) noexcept
{
    return m_components.emplace_back(std::forward<Args>(args)...);
}

template <typename TStoredComponent, int TSize>
void DataChunk<TStoredComponent, TSize>::destroyComponent(const TStoredComponent* componentToDestroy) noexcept
{
    for (size_t i = 0; i < m_components.size(); ++i)
    {
        if (unlikely(&m_components[i] == componentToDestroy))
        {
            std::swap(m_components[i], m_components.back());

            m_components[i].getGameObject().updateComponentLink<TStoredComponent>(&m_components.back(),
                                                                                  &m_components[i]);
            m_components.pop_back();
        }
    }
}

template <typename TStoredComponent, int TSize>
DataChunk<TStoredComponent, TSize>* DataChunk<TStoredComponent, TSize>::getInstance() noexcept
{
    if (unlikely(m_pInstance == nullptr))
    {
        m_pInstance = new DataChunk<TStoredComponent, TSize>();
    }

    return m_pInstance;
}

template <typename TStoredComponent, int TSize>
void DataChunk<TStoredComponent, TSize>::setInstance(DataChunk& ptr) noexcept
{
    m_pInstance = &ptr;
}