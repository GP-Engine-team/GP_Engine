#include <Engine/ECS/System/PhysXSystem.hpp>

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

GPM::Vec3 PhysXSystem::PxVec3ToGPMVec3(const physx::PxVec3& vector) noexcept
{
    return GPM::Vec3{vector.x, vector.y, vector.z};
}

physx::PxVec3 PhysXSystem::GPMVec3ToPxVec3(const GPM::Vec3& vector) noexcept
{
    return physx::PxVec3{vector.x, vector.y, vector.z};
}

GPM::Quat PhysXSystem::PxQuatToGPMQuat(const physx::PxQuat& quaternion) noexcept
{
    return GPM::Quat{GPM::Vec3{quaternion.x, quaternion.y, quaternion.z}, quaternion.w};
}

physx::PxQuat PhysXSystem::GPMQuatToPxQuat(const GPM::Quat& quaternion) noexcept
{
    return physx::PxQuat{quaternion.x, quaternion.y, quaternion.z, quaternion.s};
}
