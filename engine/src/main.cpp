#include <iostream>

#include "Engine/Core/Parsers/ObjParser.hpp"
#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Core/TimeSystem/TimeSystem.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Light/DirectionnalLight.hpp"
#include "Engine/Intermediate/RenderSystem.hpp"
#include "Engine/Intermediate/TransformComponent.hpp"
#include "Engine/Resources/Camera.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/Model.hpp"
#include "Engine/Resources/ResourcesManager.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Texture.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"

using namespace Engine::Resources;
using namespace Engine::Core::Renderering;
using namespace Engine::Core::Debug;
using namespace Engine::Intermediate;
using namespace Engine::Core::Parsers;
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

    GPE_ASSERT(true, "GPE_assert");
}

void loadTreeResource(ResourcesManager<Mesh, Shader, Texture, std::vector<Material>>& resourceManager)
{
    Attrib                      attrib;
    std::vector<Shape>          shape;
    std::vector<MaterialAttrib> materialAttribs;

    loadObjWithMTL("./resources/meshs/Tree.obj", &attrib, &shape, &materialAttribs);

    resourceManager.add<Mesh>("TreeMesh", attrib, shape);

    {
        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign(materialAttribs.begin(), materialAttribs.end());
        resourceManager.add<std::vector<Material>>("TreeMaterials", material);
    }
}

template <typename T = float>
static auto randRanged(const T& max) -> std::enable_if_t<std::is_floating_point<T>::value, T>
{
    return max <= std::numeric_limits<T>::epsilon() ? static_cast<T>(0)
                                                    : static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / max));
}

template <typename T = float>
static auto randRanged(const T& min, const T& max) -> std::enable_if_t<std::is_floating_point<T>::value, T>
{
    return max - min <= std::numeric_limits<T>::epsilon()
               ? max
               : min + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (max - min)));
}

void loadTree(GameObject& parent, ResourcesManager<Mesh, Shader, Texture, std::vector<Material>>& resourceManager,
              unsigned int number)
{
    GameObject::CreateArg treeGameObject{"Trees", {{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}}};

    GameObject&      treeContener = parent.addChild<GameObject>(treeGameObject);
    Model::CreateArg treeModelArg{resourceManager.get<Shader>("TextureWithLihghts"),
                                  resourceManager.get<std::vector<Material>>("TreeMaterials"),
                                  resourceManager.get<Mesh>("TreeMesh")};

    /*Create tree with random size, position and rotation and add it on tre contener*/
    for (size_t i = 0; i < number; i++)
    {
        treeGameObject.name                         = "Tree" + std::to_string(i);
        treeGameObject.transformArg.position.x      = randRanged<float>(-250.f, 250.f);
        treeGameObject.transformArg.position.z      = randRanged<float>(-250.f, 250.f);
        treeGameObject.transformArg.eulerRotation.y = randRanged<float>(360.f * 3.14f / 180.f);
        float globalScale                           = randRanged<float>(8.f, 12.f);
        treeGameObject.transformArg.scale += globalScale;

        treeGameObject.transformArg.scale = {randRanged<float>(4.f, 8.f), randRanged<float>(4.f, 8.f),
                                             randRanged<float>(4.f, 8.f)};

        GameObject& treeGO = treeContener.addChild<GameObject>(treeGameObject);
        treeGO.addComponent<Model>(treeModelArg);
    }
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

    Window     win(WindowCreateArg{"GP engine", 600, 900});
    Renderer   ren(win);
    TimeSystem ts;

    Log::logInitializationStart("sceneGraphExample");

    GameObject  world(GameObject::CreateArg{"World"});
    GameObject::CreateArg playerArg{"Player", TransformComponent::CreateArg{GPM::Vec3{0.f, 5.f, 0.f}}};

    GameObject& player = world.addChild<GameObject>(playerArg);

    Camera::PerspectiveCreateArg camCreateArg;
    camCreateArg.far = 10000.f;
    player.addComponent<Camera>(camCreateArg);

    DirectionnalLight::CreateArg lightArg{
        {0.f, 1.f, -1.f}, {1.f, 0.f, 0.f, 0.1f}, {1.f, 0.f, 0.f, 0.7f}, {1.f, 0.f, 0.f, 1.f}};
    player.addComponent<DirectionnalLight>(lightArg);

    ResourcesManager<Mesh, Shader, Texture, std::vector<Material>> rm;

    rm.add<Shader>("TextureOnly", "./resources/shaders/vTextureOnly.vs", "./resources/shaders/fTextureOnly.fs");
    rm.add<Shader>("TextureWithLihghts", "./resources/shaders/vTextureWithLight.vs",
                   "./resources/shaders/fTextureWithLight.fs", LIGHT_BLIN_PHONG);

    loadTreeResource(rm);
    loadTree(world, rm, 5000);

    Log::logInitializationEnd("sceneGraphExample");

    ResourceManagerExample();

    logExample();

    bool   m_logFPS                = true;
    int    fixedUpdateFrameCount   = 0;
    int    unFixedUpdateFrameCount = 0;
    double chronoFPLog             = 0.;
    double m_FPLogDelay            = 3.;

    float speedPlayer = 5.f;

    ts.addScaledTimer(
        m_FPLogDelay,
        [&]() {
            std::cout << "FPS (fixedUpdate): " << fixedUpdateFrameCount / m_FPLogDelay << std::endl;
            std::cout << "FPS (unFixedUpdate): " << unFixedUpdateFrameCount / m_FPLogDelay << std::endl << std::endl;
            fixedUpdateFrameCount   = 0;
            unFixedUpdateFrameCount = 0;
        },
        true);

    while (1)
    {
        ts.update([&](double fixedUnscaledDeltaTime, double fixedDeltaTime) { ++fixedUpdateFrameCount; },
                  [&](double unscaledDeltaTime, double deltaTime) {
                      ++unFixedUpdateFrameCount;
                      player.getTransform().translate(GPM::Vec3(0.f, 0.f, speedPlayer * deltaTime));
                      world.updateSelfAndChildren();
                  },
                  [&]() {
                      RenderSystem::getInstance()->draw();
                      ren.swapBuffer();
                  });
    }

    Log::closeAndTryToCreateFile();

    return 0;
}
