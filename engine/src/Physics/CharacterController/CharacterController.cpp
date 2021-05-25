#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>

#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>

#include <PhysX/PxPhysics.h>
#include <PhysX/PxRigidDynamic.h>
#include <PhysX/characterkinematic/PxCapsuleController.h>
#include <PhysX/characterkinematic/PxControllerManager.h>

// Generated
#include <Generated/CharacterController.rfk.h>

File_GENERATED

    using namespace GPE;
using namespace physx;

CharacterController::CharacterController(GameObject& owner) noexcept : Component(owner)
{
    PxCapsuleControllerDesc desc;

    desc.height   = 25.f;
    desc.material = Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, .0f);
    desc.position = PhysXSystem::GPMVec3ToPxExtendedVec3(owner.getTransform().getGlobalPosition());
    desc.radius   = 10.f;

    controller = Engine::getInstance()->physXSystem.manager->createController(desc);

    updateToSystem();

    // controller->setUserData(&getOwner());
    controller->getActor()->userData = &getOwner();
}

CharacterController::CharacterController() noexcept
{
    PxCapsuleControllerDesc desc;

    desc.height   = 1;
    desc.material = Engine::getInstance()->physXSystem.physics->createMaterial(1, 1, 0);
    desc.position = PhysXSystem::GPMVec3ToPxExtendedVec3(GPM::Vec3{0, 10, 0});
    desc.radius   = 1;

    controller = Engine::getInstance()->physXSystem.manager->createController(desc);
}

void CharacterController::update(double deltaTime) noexcept
{
    PxControllerFilters filters;
    updateForce();

    if (m_jumping == true)
    {
        if (const float accumulatedTime = float(Engine::getInstance()->timeSystem.getAccumulatedTime());
            (accumulatedTime >= m_startJumpTime + m_jumpTimeDelay) && canJump() == true)
        {
            m_jumping = false;
            m_force.x = m_force.y = m_force.z = .0f;
            m_startJumpTime                   = 0.f;
        }

        if (m_hasGravity)
        {
            addForce({.0f, -m_gravity, .0f});
        }
    }

    if (controller == nullptr)
        return;

    controller->move(PhysXSystem::GPMVec3ToPxVec3(m_displacement), 0.1f, float(deltaTime), filters);
    m_displacement.x = m_displacement.y = m_displacement.z = .0f;
    getOwner().getTransform().setTranslation(PhysXSystem::PxExtendedVec3ToGPMVec3(controller->getPosition()));
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
    // controller->release();
    setActive(false);
}

void CharacterController::updateToSystem() noexcept
{
    if (m_isActivated)
        Engine::getInstance()->physXSystem.addComponent(this);
    else
        Engine::getInstance()->physXSystem.removeComponent(this);
}