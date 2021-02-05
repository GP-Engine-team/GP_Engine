#include "Engine/Core/TimeSystem/TimeSystem.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include <string>
#include <thread> // std::this_thread::sleep_for

using namespace Engine::Core;
using namespace Engine::Core::Debug;

void TimeSystem::update(std::function<void(double fixedUnscaledDetlaTime, double fixedDetlaTim)> fixedUpdateFunction,
                        std::function<void(double unscaledDeltaTime, double deltaTime)> updateFunction,
                        std::function<void()> renderFunction) noexcept
{
    /*First render the current frame*/
    renderFunction();

    /*Second, prepar the next frame*/
    m_tempTime = std::chrono::system_clock::now();
    m_unscaledDeltaTime = std::chrono::duration<double>(m_tempTime - m_time).count();
    m_time = m_tempTime;

    /*Apply time scale*/
    m_deltaTime = m_unscaledDeltaTime * m_timeScale;

    /*Add accumulator*/
    m_scaledTimeAcc += m_deltaTime;
    m_unscaledTimeAcc += m_unscaledDeltaTime;

    m_scaledTimeAcc *= !isinf(m_scaledTimeAcc);     // reset if isInf (avoid conditionnal jump)
    m_unscaledTimeAcc *= !isinf(m_unscaledTimeAcc); // reset if isInf (avoid conditionnal jump)

    /*Fixed update*/
    m_fixedTimeAcc += m_unscaledDeltaTime;

    while (m_fixedTimeAcc >= m_fixedUnscaledDetlaTime)
    {
        fixedUpdateFunction(m_fixedUnscaledDetlaTime, m_fixedDetlaTime);
        m_fixedTimeAcc -= m_fixedUnscaledDetlaTime;
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

    /*unfixed update*/
    updateFunction(m_unscaledDeltaTime, m_deltaTime);
}