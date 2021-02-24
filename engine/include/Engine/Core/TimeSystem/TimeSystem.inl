#include "Engine/Core/TimeSystem/TimeSystem.hpp"

inline constexpr double TimeSystem::getFixedDeltaTime() const noexcept
{
    return m_fixedDeltaTime;
}

inline constexpr double TimeSystem::getFixedUnscaledDeltaTime() const noexcept
{
    return m_fixedUnscaledDeltaTime;
}

inline constexpr void TimeSystem::setFixedUnscaledDeltaTime(double newFixedUnscaledDeltaTime) noexcept
{
    m_fixedUnscaledDeltaTime = newFixedUnscaledDeltaTime;
}

inline constexpr double TimeSystem::getDeltaTime() const noexcept
{
    return m_deltaTime;
}

inline constexpr double TimeSystem::getUnscaledDeltaTime() const noexcept
{
    return m_unscaledDeltaTime;
}

inline constexpr double TimeSystem::getAccumulatedTime() const noexcept
{
    return m_scaledTimeAcc;
}

inline constexpr double TimeSystem::getAccumulatedUnscaledTime() const noexcept
{
    return m_unscaledTimeAcc;
}

inline constexpr double TimeSystem::getTimeScale() const noexcept
{
    return m_timeScale;
}

inline constexpr void TimeSystem::setTimeScale(double newtimeScale) noexcept
{
    m_timeScale = newtimeScale;
    m_fixedDeltaTime = m_fixedUnscaledDeltaTime * m_timeScale;
}

void TimeSystem::addScaledTimer(double delay, std::function<void()> functionToExecute, bool isLooping) noexcept
{
    m_scaledTimerQueue.emplace(TimerTask{delay, delay + m_scaledTimeAcc, functionToExecute, isLooping});
}

void TimeSystem::addUnscaledTimer(double delay, std::function<void()> functionToExecute, bool isLooping) noexcept
{
    m_unscaledTimerQueue.emplace(TimerTask{delay, delay + m_unscaledTimeAcc, functionToExecute, isLooping});
}

inline void TimeSystem::clearScaledTimer() noexcept
{
    while (!m_scaledTimerQueue.empty())
        m_scaledTimerQueue.pop();
}

inline void TimeSystem::clearUnscaledTimer() noexcept
{
    while (!m_unscaledTimerQueue.empty())
        m_unscaledTimerQueue.pop();
}
