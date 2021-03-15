#include "Engine/Core/Physics/PhysX.hpp"
#include "PxPhysicsVersion.h"
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"

using namespace GPE;
using namespace physx;

static UserErrorCallback  gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;

PhysX::PhysX()
{

    /* m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
     if (!m_foundation)
         fatalError("PxCreateFoundation failed!");

     bool recordMemoryAllocations = true;

     mPvd                      = PxCreatePvd(*gFoundation);
     PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
     mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

     mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), recordMemoryAllocations, mPvd);
     if (!mPhysics)
         fatalError("PxCreatePhysics failed!");*/
}
