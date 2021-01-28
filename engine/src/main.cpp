#include <iostream>

#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"

using namespace Engine::Core::Renderering;
using namespace Engine::Core::Debug;

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

    RendererGLFW_GL46 ren (WindowCreateArg{"GP engine", 600, 900});

    logExample();

    Log::closeAndTryToCreateFile();

    return 0;
}

