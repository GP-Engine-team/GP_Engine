/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Debug/Log.hpp"
#include <chrono>     // std::chrono::seconds, std::chrono::system_clock::now()
#include <functional> //std::function
#include <queue>

namespace Engine::Core
{
struct TimerTask
{
    double localTimer = 0.; // if current time egal 1s and local timer egal 0.5 global time egal 1.5
    double globalTimer = 0.;
    std::function<void()> task = nullptr;
    bool isLooping = false;

    bool operator>(const TimerTask& other) const noexcept
    {
        return globalTimer > other.globalTimer;
    }
};

class TimeSystem
{
protected:
    std::chrono::system_clock::time_point m_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point m_tempTime = m_time;
    double m_fixedTimeAcc = 0.;

    unsigned int m_frameAcc = 0;

    double m_deltaTime = 0.;
    double m_unscaledDeltaTime = 0.;
    double m_timeScale = 1.;
    double m_fixedUnscaledDetlaTime = 1. / 60.;
    double m_fixedDetlaTime = m_fixedUnscaledDetlaTime * m_timeScale;

    double m_scaledTimeAcc = 0.f;
    double m_unscaledTimeAcc = 0.f;

    std::priority_queue<TimerTask, std::vector<TimerTask>, std::greater<TimerTask>> m_scaledTimerQueue;
    std::priority_queue<TimerTask, std::vector<TimerTask>, std::greater<TimerTask>> m_unscaledTimerQueue;

public:
    /**
     * @brief Update the time, update system and renderSystem. The update function will call with accumulator to be call
     * a fixed time by second
     *
     * @param fixedUpdateFunction : lambda that contain all fixed update function
     * @param updateFunction : lambda that contain all update function
     * @param renderFunction : lambda that contain all render function
     */
    void update(std::function<void(double fixedUnscaledDetlaTime, double fixedDetlaTime)> fixedUpdateFunction,
                std::function<void(double unscaledDeltaTime, double deltaTime)> updateFunction,
                std::function<void()> renderFunction) noexcept;

    [[nodiscard]] inline double getFixedDeltaTime() noexcept;

    [[nodiscard]] inline double getFixedUnscaledDeltaTime() noexcept;

    inline void setFixedUnscaledDeltaTime(double newFixedUnscaledDetlaTime) noexcept;

    [[nodiscard]] inline double getDeltaTime() noexcept;

    [[nodiscard]] inline double getUnscaledDeltaTime() noexcept;

    [[nodiscard]] inline double getAccumulateTime() noexcept;

    [[nodiscard]] inline double getAccumulateUnscaledTime() noexcept;

    [[nodiscard]] inline double getTimeScale() noexcept;

    inline void setTimeScale(double newtimeScale) noexcept;

    inline void addScaledTimer(double delay, std::function<void()> functionToExecute, bool isLooping = false) noexcept;
    inline void addUnscaledTimer(double delay, std::function<void()> functionToExecute,
                                 bool isLooping = false) noexcept;

    inline void clearScaledTimer() noexcept;
    inline void clearUnscaledTimer() noexcept;
};

} // namespace Engine::Core

#include "TimeSystem.inl"