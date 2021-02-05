#include <iostream>

#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Core/TimeSystem/TimeSystem.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"

using namespace Engine::Core::Renderering;
using namespace Engine::Core::Debug;
using namespace Engine::Core;

#ifdef _WIN32
// Use discrete GPU by default.
extern "C"
{
    // http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
    __declspec(dllexport) int NvOptimusEnablement = 1;

    // https://community.amd.com/t5/firepro-development/can-an-opengl-app-default-to-the-discrete-gpu-on-an-enduro/m-p/279441#M471
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

void logExample()
{
    Log::logFileHeader();

    Log::logInitializationStart("logInitializationStart");

    Log::logInitializationStep("logInitializationStep", 50);

    Log::log("log");
    Log::logTips("logTips");
    Log::logWarning("logWarning");
    Log::logError("logError");
    Log::logInitializationEnd("logInitializationEnd");

    FUNCT_ERROR("functError");
    FUNCT_WARNING("functWarning");

    GPE_ASSERT(true, "GPE_assertInfo");
}

int main()
{
    // Log::setSetting(ESetting::ALWAYS_PRINT_LOG_FILE, true);

    Window win(WindowCreateArg{"GP engine", 600, 900});
    Renderer ren(win);
    TimeSystem ts;

    logExample();

    bool m_logFPS = true;
    int fixedUpdateFrameCount = 0;
    int unFixedUpdateFrameCount = 0;
    double chronoFPSLog = 0.;
    double m_FPSLogDelay = 1.;

    while (1)
    {
        ts.update([&](double fixedUnscaledDetlaTime, double fixedDetlaTim)
        { 
            ++fixedUpdateFrameCount;
        }, 
            [&](double unscaledDeltaTime, double deltaTime)
        { 
            ++unFixedUpdateFrameCount;

            chronoFPSLog += unscaledDeltaTime;

            /*Display FPS*/
            if (chronoFPSLog >= m_FPSLogDelay)
            {
                std::cout << "FPS (fixedUpdate): " << fixedUpdateFrameCount << std::endl;
                std::cout << "FPS (unFixedUpdate): " << unFixedUpdateFrameCount << std::endl << std::endl;
                chronoFPSLog -= m_FPSLogDelay;
                fixedUpdateFrameCount = 0;
                unFixedUpdateFrameCount = 0;
            }
        }, 
            [&]() 
        {
        
        });
    }

    Log::closeAndTryToCreateFile();

    return 0;
}
