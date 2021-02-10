#include "Engine/Intermediate/Component.hpp"

    inline
    Engine::Intermediate::Component::Component(GameObject& refGameObject) noexcept : m_gameObject{refGameObject}, m_name{__FUNCTION__}
    {}

    Engine::Intermediate::GameObject& Engine::Intermediate::Component::getGameObject() noexcept
    {
        return m_gameObject;
    }

    const Engine::Intermediate::GameObject& Engine::Intermediate::Component::getGameObject() const noexcept
    {
        return m_gameObject;
    }

    const std::string& Engine::Intermediate::Component::toString() const noexcept
    {
        return m_name;
    };

    bool Engine::Intermediate::Component::isActivated() const noexcept
    {
        return m_isActivated;
    }

    void Engine::Intermediate::Component::setActive(bool newState) noexcept
    {
        m_isActivated = newState;
    }