#include "Engine/Core/Debug/Log.hpp"
#include "GameStartup.hpp"

using namespace Engine::Core::Debug;

int main(int argc, char* argv[])
{
    Log::logFileHeader();

    Log::logInitializationStart("GameStartup creation");

    GameStartup gameStartup;
    gameStartup.run();

    return 0;
}