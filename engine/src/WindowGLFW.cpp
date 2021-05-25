#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Tools/Format.hpp"

#include <GLFW/glfw3.h>

using namespace GPE;
using namespace std;

Window::Window(const CreateArg& arg) noexcept
{
    Log::getInstance()->logInitializationStart("GLFW window");

    // Init glfw
    if (!glfwInit())
    {
        FUNCT_ERROR("glfwInit failed");
        exit(EXIT_FAILURE);
    }

#ifdef _DEBUG
    glfwSetErrorCallback([](int errorCode, const char* description) {
        Log::getInstance()->logError(stringFormat("GLFW error (%x) = %s\n", errorCode, description));
    });
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    m_window = glfwCreateWindow(arg.width, arg.height, arg.name.c_str(), nullptr, nullptr);

    if (!m_window)
    {
        FUNCT_ERROR("glfwCreateWindow failed");
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0); // disable v-sync

    Log::getInstance()->logInitializationEnd("GLFW window");
}

Window::~Window() noexcept
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
    Log::getInstance()->log("GLFW window release");
}

void Window::close()
{
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

void Window::getSize(int& width, int& height) const noexcept
{
    glfwGetWindowSize(m_window, &width, &height);
}