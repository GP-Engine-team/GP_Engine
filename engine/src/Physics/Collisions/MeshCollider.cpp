#include <Engine/ECS/Component/Physics/Collisions/MeshCollider.hpp>
#include <Engine/Engine.hpp>
#include <PhysX/extensions/PxRigidActorExt.h>

// Generated
#include "Generated/MeshCollider.rfk.h"

File_GENERATED

    using namespace GPE;
using namespace physx;
using namespace std;

MeshCollider::MeshCollider(GameObject& owner) noexcept : Collider(owner)
{
    static const PxVec3 convexVerts[] = {PxVec3(0, 1, 0), PxVec3(1, 0, 0), PxVec3(-1, 0, 0), PxVec3(0, 0, 1),
                                         PxVec3(0, 0, -1)};

    PxConvexMeshDesc convexDesc;
    convexDesc.points.count  = 5;
    convexDesc.points.stride = sizeof(PxVec3);
    convexDesc.points.data   = convexVerts;
    convexDesc.flags         = PxConvexFlag::eCOMPUTE_CONVEX | PxConvexFlag::eDISABLE_MESH_VALIDATION |
                       PxConvexFlag::eFAST_INERTIA_COMPUTATION;

#ifdef _DEBUG
    // mesh should be validated before cooking without the mesh cleaning
    bool res = Engine::getInstance()->physXSystem.cooking->validateConvexMesh(convexDesc);
    PX_ASSERT(res);
#endif

    convexMesh = Engine::getInstance()->physXSystem.cooking->createConvexMesh(
        convexDesc, Engine::getInstance()->physXSystem.physics->getPhysicsInsertionCallback());

    material = Engine::getInstance()->physXSystem.physics->createMaterial(1, 1, 0);

    shape = Engine::getInstance()->physXSystem.physics->createShape(PxConvexMeshGeometry(convexMesh), *material);
}