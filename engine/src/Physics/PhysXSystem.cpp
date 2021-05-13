#include <Engine/ECS/System/PhysXSystem.hpp>

#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>
#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyDynamic.hpp>
#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyStatic.hpp>
#include <Engine/ECS/System/RenderSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>

#include <PhysX/gpu/PxGpu.h>
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsVersion.h>
#include <PhysX/PxScene.h>
#include <PhysX/PxSceneDesc.h>
#include <PhysX/Pxfoundation.h>
#include <PhysX/characterkinematic/PxControllerManager.h>
#include <PhysX/characterkinematic/PxController.h>
#include <PhysX/cooking/PxCooking.h>
#include <PhysX/common/PxRenderBuffer.h>
#include <PhysX/common/PxTolerancesScale.h>
#include <PhysX/extensions/PxDefaultAllocator.h>
#include <PhysX/extensions/PxDefaultSimulationFilterShader.h>
#include <PhysX/extensions/PxExtensionsAPI.h>

using namespace GPE;
using namespace physx;

static UserErrorCallback  gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;

void UserErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) noexcept
{
    FUNCT_ERROR(message);
}


PhysXSystem::PhysXSystem()
    : foundation          {nullptr},
      physics             {nullptr},
      cooking             {nullptr},
      scene               {nullptr},
      manager             {nullptr},
      rigidbodyStatics    {},
      rigidbodyDynamics   {},
      characterControllers{}
{
    foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if (!foundation)
        FUNCT_ERROR("PxCreateFoundation failed!");

    bool recordMemoryAllocations = true;
//#ifdef ENABLE_PVD
//    pvd                       = PxCreatePvd(*foundation);
//    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
//    pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
//#endif
    PxTolerancesScale scale;
    physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, scale, recordMemoryAllocations, nullptr);
    if (!physics)
        FUNCT_ERROR("PxCreatePhysics failed!");

    cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(scale));
    if (!cooking)
        FUNCT_ERROR("PxCreateCooking failed!");

    if (!PxInitExtensions(*physics, nullptr))
        FUNCT_ERROR("PxInitExtensions failed!");

    PxRegisterHeightFields(*physics);

    PxSceneDesc sceneDesc(physics->getTolerancesScale());
    sceneDesc.gravity      = PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    PxCpuDispatcher* m_CpuDispatcher = PxDefaultCpuDispatcherCreate(1);
    if (!m_CpuDispatcher)
        FUNCT_ERROR("PxDefaultCpuDispatcherCreate failed!");
    sceneDesc.cpuDispatcher = m_CpuDispatcher;

    PxCudaContextManagerDesc cudaContextManagerDesc;

    sceneDesc.cudaContextManager =
        PxCreateCudaContextManager(*foundation, cudaContextManagerDesc, PxGetProfilerCallback());

    sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;
    sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
    sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
    sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;

    scene = physics->createScene(sceneDesc);

    scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.f);
    scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.f);
    scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 10.f);

    manager = PxCreateControllerManager(*scene);
    manager->setOverlapRecoveryModule(true);
    manager->setPreciseSweeps(false);
}

PhysXSystem::~PhysXSystem()
{
    PxCloseExtensions();

    manager->release();
    scene->release();
    cooking->release();
    physics->release();
    // pvd->release();
    foundation->release();
}

void PhysXSystem::advance(double deltaTime) noexcept
{
    scene->simulate(static_cast<PxReal>(deltaTime));
    scene->fetchResults(true);

    PxU32     nbActiveActors;
    PxActor** activeActors = scene->getActiveActors(nbActiveActors);

    for (PxU32 i = 0; i < nbActiveActors; ++i)
    {
        GameObject* owner = static_cast<GameObject*>(activeActors[i]->userData);
        if (owner)
        {
            RigidbodyDynamic* rigidbody = owner->getComponent<RigidbodyDynamic>();
            if (rigidbody)
            {
                rigidbody->update();
            }
        }
    }
}

void PhysXSystem::drawDebugScene()
{
    const PxRenderBuffer& rb = scene->getRenderBuffer();
    for (PxU32 i = 0; i < rb.getNbLines(); i++)
    {
        const PxDebugLine& line = rb.getLines()[i];
        Engine::getInstance()->sceneManager.getCurrentScene()->sceneRenderer.drawDebugLine(PxVec3ToGPMVec3(line.pos0),
                                                                                           PxVec3ToGPMVec3(line.pos1));
    }
}

size_t PhysXSystem::addComponent(RigidbodyStatic* rigidbody) noexcept
{
    rigidbodyStatics.push_back(rigidbody);
    scene->addActor(*rigidbody->rigidbody);

    return rigidbodyStatics.size();
}

void PhysXSystem::removeComponent(RigidbodyStatic* rigidbody) noexcept
{
    for (std::vector<RigidbodyStatic*>::iterator it = rigidbodyStatics.begin(); it != rigidbodyStatics.end(); it++)
    {
        if ((*it) == rigidbody)
        {
            std::swap<RigidbodyStatic*>(rigidbodyStatics.back(), (*it));
            rigidbodyStatics.pop_back();
            return;
        }
    }
}

size_t PhysXSystem::addComponent(RigidbodyDynamic* rigidbody) noexcept
{
    rigidbodyDynamics.push_back(rigidbody);
    scene->addActor(*rigidbody->rigidbody);

    return rigidbodyDynamics.size();
}

void PhysXSystem::removeComponent(RigidbodyDynamic* rigidbody) noexcept
{
    for (std::vector<RigidbodyDynamic*>::iterator it = rigidbodyDynamics.begin(); it != rigidbodyDynamics.end(); it++)
    {
        if ((*it) == rigidbody)
        {
            std::swap<RigidbodyDynamic*>(rigidbodyDynamics.back(), (*it));
            rigidbodyDynamics.pop_back();
            return;
        }
    }
}

size_t PhysXSystem::addComponent(CharacterController* characterController) noexcept
{
    characterControllers.push_back(characterController);
    scene->addActor(*characterController->controller->getActor());

    return characterControllers.size();
}

void PhysXSystem::removeComponent(CharacterController* characterController) noexcept
{
    for (std::vector<CharacterController*>::iterator it = characterControllers.begin();
         it != characterControllers.end(); it++)
    {
        if ((*it) == characterController)
        {
            std::swap<CharacterController*>(characterControllers.back(), (*it));
            characterControllers.pop_back();
            return;
        }
    }
}
PxTransform PhysXSystem::GPETransformComponentToPxTransform(const TransformComponent& _transform) noexcept
{
    PxTransform transform;
    transform.p = PhysXSystem::GPMVec3ToPxVec3(_transform.getGlobalPosition());
    transform.q = PhysXSystem::GPMQuatToPxQuat(_transform.getGlobalRotation());

    return transform;
}
TransformComponent PhysXSystem::PxTransformToGPETransformComponent(const PxTransform& _transform) noexcept
{
    TransformComponent transform;
    transform.setTranslation(PhysXSystem::PxVec3ToGPMVec3(_transform.p));
    transform.setRotation(PhysXSystem::PxQuatToGPMQuat(_transform.q));

    return transform;
}
