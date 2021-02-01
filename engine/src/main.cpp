#include <iostream>

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"

#include "Engine/Core/HotReload/ReloadableCpp.hpp"

using namespace Engine::Core::Debug;

/* Game Api Functions Signatures*/
void myTestFunction();

void hotReloadTest(int argc, char *argv[])
{
    // Get the name of the directory the .exe is in.
    // We are assuming that the .dll to live-reload is in the same directory.
    std::string directory(argv[0], argv[0] + strlen(argv[0]));
    directory.erase(directory.find_last_of(L'\\') + 1);

    // Specify the path to the DLL and the functions we want to load from it.
    // Engine::Core::HotReload::ReloadableCpp rcpp = directory + "live_reloaded_code.dll";
    Engine::Core::HotReload::ReloadableCpp rcpp = directory + "GPGame.dll";
    ADD_PROCESS(rcpp, myTestFunction);

    while (true)
    {
        // Poll the DLL and update any live-reloaded function from it.
        // if (Poll(rcpp))
        if (rcpp.refresh())
        {
            // Get the pointer to the live-reloaded function,
            // and cast it to a function pointer type with the right signature.
            auto onReloadPtr = GET_PROCESS(rcpp, myTestFunction);

            // If loading the function pointer worked properly, we can call it.
            if (onReloadPtr)
            {
                onReloadPtr();
            }
        }

        Sleep(1000);
    }
}

int main(int argc, char *argv[])
{
    hotReloadTest(argc, argv);
    // Log::setSetting(ESetting::ALWAYS_PRINT_LOG_FILE, true);

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

    GPE_ASSERT(false, "GPE_assertInfo");

    Log::closeAndTryToCreateFile();

    return 0;
}