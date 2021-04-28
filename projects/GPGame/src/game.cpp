#define IMGUI_DEFINE_MATH_OPERATORS
#define GLFW_INCLUDE_NONE

#include "Game.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Parsers/ObjParser.hpp"
#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/ECS/Component/AudioComponent.hpp"
#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/ECS/Component/InputComponent.hpp"
#include "Engine/ECS/Component/Light/DirectionalLight.hpp"
#include "Engine/ECS/Component/Light/PointLight.hpp"
#include "Engine/ECS/Component/Model.hpp"
#include "Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp"
#include "Engine/ECS/Component/TransformComponent.hpp"
#include "Engine/ECS/System/BehaviourSystem.hpp"
#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/ECS/System/TimeSystem.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"
#include "Engine/Resources/Scene.hpp"
#include "Engine/Resources/Script/FreeFly.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Texture.hpp"
#include "myFpsScript.hpp"
#include <Engine/ECS/Component/Physics/Collisions/BoxCollider.hpp>
#include <Engine/ECS/Component/Physics/Collisions/SphereCollider.hpp>
#include <myFpsScript.hpp>

#include "Engine/Resources/Importer/Importer.hpp"

// Third_party
#include <glad/glad.h> //In first
#include <glfw/glfw3.h>

#include "imgui/imgui_internal.h"
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

using namespace GPE;
using namespace GPM;

void Game::update(double unscaledDeltaTime, double deltaTime)
{
    ++unFixedUpdateFrameCount;

    bSys.update(static_cast<float>(deltaTime));
    sm.getCurrentScene()->getWorld().updateSelfAndChildren();

    GPE::Engine::getInstance()->physXSystem.drawDebugScene();
}

void Game::fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime)
{
    GPE::Engine::getInstance()->physXSystem.advance(static_cast<float>(fixedDeltaTime));
    ++fixedUpdateFrameCount;
    bSys.fixedUpdate(static_cast<float>(fixedDeltaTime));
}

