GPM::Vec3 PhysXSystem::PxVec3ToGPMVec3(const physx::PxVec3& vector) noexcept
{
    return GPM::Vec3{vector.x, vector.y, vector.z};
}

physx::PxVec3 PhysXSystem::GPMVec3ToPxVec3(const GPM::Vec3& vector) noexcept
{
    return physx::PxVec3{vector.x, vector.y, vector.z};
}

GPM::Vec3 PhysXSystem::PxExtendedVec3ToGPMVec3(const physx::PxExtendedVec3& vector) noexcept
{
    return GPM::Vec3{GPM::f32(vector.x), GPM::f32(vector.y), GPM::f32(vector.z)};
}

physx::PxExtendedVec3 PhysXSystem::GPMVec3ToPxExtendedVec3(const GPM::Vec3& vector) noexcept
{
    return physx::PxExtendedVec3{double(vector.x), double(vector.y), double(vector.z)};
}

GPM::Quat PhysXSystem::PxQuatToGPMQuat(const physx::PxQuat& quaternion) noexcept
{
    return GPM::Quat{GPM::Vec3{quaternion.x, quaternion.y, quaternion.z}, quaternion.w};
}

physx::PxQuat PhysXSystem::GPMQuatToPxQuat(const GPM::Quat& quaternion) noexcept
{
    return physx::PxQuat{quaternion.x, quaternion.y, quaternion.z, quaternion.w};
}
