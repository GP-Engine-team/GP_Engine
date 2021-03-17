#include <Engine/Core/Physics/PhysX.hpp>
#include <PxPhysicsVersion.h>
#include <common/PxTolerancesScale.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <extensions/PxExtensionsAPI.h>
#include <pvd/PxPvdTransport.h>

using namespace GPE;
using namespace physx;

static UserErrorCallback  gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;

PhysX::PhysX()
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

    m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), recordMemoryAllocations, m_Pvd);
    if (!m_Physics)
        FUNCT_ERROR("PxCreatePhysics failed!");

    m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, PxCookingParams(PxTolerancesScale()));
    if (!m_Cooking)
        FUNCT_ERROR("PxCreateCooking failed!");

    if (!PxInitExtensions(*m_Physics, m_Pvd))
        FUNCT_ERROR("PxInitExtensions failed!");

    PxRegisterHeightFields(*m_Physics);
}

PhysX::~PhysX()
{
    PxCloseExtensions();

    m_Cooking->release();
    m_Pvd->release();
    m_Physics->release();
    m_Foundation->release();
}
