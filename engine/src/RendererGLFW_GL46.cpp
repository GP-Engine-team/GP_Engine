#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"

#include <glad/glad.h> //In first

#include <GLFW/glfw3.h> //In second

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Core/Tools/Format.hpp"
#include "Engine/ECS/System/SystemsManager.hpp"

using namespace GPE;
using namespace std;

inline void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                            const GLchar* msg, const void* data)
{
    const char* _source;
    const char* _type;
    const char* _severity;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

    default:
        _severity = "UNKNOWN";
        break;
    }

    Log::logError(stringFormat("%d: %s of %s severity, raised from %s: %s\n", id, _type, _severity, _source, msg));
}

Renderer::Renderer(Window& window) noexcept : m_pWindow{&window}
{
    Log::logInitializationStart("GLFW / OpenGL 4.6 Renderer");

    // Init glad
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        FUNCT_ERROR("gladLoadGLLoader failed");
        exit(EXIT_FAILURE);
    }

    // Setup KHR_debug callback
    if (GLAD_GL_KHR_debug)
    {
#ifdef _DEBUG
        // The following function calls should be made directly after OpenGL
        // initialization.

        // Enable the debugging layer of OpenGL
        //
        // GL_DEBUG_OUTPUT - Faster version but not useful for breakpoints
        // GL_DEBUG_OUTPUT_SYNCHRONUS - Callback is in sync with errors, so a breakpoint
        // can be placed on the callback in order to get a stacktrace for the GL error.

        // glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

        glDebugMessageCallback(GLDebugMessageCallback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
    }

    Log::logInitializationEnd("SDL/OpenGL 4.6 Renderer");
    Log::log(stringFormat("GL_VENDOR = %s", glGetString(GL_VENDOR)));
    Log::log(stringFormat("GL_RENDERER = %s", glGetString(GL_RENDERER)));
    Log::log(stringFormat("GL_VERSION = %s", glGetString(GL_VERSION)));
}

Renderer::~Renderer() noexcept
{
    SystemsManager::getInstance()->renderSystem.removeRenderer(this);

    Log::log("GLFW / OpenGL 4.6 renderer release");
}

void Renderer::swapBuffer() noexcept
{
    // Present frame
    glfwSwapBuffers(m_pWindow->getGLFWWindow());
}
