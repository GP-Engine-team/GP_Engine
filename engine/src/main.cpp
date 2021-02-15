#include <iostream>

#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Core/TimeSystem/TimeSystem.hpp"
#include "Engine/Resources/ResourcesManager.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Intermediate/TransformComponent.hpp"
#include "Engine/Core/Input/InputManagerGLFW.hpp"
#include "GLFW/glfw3.h"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"

using namespace Engine::Resources;
using namespace Engine::Core::Renderering;
using namespace Engine::Core::Debug;
using namespace Engine::Core::Input;
using namespace Engine::Intermediate;
using namespace Engine::Core;

#ifdef _WIN32
// Use discrete GPU by default.
extern "C"
{
    // http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
    __declspec(dllexport) int NvOptimusEnablement = 1;

    // https://community.amd.com/t5/firepro-development/can-an-opengl-app-default-to-the-discrete-gpu-on-an-enduro/m-p/279441#M471
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

void ResourceManagerExample()
{
    Log::logInitializationStart("ResourceManagerExample");
    ResourcesManager<int, float, double> rm;

    rm.add<int>("six", 6);

    std::cout << *rm.get<int>("six") << std::endl;
    Log::logInitializationEnd("ResourceManagerExample");
}

void logExample()
{
    Log::logFileHeader();

    Log::logInitializationStart("logExample");

    Log::logInitializationStep("logInitializationStep", 50);

    Log::log("log");
    Log::logTips("logTips");
    Log::logWarning("logWarning");
    Log::logError("logError");
    Log::logInitializationEnd("logExample");

    FUNCT_ERROR("functError");
    FUNCT_WARNING("functWarning");

    GPE_ASSERT(true, "GPE_assertInfo");
}

void sceneGraphExample()
{
    Log::logInitializationStart("sceneGraphExample");

    GameObject world(GameObjectCreateArg{"Worsld"});
    GameObject player(GameObjectCreateArg{"Player"});
    player.addComponent<TransformComponent>(); //Add additionnal transformComponent
    player.destroyImmediateUniqueComponent<TransformComponent>();

    player.parent = &world;
    std::cout << player.getRelativePath() << std::endl;

    Log::logInitializationEnd("sceneGraphExample");
}

void logTimerExample(TimeSystem& ts)
{
    Log::logInitializationStart("logTimerExample");

    ts.addScaledTimer(
        0.5, [&]() { std::cout << "Timer 0.5" << std::endl; }, true);

    ts.addScaledTimer(
        2., [&]() { std::cout << "Timer 2." << std::endl; }, true);

    Log::logInitializationEnd("logTimerExample");
}

int main()
{
    // Log::setSetting(ESetting::ALWAYS_PRINT_LOG_FILE, true);

    Window win(WindowCreateArg{"GP engine", 600, 900});
    Renderer ren(win);
    TimeSystem ts;

    InputManager::GetInstance()->setupCallbacks(win.getGLFWWindow());

    sceneGraphExample();

    ResourceManagerExample();

    logExample();

    bool m_logFPS = true;
    int fixedUpdateFrameCount = 0;
    int unFixedUpdateFrameCount = 0;
    double chronoFPSLog = 0.;
    double m_FPSLogDelay = 3.;

     ts.addScaledTimer(
        m_FPSLogDelay,
        [&]() 
     {
            std::cout << "FPS (fixedUpdate): " << fixedUpdateFrameCount / m_FPSLogDelay << std::endl;
            std::cout << "FPS (unFixedUpdate): " << unFixedUpdateFrameCount / m_FPSLogDelay << std::endl << std::endl;
            fixedUpdateFrameCount = 0;
            unFixedUpdateFrameCount = 0;

     }, true);

    while (1)
    {
        glfwPollEvents();
        ts.update([&](double fixedUnscaledDeltaTime, double fixedDeltaTime)
        { 
            ++fixedUpdateFrameCount;
        }, 
            [&](double unscaledDeltaTime, double deltaTime)
        { 
            ++unFixedUpdateFrameCount;
        }, 
            [&]() {});
    }

    Log::closeAndTryToCreateFile();

    return 0;
}
