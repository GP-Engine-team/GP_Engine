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

    m_timeAcc += m_unscaledDeltaTime;

    /*Apply time scale*/
    m_deltaTime = m_unscaledDeltaTime * m_timeScale;

    /*Add accumulator*/
    m_scaledTimeAcc += m_deltaTime;
    m_unscaledTimeAcc += m_unscaledDeltaTime;

    if (isinf(m_scaledTimeAcc))
    {
        m_scaledTimeAcc = 0;
    }

    if (isinf(m_unscaledTimeAcc))
    {
        m_unscaledTimeAcc = 0;
    }

    /*Fixed update*/
    while (m_timeAcc >= m_fixedUnscaledDetlaTime)
    {
        fixedUpdateFunction(m_fixedUnscaledDetlaTime, m_fixedDetlaTime);
        m_timeAcc -= m_fixedUnscaledDetlaTime;
    }

    /*unfixed update*/
    updateFunction(m_unscaledDeltaTime, m_deltaTime);
}