void Game::render()
{
    // UI code can be easly move in update because it's not real render function. It however her for simplicity
    // Initialize a new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::GetIO().DisplaySize = ImVec2((float)m_w, (float)m_h);
    ImGui::GetIO().MousePos -= ImVec2((float)m_x, (float)m_y);

    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2{(float)m_w, (float)m_h});
    ImGui::SetNextWindowPos({0, 0});

    ImGui::Begin("UI", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

    // Draw GUI
    GPE::Engine::getInstance()->behaviourSystem.onGUI();
    ImGui::End();
    ImGui::Render();

    RenderSystem& sceneRS = Engine::getInstance()->sceneManager.getCurrentScene()->sceneRenderer;
    sceneRS.render(sceneRS.defaultRenderPipeline());

    // draw UI
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

extern "C" GPE::AbstractGame* createGameInstance()
{
    // Init glad
    if (!gladLoadGL())
    {
        FUNCT_ERROR("gladLoadGLLoader failed");
        exit(EXIT_FAILURE);
    }
    GPE::AbstractGame* const pGame = new Game();
    GPE::Engine::getInstance()->behaviourSystem.awake();
    return pGame;
}

extern "C" void destroyGameInstance(GPE::AbstractGame* game)
{
    GPE_ASSERT(game != nullptr, "m_editor should be valid since we've just ran the editor.");
    delete game;
}

void loadTreeResource(ResourceManagerType& resourceManager)
{
    Model::CreateArg arg;

    SubModel subModel;
    subModel.pShader   = resourceManager.get<Shader>("TextureWithLihghts");
    subModel.pMaterial = loadMaterialFile("./resources/meshs/Trank_bark.GPMaterial");
    subModel.pMesh     = loadMeshFile("./resources/meshs/g1.GPMesh");

    arg.subModels.push_back(subModel);

    subModel.pMaterial = loadMaterialFile("./resources/meshs/DB2X2_L01.GPMaterial");
    subModel.pMesh     = loadMeshFile("./resources/meshs/g2.GPMesh");

    arg.subModels.push_back(subModel);

    resourceManager.add<Model::CreateArg>("TreeModel", arg);
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

void loadTree(GameObject& parent, ResourceManagerType& resourceManager, unsigned int number)
{
    GameObject::CreateArg treeGameObject{"Trees", {{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}}};

    GameObject&       treeContener = parent.addChild(treeGameObject);
    Model::CreateArg& treeModelArg = *resourceManager.get<Model::CreateArg>("TreeModel");

    /*Create tree with random size, position and rotation and add it on tre contener*/
    for (size_t i = 0; i < number; ++i)
    {
        treeGameObject.name                         = "Tree" + std::to_string(i);
        treeGameObject.transformArg.position.x      = randRanged<float>(-1000.f, 1000.f);
        treeGameObject.transformArg.position.z      = randRanged<float>(-1000.f, 1000.f);
        treeGameObject.transformArg.eulerRotation.y = randRanged<float>(360.f * 3.14f / 180.f);

        treeGameObject.transformArg.scale = {randRanged<float>(4.f, 8.f), randRanged<float>(4.f, 8.f),
                                             randRanged<float>(4.f, 8.f)};

        GameObject& treeGO = treeContener.addChild(treeGameObject);
        treeGO.addComponent<Model>(treeModelArg);
    }
}

void loadSkyboxResource(ResourceManagerType& resourceManager)
{
    Model::CreateArg arg;

    SubModel subModel;
    subModel.pShader               = loadShaderFile("./resources/Skybox.GPShader");
    subModel.pMaterial             = loadMaterialFile("./resources/Skybox.GPMaterial");
    subModel.pMesh                 = loadMeshFile("./resources/meshs/Cube.GPMesh");
    subModel.enableBackFaceCulling = false;

    arg.subModels.push_back(subModel);

    resourceManager.add<Model::CreateArg>("SkyboxModel", arg);
}

void loadSkyBox(GameObject& parent, ResourceManagerType& resourceManager)
{
    GameObject::CreateArg skyboxArgGameObject{"Skybox", {{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {10.f, 10.f, 10.f}}};

    parent.addChild(skyboxArgGameObject).addComponent<Model>(*resourceManager.get<Model::CreateArg>("SkyboxModel"));
}

void Game::initDearImGui(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void Game::setViewport(float x, float y, float w, float h)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
}

Game::~Game()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Game::Game()
{
    initDearImGui(GPE::Engine::getInstance()->window.getGLFWWindow());

    sm.addEmpty("main");
    Scene& currentScene = sm.loadScene("main");

    iManager.bindInput(GLFW_KEY_W, "forward");
    iManager.bindInput(GLFW_KEY_S, "back");
    iManager.bindInput(GLFW_KEY_A, "left");
    iManager.bindInput(GLFW_KEY_D, "right");
    iManager.bindInput(GLFW_KEY_SPACE, "jump");
    iManager.bindInput(GLFW_KEY_LEFT_CONTROL, "down");
    iManager.bindInput(GLFW_KEY_ESCAPE, "exitGame01");
    iManager.bindInput(GLFW_KEY_ESCAPE, "exitGame02");
    iManager.bindInput(GLFW_KEY_LEFT_SHIFT, "sprintStart");
    iManager.bindInput(GLFW_KEY_LEFT_SHIFT, "sprintEnd");
    iManager.bindInput(GLFW_KEY_KP_ADD, "growUpCollider");
    iManager.bindInput(GLFW_KEY_KP_SUBTRACT, "growDownCollider");
    iManager.bindInput(GLFW_KEY_X, "swapInputModeToGame01");
    iManager.bindInput(GLFW_KEY_X, "swapInputModeToGame02");

    iManager.setInputMode("game02");
    iManager.setCursorTrackingState(false);

    int x, y;
    GPE::Engine::getInstance()->window.getSize(x, y);

    GameObject::CreateArg playerArg{"Player", TransformComponent::CreateArg{GPM::Vec3{0.f, 50.f, 0.f}}};
    GameObject::CreateArg testPhysXArg{"TestphysX", TransformComponent::CreateArg{GPM::Vec3{0.f, 0.f, 50.f}}};
    GameObject::CreateArg groundArg{"GroundArg", TransformComponent::CreateArg{GPM::Vec3{0.f, 0.f, 0.f}}};

    Camera::PerspectiveCreateArg camCreateArg;
    camCreateArg.aspect = Camera::computeAspect(900, 600);

    camCreateArg.farVal  = 3000;
    camCreateArg.nearVal = 0.01f;

    PointLight::CreateArg lightArg{
        {1.f, 1.f, 1.f, 0.1f}, {1.f, 1.f, 1.f, 1.0f}, {1.f, 1.f, 1.f, 1.f}, 1.0f, 0.0014f, 0.000007f};

    rm.add<Shader>("TextureOnly", "./resources/shaders/vTextureOnly.vs", "./resources/shaders/fTextureOnly.fs",
                   AMBIANTE_COLOR_ONLY);
    rm.add<Shader>("TextureWithLihghts", "./resources/shaders/vTextureWithLight.vs",
                   "./resources/shaders/fTextureWithLight.fs", LIGHT_BLIN_PHONG);

    GameObject& ground    = sm.getCurrentScene()->getWorld().addChild(groundArg);
    GameObject& player    = sm.getCurrentScene()->getWorld().addChild(playerArg);
    GameObject& testPhysX = sm.getCurrentScene()->getWorld().addChild(testPhysXArg);

    player.addComponent<Camera>(camCreateArg);
    player.addComponent<GPG::MyFpsScript>();
    PointLight& light = player.addComponent<PointLight>(lightArg);

    testPhysX.addComponent<SphereCollider>();
    testPhysX.getComponent<SphereCollider>()->isVisible = true;
    testPhysX.getComponent<SphereCollider>()->setRadius(10.f);
    testPhysX.addComponent<RigidbodyStatic>();
    testPhysX.getComponent<RigidbodyStatic>()->collider = testPhysX.getComponent<SphereCollider>();

    // FreeFly must be used to compile properly with GPGame.dll, to not be optimized out, for serialization.
    {
        rfk::Entity const* a = rfk::Database::getEntity(GPE::FreeFly::staticGetArchetype().id);
    }

    /*Model::CreateArg modelArg;
    modelArg.subModels.emplace_back(SubModel{nullptr, Engine::getInstance()->resourceManager.get<Shader>("TextureOnly"),
                                             Engine::getInstance()->resourceManager.get<Material>("SkyboxMaterial"),
                                             Engine::getInstance()->resourceManager.get<Mesh>("Sphere")});

    testPhysX.addComponent<Model>(modelArg);*/

    ground.addComponent<BoxCollider>();
    ground.getComponent<BoxCollider>()->isVisible = true;
    ground.getComponent<BoxCollider>()->setDimensions(Vec3{1000.f, 10.f, 1000.f});
    ground.addComponent<RigidbodyStatic>();
    ground.getComponent<RigidbodyStatic>()->collider = ground.getComponent<BoxCollider>();

    /*Model::CreateArg modelArg2;
    modelArg2.subModels.emplace_back(SubModel{nullptr,
                                              Engine::getInstance()->resourceManager.get<Shader>("TextureOnly"),
                                              Engine::getInstance()->resourceManager.get<Material>("SkyboxMaterial"),
                                              Engine::getInstance()->resourceManager.get<Mesh>("CubeDebug")});

    ground.addComponent<Model>(modelArg2);*/
    loadSkyboxResource(rm);
    loadTreeResource(rm);

    loadSkyBox(sm.getCurrentScene()->getWorld(), rm);
    loadTree(sm.getCurrentScene()->getWorld(), rm, 100);

    ts.addScaledTimer(
        FPLogDelay,
        [&]() {
            std::cout << "FPS (fixedUpdate): " << fixedUpdateFrameCount / FPLogDelay << std::endl;
            std::cout << "FPS (unFixedUpdate): " << unFixedUpdateFrameCount / FPLogDelay << std::endl << std::endl;
            fixedUpdateFrameCount   = 0;
            unFixedUpdateFrameCount = 0;
        },
        true);

    Log::getInstance()->logInitializationEnd("Game");
}
