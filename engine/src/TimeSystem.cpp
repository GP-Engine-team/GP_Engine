#include "Engine/ECS/System/TimeSystem.hpp"
#include "Engine/Core/Debug/Log.hpp"

#include <string>

using namespace GPE;

void TimeSystem::update(std::function<void(double fixedUnscaledDeltaTime, double fixedDeltaTime)> fixedUpdateFunction,
                        std::function<void(double unscaledDeltaTime, double deltaTime)>           updateFunction,
                        std::function<void()> renderFunction) noexcept
{
    /*unfixed update*/
    updateFunction(m_unscaledDeltaTime, m_deltaTime);

    /*render the current frame*/
    renderFunction();

    /*Prepar the next frame*/
    m_tempTime          = std::chrono::steady_clock::now();
    m_unscaledDeltaTime = std::chrono::duration<double>(m_tempTime - m_time).count();
    m_time              = m_tempTime;

    // This is temporary
    if (m_unscaledDeltaTime > 0.25)
        m_unscaledDeltaTime = 0.25;

    if (m_unscaledDeltaTime > 0.25)
        m_unscaledDeltaTime = 0.25;

    /*Apply time scale*/
    m_deltaTime = m_unscaledDeltaTime * m_timeScale;

    /*Add accumulator*/
    m_scaledTimeAcc += m_deltaTime;
    m_unscaledTimeAcc += m_unscaledDeltaTime;

    m_scaledTimeAcc *= !isinf(m_scaledTimeAcc);     // reset if isInf (avoid conditionnal jump)
    m_unscaledTimeAcc *= !isinf(m_unscaledTimeAcc); // reset if isInf (avoid conditionnal jump)

    /*Fixed update*/
    m_fixedUnscaledTimeAcc += m_unscaledDeltaTime;

    while (m_fixedUnscaledTimeAcc >= m_fixedUnscaledDeltaTime)
    {
        fixedUpdateFunction(m_fixedUnscaledDeltaTime, m_fixedDeltaTime);
        m_fixedUnscaledTimeAcc -= m_fixedUnscaledDeltaTime;
    }

    /*Update timer queue task*/
    while (!m_unscaledTimerQueue.empty() && m_unscaledTimerQueue.top().globalTimer <= m_unscaledTimeAcc)
    {
        const TimerTask& timerTask = m_unscaledTimerQueue.top();
        timerTask.task();

        if (timerTask.isLooping)
        {
            addUnscaledTimer(timerTask.localTimer, timerTask.task, timerTask.isLooping);
        }
        m_unscaledTimerQueue.pop();
    }

    while (!m_scaledTimerQueue.empty() && m_scaledTimerQueue.top().globalTimer <= m_scaledTimeAcc)
    {
        const TimerTask& timerTask = m_scaledTimerQueue.top();
        timerTask.task();

        if (timerTask.isLooping)
        {
            addScaledTimer(timerTask.localTimer, timerTask.task, timerTask.isLooping);
        }
        m_scaledTimerQueue.pop();
    }
}