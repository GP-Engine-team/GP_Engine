#include "Game.hpp"

#include "Engine/Core/Debug/Assert.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Core/Parsers/ObjParser.hpp"
#include "Engine/Core/Rendering/Renderer/RendererGLFW_GL46.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/ECS/Component/InputComponent.hpp"
#include "Engine/ECS/Component/Light/DirectionalLight.hpp"
#include "Engine/ECS/Component/Light/PointLight.hpp"
#include "Engine/ECS/Component/Model.hpp"
#include "Engine/ECS/Component/TransformComponent.hpp"
#include "Engine/ECS/System/BehaviourSystem.hpp"
#include "Engine/ECS/System/InputManagerGLFW.hpp"
#include "Engine/ECS/System/RenderSystem.hpp"
#include "Engine/ECS/System/TimeSystem.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Material.hpp"
#include "Engine/Resources/Mesh.hpp"
#include "Engine/Resources/ResourcesManagerType.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Texture.hpp"
#include <Engine/ECS/Component/Physics/Collisions/BoxCollider.hpp>
#include <Engine/ECS/Component/Physics/Collisions/SphereCollider.hpp>
#include <myFpsScript.hpp>
//#include "GPM/Random.hpp"

#include <glad/glad.h> //In first
#include <glfw/glfw3.h>

using namespace GPE;
using namespace GPM;

extern "C" AbstractGame* createGameInstance()
{
    // Init glad
    if (!gladLoadGL())
    {
        FUNCT_ERROR("gladLoadGLLoader failed");
        exit(EXIT_FAILURE);
    }

    return new Game();
}

extern "C" void destroyGameInstance(AbstractGame* game)
{
    GPE_ASSERT(game != nullptr, "m_editor should be valid since we've just ran the editor.");
    delete game;
}

void loadTreeResource(ResourceManagerType& resourceManager)
{
    resourceManager.add<Model::CreateArg>(
        "TreeModel", importeSingleModel("./resources/meshs/Tree.obj", resourceManager, Mesh::EBoundingVolume::AABB));
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

    GameObject&       treeContener = parent.addChild<GameObject>(treeGameObject);
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

        GameObject& treeGO = treeContener.addChild<GameObject>(treeGameObject);
        treeGO.addComponent<Model>(treeModelArg);
    }
}

void loadSkyboxResource(ResourceManagerType& resourceManager)
{
    Model::CreateArg& modelArg = resourceManager.add<Model::CreateArg>(
        "SkyboxModel", importeSingleModel("./resources/meshs/Skybox.obj", resourceManager));

    Texture::LoadArg textureArg;
    textureArg.path             = "./resources/textures/Skybox/skb.bmp";
    textureArg.flipTexture      = false;
    textureArg.textureMinFilter = Texture::ETextureMinFilter::LINEAR;
    textureArg.textureMagFilter = Texture::ETextureMagFilter::LINEAR;

    Material::CreateArg matSkybox;
    matSkybox.name     = "Skybox";
    matSkybox.pTexture = &resourceManager.add<Texture>("SkyboxTexture", textureArg);

    modelArg.subModels[0].pMaterial = &resourceManager.add<Material>("SkyboxMaterial", matSkybox);
    modelArg.subModels[0].pShader   = &resourceManager.add<Shader>("SkyboxShader", "./resources/shaders/vSkybox.vs",
                                                                 "./resources/shaders/fSkybox.fs", SKYBOX);
    modelArg.subModels[0].pMesh->setBoundingVolumeType(Mesh::EBoundingVolume::NONE);
    modelArg.subModels[0].enableBackFaceCulling = false;
}

void loadSkyBox(GameObject& parent, ResourceManagerType& resourceManager)
{
    GameObject::CreateArg skyboxArgGameObject{"Skybox", {{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {10.f, 10.f, 10.f}}};

    parent.addChild<GameObject>(skyboxArgGameObject)
        .addComponent<Model>(*resourceManager.get<Model::CreateArg>("SkyboxModel"));
}

Game::Game()
{
    sm.addEmpty("main");
    sm.loadScene("main");

    iManager.bindInput(GLFW_KEY_W, "forward");
    iManager.bindInput(GLFW_KEY_S, "back");
    iManager.bindInput(GLFW_KEY_A, "left");
    iManager.bindInput(GLFW_KEY_D, "right");
    iManager.bindInput(GLFW_KEY_SPACE, "jump");
    iManager.bindInput(GLFW_KEY_LEFT_CONTROL, "down");
    iManager.bindInput(GLFW_KEY_ESCAPE, "exit");
    iManager.bindInput(GLFW_KEY_LEFT_SHIFT, "sprintStart");
    iManager.bindInput(GLFW_KEY_LEFT_SHIFT, "sprintEnd");
    iManager.bindInput(GLFW_KEY_KP_ADD, "growUpCollider");
    iManager.bindInput(GLFW_KEY_KP_SUBTRACT, "growDownCollider");

    GameObject::CreateArg playerArg{"Player", TransformComponent::CreateArg{GPM::Vec3{0.f, 0.f, 0.f}}};
    GameObject::CreateArg testPhysXArg{"TestphysX", TransformComponent::CreateArg{GPM::Vec3{0.f, 0.f, 50.f}}};
    GameObject::CreateArg groundArg{"GroundArg", TransformComponent::CreateArg{GPM::Vec3{0.f, -50.f, 0.f}}};

    Camera::PerspectiveCreateArg camCreateArg;
    camCreateArg.aspect = Camera::computeAspect(900, 600);

    camCreateArg.farVal  = 3000;
    camCreateArg.nearVal = 0.01f;

    PointLight::CreateArg lightArg{
        {1.f, 1.f, 1.f, 0.1f}, {1.f, 0.f, 0.f, 1.0f}, {1.f, 1.f, 1.f, 1.f}, 1.0f, 0.0014f, 0.000007f};

    rm.add<Shader>("TextureOnly", "./resources/shaders/vTextureOnly.vs", "./resources/shaders/fTextureOnly.fs",
                   AMBIANTE_COLOR_ONLY);
    rm.add<Shader>("TextureWithLihghts", "./resources/shaders/vTextureWithLight.vs",
                   "./resources/shaders/fTextureWithLight.fs", LIGHT_BLIN_PHONG);

    GameObject& player    = sm.getCurrentScene()->world.addChild<GameObject>(playerArg);
    GameObject& testPhysX = sm.getCurrentScene()->world.addChild<GameObject>(testPhysXArg);
    GameObject& ground    = sm.getCurrentScene()->world.addChild<GameObject>(groundArg);

    player.addComponent<Camera>(camCreateArg);
    player.addComponent<GPG::MyFpsScript>();
    PointLight& light = player.addComponent<PointLight>(lightArg);
    loadSkyboxResource(rm);

    testPhysX.addComponent<SphereCollider>();
    testPhysX.getComponent<SphereCollider>()->isVisible = true;
    testPhysX.getComponent<SphereCollider>()->setRadius(10.f);
    testPhysX.addComponent<RigidbodyStatic>();
    testPhysX.getComponent<RigidbodyStatic>()->collider = testPhysX.getComponent<SphereCollider>();

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

    loadTreeResource(rm);

    loadSkyBox(sm.getCurrentScene()->world, rm);
    loadTree(sm.getCurrentScene()->world, rm, 1000);

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
