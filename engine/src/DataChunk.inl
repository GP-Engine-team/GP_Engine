
#include "Engine/Intermediate/DataChunk.hpp"

#include <utility> //std::swap

#include "Engine/Core/Tools/BranchPrediction.hpp"

template <typename TStoredComponent, int TSize>
constexpr inline Engine::Intermediate::DataChunk<TStoredComponent, TSize>::DataChunk() noexcept
{
    m_components.reserve(TSize / sizeof(TStoredComponent));
}

template <typename TStoredComponent, int TSize>
template <typename... Args>
TStoredComponent& Engine::Intermediate::DataChunk<TStoredComponent, TSize>::addComponent(Args&&... args) noexcept
{
    std::unique_lock lock(m_mutex);

    m_components.emplace_back(std::forward<Args>(args)...);
    return m_components.back();
}

template <typename TStoredComponent, int TSize>
void Engine::Intermediate::DataChunk<TStoredComponent, TSize>::destroyComponent(
    const TStoredComponent* componentToDestroy)
{
    std::unique_lock lock(m_mutex);

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
Engine::Intermediate::DataChunk<TStoredComponent, TSize>* Engine::Intermediate::DataChunk<TStoredComponent,
                                                                                          TSize>::getInstance() noexcept
{
    // double same if to avoid to lock mutex
    if (unlikely(m_pInstance == nullptr))
    {
        std::unique_lock lock(m_mutex);
        if (unlikely(m_pInstance == nullptr))
        {
            m_pInstance = new DataChunk<TStoredComponent, TSize>();
        }
        return m_pInstance;
    }

    std::shared_lock lock(m_mutex);
    return m_pInstance;
}