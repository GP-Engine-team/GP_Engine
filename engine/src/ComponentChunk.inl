
#include "Engine/Intermediate/ComponentChunk.hpp"
#include <utility> //std::swap

template <typename TStoredComponent, int TSize>
constexpr inline Engine::Intermediate::ComponentChunk<TStoredComponent, TSize>::ComponentChunk() noexcept
{
    m_components.reserve(TSize / sizeof(TStoredComponent));
}

template <typename TStoredComponent, int TSize>
template <typename... Args>
TStoredComponent& Engine::Intermediate::ComponentChunk<TStoredComponent, TSize>::addComponent(Args&&... args) noexcept
{
    std::unique_lock lock(m_mutex);

    m_components.emplace_back(args...);
    return m_components.back();
}

template <typename TStoredComponent, int TSize>
void Engine::Intermediate::ComponentChunk<TStoredComponent, TSize>::destroyComponent(
    const TStoredComponent* componentToDestroy)
{
    std::unique_lock lock(m_mutex);

    for (size_t i = 0; i < m_components.size(); ++i)
    {
        if (&m_components[i] == componentToDestroy)
            [[unlikely]]
            {
                std::swap(m_components[i], m_components.back());

                m_components[i].getGameObject().updateComponentLink<TStoredComponent>(&m_components.back(), &m_components[i]);
                m_components.pop_back();
            }
    }
}

template <typename TStoredComponent, int TSize>
Engine::Intermediate::ComponentChunk<TStoredComponent, TSize>* Engine::Intermediate::ComponentChunk<
    TStoredComponent, TSize>::getInstance() noexcept
{
    // double same if to avoid to lock mutex
    if (m_pInstance == nullptr)
        [[unlikely]]
        {
            std::unique_lock lock(m_mutex);
            if (m_pInstance == nullptr)
                [[unlikely]]
                {
                    m_pInstance = new ComponentChunk<TStoredComponent, TSize>();
                }
            return m_pInstance;
        }

    std::shared_lock lock(m_mutex);
    return m_pInstance;
}