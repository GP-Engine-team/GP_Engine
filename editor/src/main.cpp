#include "Engine/Core/Debug/Log.hpp"
#include "Editor/EditorStartup.hpp"

using namespace Engine::Core::Debug;

int main(int argc, char* argv[])
{
    Log::logFileHeader();

    Log::logInitializationStart("GameStartup creation");

    EditorStartup editorStartup;
    editorStartup.run();

    return 0;
}