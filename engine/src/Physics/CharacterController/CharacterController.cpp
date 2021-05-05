#include <Engine/Core/Debug/Log.hpp>
#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>
#include <Engine/ECS/System/TimeSystem.hpp>
#include <Engine/Engine.hpp>
#include <string>

// Generated
#include "Generated/CharacterController.rfk.h"

File_GENERATED

using namespace GPE;
using namespace physx;

CharacterController::CharacterController(GameObject& owner) noexcept : Component(owner)
{
    physx::PxCapsuleControllerDesc desc;

    desc.height   = 1;
    desc.material = GPE::Engine::getInstance()->physXSystem.physics->createMaterial(1, 1, 0);
    desc.position = GPE::PhysXSystem::GPMVec3ToPxExtendedVec3(getOwner().getTransform().getGlobalPosition());
    desc.radius   = 1;

    controller = GPE::Engine::getInstance()->physXSystem.manager->createController(desc);
    GPE::Engine::getInstance()->physXSystem.addComponent(this);
}

CharacterController::CharacterController() noexcept
{
     physx::PxCapsuleControllerDesc desc;

     desc.height   = 1;
     desc.material = GPE::Engine::getInstance()->physXSystem.physics->createMaterial(1, 1, 0);
     desc.position = GPE::PhysXSystem::GPMVec3ToPxExtendedVec3(GPM::Vec3::zero());
     desc.radius   = 1;

     controller = GPE::Engine::getInstance()->physXSystem.manager->createController(desc);
     GPE::Engine::getInstance()->physXSystem.addComponent(this);
}

void CharacterController::update(double deltaTime) noexcept
{
    physx::PxControllerFilters filters;
    updateForce();

    if (m_jumping == true)
    {
        if (Engine::getInstance()->timeSystem.getAccumulatedTime() >= m_startJumpTime + m_jumpTimeDelay &&
            canJump() == true)
        {
            m_jumping       = false;
            m_force         = {0, 0, 0};
            m_startJumpTime = 0.f;
        }

        if (m_hasGravity)
        {
            addForce({.0f, -m_gravity, .0f});
        }
    }

    if (controller == nullptr)
        return;

    controller->move(GPE::PhysXSystem::GPMVec3ToPxVec3(m_displacement), 0.1f, deltaTime, filters);
    m_displacement = {0, 0, 0};
    getOwner().getTransform().setTranslation(GPE::PhysXSystem::PxExtendedVec3ToGPMVec3(controller->getPosition()));
}

void CharacterController::move(const GPM::Vec3& displacement) noexcept
{
    m_displacement += displacement * m_speed;
}

void CharacterController::move(const GPM::Vec3& displacement, float customSpeed) noexcept
{
    m_displacement += displacement * customSpeed;
}

void CharacterController::addForce(const GPM::Vec3& force) noexcept
{
    m_force += force;
}

void CharacterController::updateForce() noexcept
{
    m_displacement += m_force;
    // m_force *= 0.9f;
}

bool CharacterController::canJump() noexcept
{
    PxControllerState cctState;
    controller->getState(cctState);
    return (cctState.collisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN) != 0;
}

void CharacterController::setJumping(float jumping) noexcept
{
    m_jumping       = jumping;
    m_startJumpTime = static_cast<float>(Engine::getInstance()->timeSystem.getAccumulatedTime());
}

CharacterController::~CharacterController() noexcept
{
    GPE::Engine::getInstance()->physXSystem.removeComponent(this);
    // controller->release();
}

void CharacterController::setActive(bool newState) noexcept
{
    if (m_isActivated == newState)
        return;

    m_isActivated = newState;
    if (m_isActivated)
        GPE::Engine::getInstance()->physXSystem.addComponent(this);
    else
        GPE::Engine::getInstance()->physXSystem.removeComponent(this);
}