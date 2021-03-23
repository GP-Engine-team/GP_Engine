#include <Engine/ECS/Component/Physics/Collisions/SphereCollider.hpp>
#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Engine.hpp>
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
    foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if (!foundation)
        FUNCT_ERROR("PxCreateFoundation failed!");

    bool recordMemoryAllocations = true;
#ifdef ENABLE_PVD
    pvd                       = PxCreatePvd(*foundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
#endif
    PxTolerancesScale scale;
    physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, scale, recordMemoryAllocations, pvd);
    if (!physics)
        FUNCT_ERROR("PxCreatePhysics failed!");

    cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(scale));
    if (!cooking)
        FUNCT_ERROR("PxCreateCooking failed!");

    if (!PxInitExtensions(*physics, pvd))
        FUNCT_ERROR("PxInitExtensions failed!");

    PxRegisterHeightFields(*physics);

    PxSceneDesc sceneDesc(physics->getTolerancesScale());
    sceneDesc.gravity      = PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    PxCpuDispatcher* m_CpuDispatcher = PxDefaultCpuDispatcherCreate(1);
    if (!m_CpuDispatcher)
        FUNCT_ERROR("PxDefaultCpuDispatcherCreate failed!");
    sceneDesc.cpuDispatcher = m_CpuDispatcher;

    scene = physics->createScene(sceneDesc);
}

PhysXSystem::~PhysXSystem()
{
    PxCloseExtensions();

    cooking->release();
    pvd->release();
    physics->release();
    foundation->release();
}

void PhysXSystem::advance(const double& deltaTime) noexcept
{
    scene->simulate(static_cast<PxReal>(deltaTime));
}

void PhysXSystem::drawDebugScene()
{
    for (unsigned int i = 0; i < rigidbodyStatics.size(); i++)
    {
        if (rigidbodyStatics[i]->collider && rigidbodyStatics[i]->collider->isVisible == true)
        {
            Collider* collider = rigidbodyStatics[i]->collider;
            if (static_cast<SphereCollider*>(collider))
            {
                SphereCollider* sphereCol = static_cast<SphereCollider*>(collider);
                rigidbodyStatics[i]->getOwner().pOwnerScene->sceneRenderer.drawDebugSphere(sphereCol->getCenter(),
                                                                                           sphereCol->getRadius());
            }
        }
    }
}
