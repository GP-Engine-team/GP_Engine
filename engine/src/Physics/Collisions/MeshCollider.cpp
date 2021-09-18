#include <Engine/Engine.hpp>
#include <PhysX/cooking/PxConvexMeshDesc.h>
#include <PhysX/cooking/PxCooking.h>
#include <PhysX/extensions/PxRigidActorExt.h>
#include <PhysX/geometry/PxConvexMesh.h>

#include <Generated/MeshCollider.cpp.h>

using namespace GPE;
using namespace physx;
using namespace std;

MeshCollider::MeshCollider() noexcept : Collider()
{
	static const PxVec3 convexVerts[] = {
		{.0f, 1.f, .0f}, {1.f, .0f, .0f}, {-1.f, .0f, .0f}, {.0f, .0f, 1.f}, {.0f, .0f, -1.f} };

	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = 5u;
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = convexVerts;
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX | PxConvexFlag::eDISABLE_MESH_VALIDATION |
		PxConvexFlag::eFAST_INERTIA_COMPUTATION;

	const Engine* engine = Engine::getInstance();

#ifdef _DEBUG
	// mesh should be validated before cooking without the mesh cleaning
	PX_ASSERT(engine->physXSystem.cooking->validateConvexMesh(convexDesc));
#endif

	convexMesh = engine->physXSystem.cooking->createConvexMesh(
		convexDesc, engine->physXSystem.physics->getPhysicsInsertionCallback());

	material = engine->physXSystem.physics->createMaterial(1, 1, 0);

	shape = engine->physXSystem.physics->createShape(PxConvexMeshGeometry(convexMesh), *material);
}
