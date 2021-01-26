#include <iostream>

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Debug/Assert.hpp"

using namespace Engine::Core::Debug;


int main ()
{
    //Log::setSetting(ESetting::ALWAYS_PRINT_LOG_FILE, true);

    Log::init();

    Log::logInitializationStart("logInitializationStart");

    Log::logInitializationStep("logInitializationStep", 50);

    Log::log("log");
    Log::logTips("logTips");
    Log::logWarning("logWarning");
    Log::logError("logError");
    Log::logInitializationEnd("logInitializationEnd");

    functError("functError");
    functWarning("functWarning");

    GPE_assert(false, "GPE_assertInfo");

    Log::release();

    return 0;
}