#include "Engine/Core/Debug/Log.hpp"
#include "Editor/EditorStartup.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "include/glad/glad.h"
#include "include/GLFW/glfw3.h"

using namespace Engine::Core::Debug;
using namespace Editor;


int main(int argc, char* argv[])
{
    Log::logFileHeader();

    Log::logInitializationStart("EditorStartup creation");

    EditorStartup editorStartup;
    editorStartup.run();

    return 0;
}