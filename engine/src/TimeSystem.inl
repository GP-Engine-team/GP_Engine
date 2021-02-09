#include "Engine/Core/TimeSystem/TimeSystem.hpp"

inline double Engine::Core::TimeSystem::getFixedDeltaTime() noexcept
{
    return m_fixedDeltaTime;
}

inline double Engine::Core::TimeSystem::getFixedUnscaledDeltaTime() noexcept
{
    return m_fixedUnscaledDeltaTime;
}

inline constexpr void Engine::Core::TimeSystem::setFixedUnscaledDeltaTime(double newFixedUnscaledDeltaTime) noexcept
{
    if (newFixedUnscaledDeltaTime > 1.)
        Engine::Core::Debug::Log::logWarning("New fixed unscaled delta time > 1.");

    m_fixedUnscaledDeltaTime = newFixedUnscaledDeltaTime;
}

inline constexpr double Engine::Core::TimeSystem::getDeltaTime() const noexcept
{
    return m_deltaTime;
}

inline constexpr double Engine::Core::TimeSystem::getUnscaledDeltaTime() const noexcept
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

inline constexpr double Engine::Core::TimeSystem::getTimeScale() const noexcept
{
    return m_timeScale;
}

inline constexpr void Engine::Core::TimeSystem::setTimeScale(double newtimeScale) noexcept
{
    m_timeScale = newtimeScale;
    m_fixedDeltaTime = m_fixedUnscaledDeltaTime * m_timeScale;
}

void Engine::Core::TimeSystem::addScaledTimer(double delay, std::function<void()> functionToExecute, bool isLooping) noexcept
{
    m_scaledTimerQueue.emplace(TimerTask{delay, delay + m_scaledTimeAcc, functionToExecute, isLooping});
}

void Engine::Core::TimeSystem::addUnscaledTimer(double delay, std::function<void()> functionToExecute, bool isLooping) noexcept
{
    m_unscaledTimerQueue.emplace(TimerTask{delay, delay + m_unscaledTimeAcc, functionToExecute, isLooping});
}

inline void Engine::Core::TimeSystem::clearScaledTimer() noexcept
{
    while (!m_scaledTimerQueue.empty())
        m_scaledTimerQueue.pop();
}

inline void Engine::Core::TimeSystem::clearUnscaledTimer() noexcept
{
    while (!m_unscaledTimerQueue.empty())
        m_unscaledTimerQueue.pop();
}
