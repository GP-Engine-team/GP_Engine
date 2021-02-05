/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Debug/Log.hpp"
#include <chrono> // std::chrono::seconds, std::chrono::system_clock::now()
#include <functional>

namespace Engine::Core
{
class TimeSystem
{
protected :

    std::chrono::system_clock::time_point m_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point m_tempTime = m_time;
    double m_timeAcc = 0.;

    unsigned int m_frameAcc = 0;

    double m_deltaTime = 0.;
    double m_unscaledDeltaTime = 0.;
    double m_timeScale = 1.;
    double m_fixedUnscaledDetlaTime = 1. / 60.;
    double m_fixedDetlaTime = m_fixedUnscaledDetlaTime * m_timeScale;

    double m_scaledTimeAcc = 0.f;
    double m_unscaledTimeAcc = 0.f;

public:

    /**
     * @brief Update the time, update system and renderSystem. The update function will call with accumulator to be call
     * a fixed time by second
     *
     * @param fixedUpdateFunction : lambda that contain all fixed update function (that use fixed deltaTime)
     * @param updateFunction : lambda that contain all update function (physic update, scene, update...)
     * @param renderFunction : lambda that contain all render function (clear screen, display elements, display
     * screen...)
     */
    void update(std::function<void(double fixedUnscaledDetlaTime, double fixedDetlaTime)> fixedUpdateFunction, 
                std::function<void(double unscaledDeltaTime, double deltaTime)> updateFunction,
                std::function<void()> renderFunction) noexcept;

    double getFixedDeltaTime() noexcept;

    double getFixedUnscaledDeltaTime() noexcept;
    void   setFixedUnscaledDeltaTime(double newFixedUnscaledDetlaTime) noexcept;

    double getDeltaTime() noexcept;
    double getUnscaledDeltaTime() noexcept;

    double getAccumulateTime() noexcept;
    double getAccumulateUnscaledTime() noexcept;

    double getTimeScale() noexcept;
    void   setTimeScale(double newtimeScale) noexcept;
};

} // namespace Engine::Core

#include "TimeSystem.inl"