#include <iostream>

#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"

using namespace Engine::Core::Renderering;
using namespace Engine::Core::Debug;

extern "C"
{
    _declspec(dllexport) int NvOptimusEnablement = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

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

    WindowGLFW win(WindowCreateArg{"GP engine", 600, 900});
    RendererGLFW_GL46 ren(win);

    logExample();

    Log::closeAndTryToCreateFile();

    return 0;
}
