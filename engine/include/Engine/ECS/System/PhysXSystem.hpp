/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>
#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyDynamic.hpp>
#include <Engine/ECS/Component/Physics/Rigidbody/RigidbodyStatic.hpp>
#include <GPM/Quaternion.hpp>
#include <PhysX/characterkinematic/PxExtended.h>
#include <PxPhysics.h>
#include <PxScene.h>
#include <PxSceneDesc.h>
#include <Pxfoundation.h>
#include <characterkinematic/PxControllerManager.h>
#include <cooking/PxCooking.h>
#include <foundation/PxErrorCallback.h>
#include <foundation/PxQuat.h>
#include <foundation/PxVec3.h>
#include <pvd/PxPvd.h>
#include <vector>

namespace GPE
{

class TransformComponent;

class UserErrorCallback : public physx::PxErrorCallback
{
public:
    virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) noexcept
    {
        FUNCT_ERROR(message);
    }
};

class PhysXSystem
{
public:
    physx::PxFoundation*              foundation;
    physx::PxPvd*                     pvd;
    physx::PxPhysics*                 physics;
    physx::PxCooking*                 cooking;
    physx::PxScene*                   scene;
    physx::PxControllerManager*       manager;
    std::vector<RigidbodyStatic*>     rigidbodyStatics;
    std::vector<RigidbodyDynamic*>    rigidbodyDynamics;
    std::vector<CharacterController*> characterControllers;

public:
    PhysXSystem();
    ~PhysXSystem();

    void advance(double deltaTime) noexcept;

    void drawDebugScene();

    /**
     * @brief add RigidbodyStatic component to the component list
     * @param rigidbody
     * @return
     */
    inline size_t addComponent(RigidbodyStatic* rigidbody) noexcept;

    /**
     * @brief remove RigidbodyStatic component to the component list
     * @param rigidbody
     * @return
     */
    inline void removeComponent(RigidbodyStatic* rigidbody) noexcept;

    /**
     * @brief add RigidbodyDynamic component to the component list
     * @param rigidbody
     * @return
     */
    inline size_t addComponent(RigidbodyDynamic* rigidbody) noexcept;

    /**
     * @brief remove RigidbodyDynamic component to the component list
     * @param rigidbody
     * @return
     */
    inline void removeComponent(RigidbodyDynamic* rigidbody) noexcept;

    /**
     * @brief add CharacterController component to the component list
     * @param characterController
     * @return
     */
    inline size_t addComponent(CharacterController* characterController) noexcept;

    /**
     * @brief remove CharacterController component to the component list
     * @param characterController
     * @return
     */
    inline void removeComponent(CharacterController* characterController) noexcept;

    static inline GPM::Vec3             PxVec3ToGPMVec3(const physx::PxVec3& vector) noexcept;
    static inline physx::PxVec3         GPMVec3ToPxVec3(const GPM::Vec3& vector) noexcept;
    static inline GPM::Vec3             PxExtendedVec3ToGPMVec3(const physx::PxExtendedVec3& vector) noexcept;
    static inline physx::PxExtendedVec3 GPMVec3ToPxExtendedVec3(const GPM::Vec3& vector) noexcept;
    static inline GPM::Quat             PxQuatToGPMQuat(const physx::PxQuat& quaternion) noexcept;
    static inline physx::PxQuat         GPMQuatToPxQuat(const GPM::Quat& quaternion) noexcept;
    static physx::PxTransform GPETransformComponentToPxTransform(const TransformComponent& transform) noexcept;
    static TransformComponent PxTransformToGPETransformComponent(const physx::PxTransform& transform) noexcept;
};

#include <Engine/ECS/System/PhysXSystem.inl>

} // namespace GPE
