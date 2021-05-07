#define IMGUI_DEFINE_MATH_OPERATORS
#define GLFW_INCLUDE_NONE

#include <Game.hpp>
#include <myFpsScript.hpp>

#include <Engine/ECS/Component/Camera.hpp>
#include <Engine/ECS/Component/Light/DirectionalLight.hpp>
#include <Engine/ECS/Component/Light/PointLight.hpp>
#include <Engine/ECS/Component/Physics/Collisions/BoxCollider.hpp>
#include <Engine/ECS/Component/Physics/Collisions/SphereCollider.hpp>
#include <Engine/ECS/System/RenderSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Script/FreeFly.hpp>

#include <WorldGenerator.hpp>

#include <GPM/Random.hpp>

// Third_party
#include <glfw/glfw3.h>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace GPE;
using namespace GPM;
using namespace GPM::Random;

void Game::update(double unscaledDeltaTime, double deltaTime)
{
    ++unFixedUpdateFrameCount;
    GPE::Engine::getInstance()->physXSystem.drawDebugScene();
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
    return pGame;
}

extern "C" void destroyGameInstance(GPE::AbstractGame* game)
{
    GPE_ASSERT(game != nullptr, "m_editor should be valid since we've just ran the editor.");
    delete game;
    GPE::Engine::getInstance()->sceneManager.removeScenes();
}

void loadTreeResource()
{
    ResourceManagerType& rm = Engine::getInstance()->resourceManager;
    Model::CreateArg     arg;

    SubModel subModel;
    subModel.pShader   = &rm.add<Shader>("TextureWithLihghts", "./resources/shaders/vTextureWithLight.vs",
                                       "./resources/shaders/fTextureWithLight.fs", LIGHT_BLIN_PHONG);
    subModel.pMaterial = loadMaterialFile("./resources/meshs/Trank_bark.GPMaterial");
    subModel.pMesh     = loadMeshFile("./resources/meshs/g1.GPMesh");

    arg.subModels.push_back(subModel);

    subModel.pMaterial = loadMaterialFile("./resources/meshs/DB2X2_L01.GPMaterial");
    subModel.pMesh     = loadMeshFile("./resources/meshs/g2.GPMesh");

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
    : bSys{GPE::Engine::getInstance()->behaviourSystem},
      world{Engine::getInstance()->sceneManager.loadScene("main").getWorld()}
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
        io.bindInput(GLFW_MOUSE_BUTTON_1, "RaycastExample");
        io.bindInput(GLFW_MOUSE_BUTTON_1, "shoot");

        // io.bindInput(GLFW_KEY_KP_ADD,       "growUpCollider");
        // io.bindInput(GLFW_KEY_KP_SUBTRACT,  "growDownCollider");

        // Cursor
        io.setCursorTrackingState(false);
    }

    // =========== Scene ===========
    // world is already initialized

    // Place content in the scene
    GameObject *ground, *player, *testPhysX;
    {
        const GameObject::CreateArg playerArg{"Player", TransformComponent::CreateArg{Vec3{0.f, 50.f, 0.f}}};
        const GameObject::CreateArg testPhysXArg{"TestphysX", TransformComponent::CreateArg{Vec3{0.f, 0.f, 50.f}}};
        const GameObject::CreateArg groundArg{"GroundArg", TransformComponent::CreateArg{Vec3{0.f}}};

        // A ground, player, PhysX test
        ground    = &world.addChild(groundArg);
        player    = &world.addChild(playerArg);
        testPhysX = &world.addChild(testPhysXArg);
    }

    world.addComponent<GPG::WorldGenerator>();

    // Skybox
    loadSkyboxResource();
    loadSkyBox(world);

    // Forest
    loadTreeResource();
    loadTree(world, 100u);

    { // Camera
        Camera::PerspectiveCreateArg camCreateArg{"Player camera"};
        player->addComponent<Camera>(camCreateArg);
    }

    { // Light
        const PointLight::CreateArg lightArg{
            {1.f, 1.f, 1.f, 0.1f}, {1.f, 1.f, 1.f, 1.0f}, {1.f, 1.f, 1.f, 1.f}, 1.0f, .0014f, 7e-6f};
        player->addComponent<PointLight>(lightArg);
    }

    // Scripts
    player->addComponent<GPG::MyFpsScript>();

    // Physics
    { // ground
        BoxCollider&     box = ground->addComponent<BoxCollider>();
        RigidbodyStatic& rb  = ground->addComponent<RigidbodyStatic>();
        rb.collider          = &box;
        box.isVisible        = true;
        box.setDimensions({1000.f, 10.f, 1000.f});
    }

    { // testPhysX
        SphereCollider& sphere = testPhysX->addComponent<SphereCollider>();
        sphere.isVisible       = true;
        sphere.setRadius(10.f);
        testPhysX->addComponent<RigidbodyDynamic>().collider = &sphere;
    }

    /*
    // FreeFly must be used to compile properly with GPGame.dll, to not be optimized out, for serialization.
    {
        rfk::Entity const* a = rfk::Database::getEntity(GPE::FreeFly::staticGetArchetype().id);
    }

    rm.add<Shader>("TextureOnly", "./resources/shaders/vTextureOnly.vs",
                "./resources/shaders/fTextureOnly.fs", AMBIANTE_COLOR_ONLY);

    Model::CreateArg modelArg;
    modelArg.subModels.emplace_back(SubModel{nullptr, Engine::getInstance()->resourceManager.get<Shader>("TextureOnly"),
                                             Engine::getInstance()->resourceManager.get<Material>("SkyboxMaterial"),
                                             Engine::getInstance()->resourceManager.get<Mesh>("Sphere")});

    testPhysX->addComponent<Model>(modelArg);

    Model::CreateArg modelArg2;
    modelArg2.subModels.emplace_back(SubModel{nullptr,
                                                Engine::getInstance()->resourceManager.get<Shader>("TextureOnly"),
                                                Engine::getInstance()->resourceManager.get<Material>("SkyboxMaterial"),
                                                Engine::getInstance()->resourceManager.get<Mesh>("CubeDebug")});

    ground.addComponent<Model>(modelArg2);
    */

    // =========== Timer ===========
    Log&                        logger = *Log::getInstance();
    const std::function<void()> timer  = [&]() {
        logger.log(stringFormat("FPS (fixedUpdate): %f\n", fixedUpdateFrameCount / FPLogDelay));
        logger.log(stringFormat("FPS (unFixedUpdate): %f\n\n", unFixedUpdateFrameCount / FPLogDelay));
        fixedUpdateFrameCount   = 0;
        unFixedUpdateFrameCount = 0;
    };

    Engine::getInstance()->timeSystem.addScaledTimer(FPLogDelay, timer, true);

    logger.logInitializationEnd("Game");
}
