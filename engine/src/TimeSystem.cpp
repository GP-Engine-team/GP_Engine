#include "Engine/ECS/System/TimeSystem.hpp"
#include "Engine/Core/Debug/Log.hpp"

#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>
#include <string>

using namespace GPE;

static void initDearImGui(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

TimeSystem::TimeSystem(Window& window)
{
    initDearImGui(window.getGLFWWindow());
}

void TimeSystem::update(std::function<void(double fixedUnscaledDeltaTime, double fixedDeltaTime)> fixedUpdateFunction,
                        std::function<void(double unscaledDeltaTime, double deltaTime)>           updateFunction,
                        std::function<void()> renderFunction) noexcept
{
    /*Update imGui frames*/
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    /*unfixed update*/
    updateFunction(m_unscaledDeltaTime, m_deltaTime);

    /*render the current frame*/
    renderFunction();

    ImGui::Render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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