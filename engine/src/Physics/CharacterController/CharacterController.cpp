#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>

#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>

#include <PhysX/characterkinematic/PxCapsuleController.h>
#include <PhysX/characterkinematic/PxControllerManager.h>
#include <PhysX/PxPhysics.h>
#include <PhysX/PxRigidDynamic.h>

// Generated
#include <Generated/CharacterController.rfk.h>

File_GENERATED

    using namespace GPE;
using namespace physx;

CharacterController::CharacterController(GameObject& owner) noexcept
    : Component(owner)
{
    physx::PxCapsuleControllerDesc desc;

    desc.height   = 1.f;
    desc.material = GPE::Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, .0f);
    desc.position = GPE::PhysXSystem::GPMVec3ToPxExtendedVec3(getOwner().getTransform().getGlobalPosition());
    desc.radius   = 1.f;

    controller = GPE::Engine::getInstance()->physXSystem.manager->createController(desc);
    GPE::Engine::getInstance()->physXSystem.addComponent(this);

    // controller->setUserData(&getOwner());
    controller->getActor()->userData = &getOwner();
}

/*
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
*/

void CharacterController::update(double deltaTime) noexcept
{
    physx::PxControllerFilters filters;
    updateForce();

    if (m_jumping == true)
    {
        if (const float accumulatedTime = float(Engine::getInstance()->timeSystem.getAccumulatedTime());
            (accumulatedTime >= m_startJumpTime + m_jumpTimeDelay) && canJump() == true)
        {
            m_jumping       = false;
            m_force.x = m_force.y = m_force.z = .0f;
            m_startJumpTime = 0.f;
        }

        if (m_hasGravity)
        {
            addForce({.0f, -m_gravity, .0f});
        }
    }

    if (controller == nullptr)
        return;

    controller->move(GPE::PhysXSystem::GPMVec3ToPxVec3(m_displacement), 0.1f, float(deltaTime), filters);
    m_displacement.x = m_displacement.y = m_displacement.z = .0f;
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