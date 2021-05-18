/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <chrono>     // std::chrono::seconds, std::chrono::system_clock::now()
#include <functional> //std::function
#include <queue>

namespace GPE
{
struct TimerTask
{
    std::function<void()> task        = nullptr;
    double                localTimer  = 0.; // if current time egal 1s and local timer egal 0.5 global time egal 1.5
    double                globalTimer = 0.;
    bool                  isLooping   = false;

    TimerTask(const std::function<void()>& task = nullptr, double localTimer = .0, double globalTimer = .0, bool isLooping = false)
        : task{task}, localTimer{localTimer}, globalTimer{globalTimer}, isLooping{isLooping}
    {}

    bool operator>(const TimerTask& other) const noexcept
    {
        return globalTimer > other.globalTimer;
    }
};

class TimeSystem
{
protected:
    std::chrono::steady_clock::time_point m_time                 = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point m_tempTime             = m_time;
    double                                m_fixedUnscaledTimeAcc = 0.;

    double m_deltaTime              = 0.;
    double m_unscaledDeltaTime      = 0.;
    double m_timeScale              = 1.;
    double m_fixedUnscaledDeltaTime = 1. / 60.;
    double m_fixedDeltaTime         = m_fixedUnscaledDeltaTime * m_timeScale;

    double m_scaledTimeAcc   = 0.f;
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
    void update(std::function<void(double fixedUnscaledDeltaTime, double fixedDeltaTime)> fixedUpdateFunction,
                std::function<void(double unscaledDeltaTime, double deltaTime)>           updateFunction,
                std::function<void()>                                                     renderFunction) noexcept;

    /**
     * @brief Change the fixed delta time used in fixed update. By default this value egales : 1/60 or 60 Hz
     * @param newFixedUnscaledDeltaTime
     * @return
     */
    inline constexpr void setFixedUnscaledDeltaTime(double newFixedUnscaledDeltaTime) noexcept;

    [[nodiscard]] inline constexpr double getFixedDeltaTime() const noexcept;
    [[nodiscard]] inline constexpr double getFixedUnscaledDeltaTime() const noexcept;
    [[nodiscard]] inline constexpr double getDeltaTime() const noexcept;
    [[nodiscard]] inline constexpr double getUnscaledDeltaTime() const noexcept;
    [[nodiscard]] inline constexpr double getAccumulatedTime() const noexcept;
    [[nodiscard]] inline constexpr double getAccumulatedUnscaledTime() const noexcept;
    [[nodiscard]] inline constexpr double getTimeScale() const noexcept;

    /**
     * @brief Redefine time scale. This value will use to compute scaled delta time and scaled fixed delta time. Useful
     * to change the game speed
     * @param newtimeScale
     * @return
     */
    inline constexpr void setTimeScale(double newtimeScale) noexcept;

    /**
     * @brief Add Task into scaled timer container. This container allow you to create event after a delay and allow
     * you to repeat it
     * @param delay
     * @param functionToExecute
     * @param isLooping
     * @return
     */
    inline void emplaceScaledTimer(std::function<void()> functionToExecute, double delay,
                                   bool isLooping = false) noexcept;

    /**
     * @brief Add Task into unscaled timer container. This container allow you to create event after a delay and allow
     * you to repeat it
     * @param delay
     * @param functionToExecute
     * @param isLooping
     * @return
     */
    inline void emplaceUnscaledTimer(std::function<void()> functionToExecute, double delay,
                                     bool isLooping = false) noexcept;

    /**
     * @brief Clear all task of the scaled timer container
     * @return
     */
    inline void clearScaledTimer() noexcept;

    /**
     * @brief Clear all task of the unscaled timer container
     * @return
     */
    inline void clearUnscaledTimer() noexcept;
};

#include "TimeSystem.inl"

} // namespace GPE
