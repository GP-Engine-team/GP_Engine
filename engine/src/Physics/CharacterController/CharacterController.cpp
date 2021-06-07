#include <Engine/ECS/Component/Physics/CharacterController/CharacterController.hpp>

#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>

#include <PhysX/PxPhysics.h>
#include <PhysX/PxRigidDynamic.h>
#include <PhysX/characterkinematic/PxCapsuleController.h>
#include <PhysX/characterkinematic/PxControllerManager.h>
#include <PxScene.h>

// Generated
#include <Generated/CharacterController.rfk.h>

File_GENERATED

    using namespace GPE;
using namespace physx;

CharacterController::CharacterController(GameObject& owner) noexcept : Component(owner)
{
    PxCapsuleControllerDesc desc;

    desc.height   = m_height;
    desc.material = Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, .0f);

    if (isnan(owner.getTransform().getGlobalPosition().length()))
    {
        desc.position = PhysXSystem::GPMVec3ToPxExtendedVec3(m_center);
    }

    else
    {
        desc.position = PhysXSystem::GPMVec3ToPxExtendedVec3(owner.getTransform().getGlobalPosition() + m_center);
    }

    desc.radius = m_radius;

    if (m_contactOffset <= 0)
    {
        m_contactOffset = 0.1f;
    }
    desc.contactOffset = m_contactOffset;

    controller = Engine::getInstance()->physXSystem.manager->createController(desc);

    updateToSystem();

    // controller->setUserData(&getOwner());
    controller->getActor()->userData = &getOwner();

    owner.getTransform().OnUpdate += Function::make(this, "updateTransform");

    m_parentScale = owner.getTransform().getGlobalScale();
}

void CharacterController::onPostLoad() noexcept
{
    GPE::Component::onPostLoad();

    GPM::Vec3 scale = getOwner().getTransform().getGlobalScale();

    if (isnan(scale.length()))
    {
        scale = {1, 1, 1};
    }

    PxCapsuleControllerDesc desc;

    desc.height   = scale.y + m_height;
    desc.material = Engine::getInstance()->physXSystem.physics->createMaterial(1, 1, 0);

    if (isnan(getOwner().getTransform().getGlobalPosition().length()))
    {
        desc.position = PhysXSystem::GPMVec3ToPxExtendedVec3(m_center);
    }

    else
    {
        desc.position = PhysXSystem::GPMVec3ToPxExtendedVec3(getOwner().getTransform().getGlobalPosition() + m_center);
    }

    desc.radius = std::max(scale.x, scale.z) + m_radius;

    if (m_contactOffset <= 0)
    {
        m_contactOffset = 0.1f;
    }

    desc.contactOffset = m_contactOffset;

    controller = Engine::getInstance()->physXSystem.manager->createController(desc);

    controller->getActor()->userData = &getOwner();

    getOwner().getTransform().OnUpdate += Function::make(this, "updateTransform");

    m_parentScale = getOwner().getTransform().getGlobalScale();
}

void CharacterController::update(double deltaTime) noexcept
{
    if (!m_running)
    {
        m_running = true;
    }

    GameObject*         owner = &getOwner();
    PxControllerFilters filters;
    updateForce();

    groundCheck();

    if (!m_grounded)
    {
        if (m_hasGravity)
        {
            addForce({.0f, -m_gravity * m_mass, .0f});
        }
    }

    else
    {
        if (const float accumulatedTime = float(Engine::getInstance()->timeSystem.getAccumulatedTime());
            (accumulatedTime >= m_startJumpTime + m_jumpTimeDelay))
        {
            m_force.x = m_force.y = m_force.z = .0f;
            m_canJump                         = true;
            m_startJumpTime                   = 0.f;
        }
    }

    if (controller == nullptr)
        return;

    controller->move(PhysXSystem::GPMVec3ToPxVec3(float(deltaTime) * m_displacement), 0.01f, float(deltaTime), filters);
    m_displacement.x = m_displacement.y = m_displacement.z = .0f;
    getOwner().getTransform().setTranslation(PhysXSystem::PxExtendedVec3ToGPMVec3(controller->getPosition()) -
                                             m_center);
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
    m_force += force / m_mass;
}

