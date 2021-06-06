/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/TransformComponent.hpp>
#include <foundation/PxErrorCallback.h>
#include <vector>

// Math
#include <GPM/Quaternion.hpp>
#include <PhysX/characterkinematic/PxExtended.h>
#include <foundation/PxQuat.h>

namespace physx
{
class PxFoundation;
class PxPvd;
class PxPhysics;
class PxCooking;
class PxScene;
class PxControllerManager;
} // namespace physx

namespace GPE
{

class CharacterController;
class RigidbodyDynamic;
class RigidbodyStatic;

class UserErrorCallback : public physx::PxErrorCallback
{
public:
    void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) noexcept final;
};

class PhysXSystem
{
public:
    physx::PxFoundation*              foundation;
    physx::PxPhysics*                 physics;
    physx::PxCooking*                 cooking;
    physx::PxScene*                   scene;
    physx::PxControllerManager*       manager;
    physx::PxPvd*                     pvd;
    std::vector<RigidbodyStatic*>     rigidbodyStatics;
    std::vector<RigidbodyDynamic*>    rigidbodyDynamics;
    std::vector<CharacterController*> characterControllers;

public:
    PhysXSystem();
    ~PhysXSystem();

    void advance(double deltaTime) noexcept;
    void updateController(double deltaTime) noexcept;

    void drawDebugScene();

    /**
     * @brief add RigidbodyStatic component to the component list
     * @param rigidbody
     * @return
     */
    size_t addComponent(RigidbodyStatic* rigidbody) noexcept;

    /**
     * @brief remove RigidbodyStatic component to the component list
     * @param rigidbody
     * @return
     */
    void removeComponent(RigidbodyStatic* rigidbody) noexcept;

    /**
     * @brief add RigidbodyDynamic component to the component list
     * @param rigidbody
     * @return
     */
    size_t addComponent(RigidbodyDynamic* rigidbody) noexcept;

    /**
     * @brief remove RigidbodyDynamic component to the component list
     * @param rigidbody
     * @return
     */
    void removeComponent(RigidbodyDynamic* rigidbody) noexcept;

    /**
     * @brief add CharacterController component to the component list
     * @param characterController
     * @return
     */
    size_t addComponent(CharacterController* characterController) noexcept;

    /**
     * @brief remove CharacterController component to the component list
     * @param characterController
     * @return
     */
    void removeComponent(CharacterController* characterController) noexcept;

    static inline GPM::Vec3             PxVec3ToGPMVec3(const physx::PxVec3& vector) noexcept;
    static inline physx::PxVec3         GPMVec3ToPxVec3(const GPM::Vec3& vector) noexcept;
    static inline GPM::Vec3             PxExtendedVec3ToGPMVec3(const physx::PxExtendedVec3& vector) noexcept;
    static inline physx::PxExtendedVec3 GPMVec3ToPxExtendedVec3(const GPM::Vec3& vector) noexcept;
    static inline GPM::Quat             PxQuatToGPMQuat(const physx::PxQuat& quaternion) noexcept;
    static inline physx::PxQuat         GPMQuatToPxQuat(const GPM::Quat& quaternion) noexcept;
    static physx::PxTransform GPETransformComponentToPxTransform(const TransformComponent& transform) noexcept;
};

#include <Engine/ECS/System/PhysXSystem.inl>

} // namespace GPE
