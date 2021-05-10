#include <Engine/Core/Tools/Raycast.hpp>

#include <Engine/ECS/System/PhysXSystem.hpp>
#include <Engine/Engine.hpp>

#include <GPM/Vector3.hpp>

#include <PhysX/PxScene.h>


namespace GPE
{

void Raycast::fire(const GPM::Vector3& origin, const GPM::Vector3& direction, float maxDistance) noexcept
{
    const physx::PxScene* scene = GPE::Engine::getInstance()->physXSystem.scene;
    const physx::PxVec3&  start = GPE::PhysXSystem::GPMVec3ToPxVec3(origin);
    const physx::PxVec3&  dir   = GPE::PhysXSystem::GPMVec3ToPxVec3(direction.normalized());

    scene->raycast(start, dir, maxDistance, hit);
}

}