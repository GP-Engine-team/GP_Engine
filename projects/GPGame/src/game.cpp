#define IMGUI_DEFINE_MATH_OPERATORS
#define GLFW_INCLUDE_NONE

#include <BasePlayer.hpp>
#include <Game.hpp>

#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyDynamic.hpp>
#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyStatic.hpp>
#include <Engine/ECS/System/RenderSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Script/FreeFly.hpp>
#include <SpatializedSoundPlayerScript.hpp>

#include <Sun.hpp>
#include <WorldGenerator.hpp>

#include <GPM/Random.hpp>

// Third_party
#include <glfw/glfw3.h>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Engine/Core/HotReload/SingletonsSync.hpp"

#include <Engine/Core/Physics/Collisions/BoxCollider.hpp>
#include <Engine/Core/Physics/Collisions/SphereCollider.hpp>

using namespace GPG;
using namespace GPE;
using namespace GPM;
using namespace GPM::Random;

void Game::update(double unscaledDeltaTime, double deltaTime)
{
    ++unFixedUpdateFrameCount;
}

void Game::fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime)
{
    ++fixedUpdateFrameCount;
}

void Game::render()
{
    // TODO: Put in-game UI in a module
    // UI code can be easly move in update because it's not real render function. It however her for simplicity
    // Initialize a new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::GetIO().DisplaySize = {m_w, m_h};
    ImGui::GetIO().MousePos -= {m_x, m_y};

    ImGui::NewFrame();

    ImGui::SetNextWindowSize({m_w, m_h});
    ImGui::SetNextWindowPos({.0f, .0f});
    ImGui::Begin("UI", nullptr,
                 ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                     ImGuiWindowFlags_NoScrollWithMouse);

    // Draw GUI
    Engine::getInstance()->sceneManager.getCurrentScene()->behaviourSystem.onGUI();
    ImGui::End();
    ImGui::Render();
    RenderSystem& sceneRS = Engine::getInstance()->sceneManager.getCurrentScene()->sceneRenderer;

    sceneRS.tryToResize(unsigned(m_w), unsigned(m_h));
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
    return pGame;
}

extern "C" void destroyGameInstance(GPE::AbstractGame* game)
{
    GPE_ASSERT(game != nullptr, "m_editor should be valid since we've just ran the editor.");
    delete game;
    GPE::Engine::getInstance()->sceneManager.removeScenes();
    GPE::Engine::getInstance()->resourceManager.clearAll();
}

void loadTreeResource()
{
    ResourceManagerType& rm = Engine::getInstance()->resourceManager;
    Model::CreateArg     arg;

    SubModel subModel;
    subModel.pShader = rm.get<Shader>("Default");

    subModel.pMaterial = loadMaterialFile("resources\\meshs\\Trank_bark.GPMaterial");
    subModel.pMesh     = loadMeshFile("resources\\meshs\\g1.GPMesh");

    arg.subModels.push_back(subModel);

    subModel.pMaterial = loadMaterialFile("resources\\meshs\\DB2X2_L01.GPMaterial");
    subModel.pMesh     = loadMeshFile("resources\\meshs\\g2.GPMesh");

    arg.subModels.push_back(subModel);

    rm.add<Model::CreateArg>("TreeModel", arg);
}

void loadTree(GameObject& parent, unsigned int number)
{
    const ResourceManagerType& rm = Engine::getInstance()->resourceManager;
    GameObject::CreateArg      forestArg{"Forest"};

    const Model::CreateArg& treeModelArg = *rm.get<Model::CreateArg>("TreeModel");
    GameObject&             forest       = parent.addChild(forestArg);

    // Create trees with random sizes, positions and rotations and add them to the forest
    for (unsigned int i = 0u; i < number; ++i)
    {
        forestArg.name                         = "Tree" + std::to_string(i);
        forestArg.transformArg.position.x      = ranged<float>(-100.f, 100.f);
        forestArg.transformArg.position.z      = ranged<float>(-100.f, 100.f);
        forestArg.transformArg.eulerRotation.y = ranged<float>(TWO_PI);
        forestArg.transformArg.scale           = {ranged<float>(4.f, 8.f)};

        forest.addChild(forestArg).addComponent<Model>(treeModelArg);
    }
}

