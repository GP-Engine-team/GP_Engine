#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Tools/Format.hpp"

#include <GLFW/glfw3.h>

using namespace Engine::Core::Renderering;
using namespace Engine::Core::Tools;
using namespace Engine::Core::Debug;
using namespace std;

Window::Window(const WindowCreateArg& arg) noexcept
{
    Log::logInitializationStart("GLFW window");

    // Init glfw
    if (!glfwInit())
    {
        FUNCT_ERROR("glfwInit failed");
        exit(EXIT_FAILURE);
    }

#ifdef _DEBUG
    glfwSetErrorCallback([](int errorCode, const char* description) {
        Log::logError(stringFormat("GLFW error (%x) = %s\n", errorCode, description));
    });
#endif

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(arg.width, arg.height, arg.name.c_str(), nullptr, nullptr);

    if (!m_window)
    {
        FUNCT_ERROR("glfwCreateWindow failed");
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // enable v-sync

    Log::logInitializationEnd("GLFW window");
}

Window::~Window() noexcept
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
    Log::log("GLFW window release");
}

void Engine::Core::Renderering::Window::getSize(int& width, int& height) const noexcept
{
    glfwGetWindowSize(m_window, &width, &height);
}