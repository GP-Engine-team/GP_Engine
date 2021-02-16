#include "Engine/Intermediate/Component.hpp"

inline Component::Component(GameObject& refGameObject) noexcept : m_gameObject{refGameObject}
{
}

constexpr inline GameObject& Component::getGameObject() noexcept
{
    return m_gameObject;
}

constexpr inline const GameObject& Component::getGameObject() const noexcept
{
    return m_gameObject;
}

constexpr inline bool Component::isActivated() const noexcept
{
    return m_isActivated;
}

constexpr inline void Component::setActive(bool newState) noexcept
{
    m_isActivated = newState;
}