void CharacterController::updateForce() noexcept
{
    m_displacement += m_force;
}

void CharacterController::groundCheck() noexcept
{
    PxControllerState cctState;
    if (controller)
    {
        controller->getState(cctState);
    }
    m_grounded = (cctState.collisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN) != 0;
}

void CharacterController::startJumpTimer() noexcept
{
    m_canJump       = false;
    m_startJumpTime = static_cast<float>(Engine::getInstance()->timeSystem.getAccumulatedTime());
}

CharacterController::~CharacterController() noexcept
{
    setActive(false);
    getOwner().getTransform().OnUpdate -= Function::make(this, "updateTransform");
}

void CharacterController::updateToSystem() noexcept
{
    if (m_isActivated)
        Engine::getInstance()->physXSystem.addComponent(this);
    else
        Engine::getInstance()->physXSystem.removeComponent(this);
}

void CharacterController::updateShape() noexcept
{
    if (controller)
    {
        Engine::getInstance()->physXSystem.scene->removeActor(*controller->getActor(), false);
        controller->release();
    }

    GPM::Vec3 scale = getOwner().getTransform().getGlobalScale();

    PxCapsuleControllerDesc desc;

    desc.height   = scale.y + m_height;
    desc.material = Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, .0f);
    desc.position = PhysXSystem::GPMVec3ToPxExtendedVec3(getOwner().getTransform().getGlobalPosition() + m_center);
    desc.radius   = std::max(scale.x, scale.z) + m_radius;

    if (m_contactOffset <= 0)
    {
        m_contactOffset = 0.1f;
    }

    desc.contactOffset = m_contactOffset;

    controller = Engine::getInstance()->physXSystem.manager->createController(desc);
}

void CharacterController::setCenter(const GPM::Vec3& newCenter) noexcept
{
    if (newCenter == m_center)
        return;

    m_center = newCenter;
    controller->setPosition(
        PhysXSystem::GPMVec3ToPxExtendedVec3(getOwner().getTransform().getGlobalPosition() + m_center));
}

void CharacterController::setHeight(float newHeight) noexcept
{
    if (newHeight == m_height)
        return;

    m_height = newHeight;
    updateShape();
}

void CharacterController::setRadius(float newRadius) noexcept
{
    if (newRadius == m_radius)
        return;

    m_radius = newRadius;
    updateShape();
}

void CharacterController::setContactOffset(float newOffset) noexcept
{
    if (newOffset == m_contactOffset || newOffset == 0)
        return;

    m_contactOffset = newOffset;

    controller->setContactOffset(m_contactOffset);
}

void CharacterController::updateScale() noexcept
{
    GPM::Vec3 tempScale = getOwner().getTransform().getGlobalScale();
    GPM::Vec3 result    = {abs(m_parentScale.x - tempScale.x), abs(m_parentScale.y - tempScale.y),
                        abs(m_parentScale.z - tempScale.z)};
    if (result.length() < 0.1f)
    {
        return;
    }

    m_parentScale = getOwner().getTransform().getGlobalScale();

    updateShape();
}

void CharacterController::updateTransform()
{
    updatePosition();
    updateScale();
}

bool CharacterController::isMoving()
{
    // epsilone
    return m_displacement.sqrLength() > 0.00001;
}

void CharacterController::updatePosition()
{
    if (!controller || m_running)
        return;

    GPM::Vec3 oldPos;

    oldPos = PhysXSystem::PxExtendedVec3ToGPMVec3(controller->getPosition());

    if (oldPos == getOwner().getTransform().getGlobalPosition())
    {
        return;
    }

    controller->setPosition(
        PhysXSystem::GPMVec3ToPxExtendedVec3(getOwner().getTransform().getGlobalPosition() + m_center));
}