void loadSkyboxResource()
{
    ResourceManagerType& rm = Engine::getInstance()->resourceManager;
    Model::CreateArg     arg;

    SubModel subModel;
    subModel.pShader               = loadShaderFile("./resources/Skybox.GPShader");
    subModel.pMaterial             = loadMaterialFile("./resources/Skybox.GPMaterial");
    subModel.pMesh                 = loadMeshFile("./resources/meshs/Cube.GPMesh");
    subModel.enableBackFaceCulling = false;

    subModel.pShader->use();
    subModel.pShader->setInt("ourTexture", 0);

    arg.subModels.push_back(subModel);

    rm.add<Model::CreateArg>("SkyboxModel", arg);
}

void loadSkyBox(GameObject& parent)
{
    const ResourceManagerType&  rm = Engine::getInstance()->resourceManager;
    const GameObject::CreateArg skyboxArgGameObject{"Skybox", {{.0f}, {.0f}, {10.f}}};

    parent.addChild(skyboxArgGameObject).addComponent<Model>(*rm.get<Model::CreateArg>("SkyboxModel"));
}

// TODO: Put in-game UI in a module
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

// TODO: Put in-game UI in a module
Game::~Game()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Game::Game()
{
    // ============= UI =============
    // TODO: Put in-game UI in a module
    initDearImGui(GPE::Engine::getInstance()->window.getGLFWWindow());

    // ============ RNG =============
    initSeed();

    // =========== Inputs ===========
    { // Keys
        InputManager& io = Engine::getInstance()->inputManager;

        io.bindInput(GLFW_KEY_W, "forward");
        io.bindInput(GLFW_KEY_S, "backward");
        io.bindInput(GLFW_KEY_A, "left");
        io.bindInput(GLFW_KEY_D, "right");
        io.bindInput(GLFW_KEY_SPACE, "jump");
        io.bindInput(GLFW_KEY_ESCAPE, "exit");
        io.bindInput(GLFW_KEY_LEFT_SHIFT, "sprintStart");
        io.bindInput(GLFW_KEY_LEFT_SHIFT, "sprintEnd");
        io.bindInput(GLFW_MOUSE_BUTTON_LEFT, "RaycastExample");
        io.bindInput(GLFW_MOUSE_BUTTON_LEFT, "shoot");
        io.bindInput(GLFW_MOUSE_BUTTON_RIGHT, "aimBegin");
        io.bindInput(GLFW_MOUSE_BUTTON_RIGHT, "aimEnd");
        io.bindInput(GLFW_KEY_KP_1, "playAmbiantMusic");
        io.bindInput(GLFW_KEY_KP_2, "playAmbiantMusicForce");
        io.bindInput(GLFW_KEY_KP_0, "stopAllMusic");
        // io.bindInput(GLFW_KEY_KP_ADD,       "growUpCollider");
        // io.bindInput(GLFW_KEY_KP_SUBTRACT,  "growDownCollider");

        // Cursor
        io.setCursorTrackingState(false);
    }

    Mesh* planeMesh = &Engine::getInstance()->resourceManager.add<Mesh>(
        "PlaneGround", Mesh::createQuad(0.5f, 0.5f, 100.f, 0, 0, Mesh::Axis::Y));

    //// =========== Scene ===========
    //// world is already initialized

    //// Place content in the scene
    // GPE::GameObject& world = Engine::getInstance()->sceneManager.setCurrentScene("main").getWorld();
    // GameObject *     ground, *player, *testPhysX, *sun, *cube;
    //{
    //    const GameObject::CreateArg cubeArg{"Cube", TransformComponent::CreateArg{{0.f, 10, 0.f}}},
    //        sunArg{"Sun", TransformComponent::CreateArg{{0.f, 200.f, 0.f}}},
    //        playerArg{"Player", TransformComponent::CreateArg{{0.f, 180.f, 0.f}}},
    //        testPhysXArg{"TestphysX", TransformComponent::CreateArg{{0.f, 0.f, 50.f}}},
    //        groundArg{"GroundArg", TransformComponent::CreateArg{{0.f}}};

    //    // A ground, player, PhysX test
    //    cube      = &world.addChild(cubeArg);
    //    sun       = &world.addChild(sunArg);
    //    ground    = &world.addChild(groundArg);
    //    player    = &world.addChild(playerArg);
    //    testPhysX = &world.addChild(testPhysXArg);
    //}

    // world.addComponent<GPG::WorldGenerator>();

    //// Skybox
    // loadSkyboxResource();
    // loadSkyBox(world);

    //// Forest
    // loadTreeResource();
    // loadTree(world, 10u);

    //{ // Camera
    //    Camera::PerspectiveCreateArg camCreateArg{"Player camera"};
    //    Camera&                      mainCam = player->addComponent<Camera>(camCreateArg);
    //    player->pOwnerScene->sceneRenderer.setMainCamera(&mainCam);
    //}

    //{ // Light
    //    sun->getTransform().setTranslation({0, 500, 0});
    //    const DirectionalLight::CreateArg lightArg{
    //        {0.f, -0.5f, 0.5f}, {1.f, 1.f, 1.f, 0.1f}, {1.f, 1.f, 1.f, 1.0f}, {1.f, 1.f, 1.f, 1.f}};
    //    sun->addComponent<DirectionalLight>(lightArg).setShadowActive(true);
    //    // sun->addComponent<Sun>();
    //}

    //// Scripts
    // player->addComponent<GPG::BasePlayer>();

    //{ // cube
    //    cube->getTransform().setScale(Vec3{10, 10, 10});
    //    Model& mod = cube->addComponent<Model>();
    //    mod.addSubModel(SubModel::CreateArg{Engine::getInstance()->resourceManager.get<Shader>("Default"),
    //                                        loadMaterialFile("./resources/meshs/Trank_bark.GPMaterial"),
    //                                        Engine::getInstance()->resourceManager.get<Mesh>("Sphere"), true});
    //}

    //// Physics
    //{ // ground
    //    Mesh* planeMesh = &Engine::getInstance()->resourceManager.add<Mesh>(
    //        "PlaneFround", Mesh::createQuad(1.f, 1.f, 100.f, 0, 0, Mesh::Axis::Y));

    //    ground->getTransform().setScale(Vec3{1000, 1, 1000});
    //    // ground->getTransform().setRotation(Quaternion::fromEuler({PI / 2.f, 0.f, 0.f}));
    //    BoxCollider&     box = ground->addComponent<BoxCollider>();
    //    RigidbodyStatic& rb  = ground->addComponent<RigidbodyStatic>();
    //    Model&           mod = ground->addComponent<Model>();
    //    rb.collider          = &box;
    //    box.isVisible        = true;
    //    box.setDimensions({1000.f, 1.f, 1000.f});
    //    mod.addSubModel(SubModel::CreateArg{Engine::getInstance()->resourceManager.get<Shader>("Default"),
    //                                        loadMaterialFile("resources\\Materials\\GroundMat.GPMaterial"), planeMesh,
    //                                        true});
    //}

    //{ // testPhysX
    //    SphereCollider& sphere = testPhysX->addComponent<SphereCollider>();
    //    sphere.isVisible       = true;
    //    sphere.setRadius(10.f);
    //    testPhysX->addComponent<RigidbodyDynamic>().collider = &sphere;
    //}
}
