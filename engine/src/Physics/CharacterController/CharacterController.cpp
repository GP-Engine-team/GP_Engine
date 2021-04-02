#include <Engine/Core/Debug/Log.hpp>
#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>
#include <Engine/Engine.hpp>
#include <string>

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

void CharacterController::update(float deltaTime) noexcept
{
    physx::PxControllerFilters filters;
    updateForce();

    if (m_jumping == true)
    {
        if (canJump() == true)
        {
            m_jumping = false;
            m_force   = {0, 0, 0};
        }

        if (m_hasGravity)
        {
            addForce(GPM::Vec3{0, -1.f, 0} * m_gravity);
        }
    }

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

CharacterController::~CharacterController() noexcept
{
    controller->release();
}