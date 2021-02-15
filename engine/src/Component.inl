#include "Engine/Intermediate/Component.hpp"

inline Engine::Intermediate::Component::Component(GameObject& refGameObject) noexcept : m_gameObject{refGameObject}
{
}

constexpr inline Engine::Intermediate::GameObject& Engine::Intermediate::Component::getGameObject() noexcept
{
    return m_gameObject;
}

constexpr inline const Engine::Intermediate::GameObject& Engine::Intermediate::Component::getGameObject() const noexcept
{
    return m_gameObject;
}

constexpr inline bool Engine::Intermediate::Component::isActivated() const noexcept
{
    return m_isActivated;
}

constexpr inline void Engine::Intermediate::Component::setActive(bool newState) noexcept
{
    m_isActivated = newState;
}