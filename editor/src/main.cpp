#include "Editor/EditorStartup.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "include/glad/glad.h"
#include "include/GLFW/glfw3.h"

using namespace GPE;

int main(int argc, char* argv[])
{
	Editor::EditorStartup editorStartup;
	editorStartup.run();

	return 0;
}