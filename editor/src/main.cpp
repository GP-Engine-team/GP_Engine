#define GLFW_INCLUDE_NONE
#include <Editor/EditorStartup.hpp>

using namespace GPE;

int main(int argc, char* argv[])
{
    Editor::EditorStartup editorStartup;
    editorStartup.run();

    return 0;
}
