#include <Engine/ECS/System/PhysXSystem.hpp>
#include <PxPhysics.h>
#include <PxPhysicsVersion.h>
#include <common/PxTolerancesScale.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxExtensionsAPI.h>
#include <iostream>
#include <pvd/PxPvdTransport.h>

using namespace GPE;
using namespace physx;

static UserErrorCallback  gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;

PhysXSystem::PhysXSystem()
{
    m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if (!m_Foundation)
        FUNCT_ERROR("PxCreateFoundation failed!");

    bool recordMemoryAllocations = true;
#ifdef ENABLE_PVD
    m_Pvd                     = PxCreatePvd(*m_Foundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
#endif
    PxTolerancesScale scale;
    m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, scale, recordMemoryAllocations, m_Pvd);
    if (!m_Physics)
        FUNCT_ERROR("PxCreatePhysics failed!");

    m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, PxCookingParams(scale));
    if (!m_Cooking)
        FUNCT_ERROR("PxCreateCooking failed!");

    if (!PxInitExtensions(*m_Physics, m_Pvd))
        FUNCT_ERROR("PxInitExtensions failed!");

    PxRegisterHeightFields(*m_Physics);

    PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
    sceneDesc.gravity      = PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    PxCpuDispatcher* m_CpuDispatcher = PxDefaultCpuDispatcherCreate(1);
    if (!m_CpuDispatcher)
        FUNCT_ERROR("PxDefaultCpuDispatcherCreate failed!");
    sceneDesc.cpuDispatcher = m_CpuDispatcher;

    m_Scene = m_Physics->createScene(sceneDesc);
}

PhysXSystem::~PhysXSystem()
{
    PxCloseExtensions();

    m_Cooking->release();
    m_Pvd->release();
    m_Physics->release();
    m_Foundation->release();
}

void PhysXSystem::advance(const double& deltaTime) noexcept
{
    m_Scene->simulate(static_cast<PxReal>(deltaTime));
}
