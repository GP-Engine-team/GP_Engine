namespace GPE
{
inline Component::Component(GameObject& owner) noexcept : m_gameObject{&owner}
{
    //updateToSystem();
}

constexpr inline GameObject& Component::getOwner() noexcept
{
    return *m_gameObject;
}

inline void Component::setOwner(GameObject& owner) noexcept
{
    m_gameObject = &owner;
}

constexpr inline const GameObject& Component::getOwner() const noexcept
{
    return *m_gameObject;
}

constexpr inline bool Component::isActivated() const noexcept
{
    return m_isActivated;
}

inline void Component::setActive(bool newState) noexcept
{
    if (m_isActivated == newState)
        return;

    m_isActivated = newState;

    updateToSystem();
}
} // namespace GPE