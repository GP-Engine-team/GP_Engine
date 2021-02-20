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
    GPE_ASSERT((componentToDestroy - m_components.data()) <= (m_components.size() - 1u) * sizeof(TStoredComponent),
               "Pointer out of range");

    // Avoid for each. Pointer allow to access
    const TStoredComponent& back = m_components.back();
    back->getGameObject().updateComponentLink<TStoredComponent>(&back, componentToDestroy);
    std::swap(*componentToDestroy, back);
    m_components.pop_back();
}

template <typename TStoredComponent, int TSize>
DataChunk<TStoredComponent, TSize>* DataChunk<TStoredComponent, TSize>::getInstance() noexcept
{
    // double same if to avoid to lock mutex
    if (unlikely(m_pInstance == nullptr))
    {
        m_pInstance = new DataChunk<TStoredComponent, TSize>();
    }

    return m_pInstance;
}
