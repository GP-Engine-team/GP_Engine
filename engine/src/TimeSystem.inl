#include "Engine/Core/TimeSystem/TimeSystem.hpp"

inline double Engine::Core::TimeSystem::getFixedDeltaTime() noexcept
{
    return m_fixedDetlaTime;
}

inline double Engine::Core::TimeSystem::getFixedUnscaledDeltaTime() noexcept
{
    return m_fixedUnscaledDetlaTime;
}

inline void Engine::Core::TimeSystem::setFixedUnscaledDeltaTime(double newFixedUnscaledDetlaTime) noexcept
{
    if (newFixedUnscaledDetlaTime > 1.)
        Engine::Core::Debug::Log::logWarning("New fixed unscaled detla time > 1.");

    m_fixedUnscaledDetlaTime = newFixedUnscaledDetlaTime;
}

inline double Engine::Core::TimeSystem::getDeltaTime() noexcept
{
    return m_deltaTime;
}

inline double Engine::Core::TimeSystem::getUnscaledDeltaTime() noexcept
{
    return m_unscaledDeltaTime;
}

inline double Engine::Core::TimeSystem::getAccumulateTime() noexcept
{
    return m_scaledTimeAcc;
}

inline double Engine::Core::TimeSystem::getAccumulateUnscaledTime() noexcept
{
    return m_unscaledTimeAcc;
}

inline double Engine::Core::TimeSystem::getTimeScale() noexcept
{
    return m_timeScale;
}

inline void Engine::Core::TimeSystem::setTimeScale(double newtimeScale) noexcept
{
    m_timeScale = newtimeScale;
    m_fixedDetlaTime = m_fixedUnscaledDetlaTime * m_timeScale;
}
