#include "Engine/ECS/Component/Component.hpp"

inline Component::Component(GameObject& owner) noexcept : m_gameObject{&owner}
{
}

constexpr inline GameObject& Component::getOwner() noexcept
{
    return *m_gameObject;
}

constexpr inline const GameObject& Component::getOwner() const noexcept
{
    return *m_gameObject;
}

constexpr inline bool Component::isActivated() const noexcept
{
    return m_isActivated;
}

constexpr inline void Component::setActive(bool newState) noexcept
{
    m_isActivated = newState;
}