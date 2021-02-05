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