#include <Engine/Core/Debug/assert.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <algorithm>
#include <gpm/Vector3.hpp>
#include <gpm/Vector4.hpp>

#include <Generated/BaseCharacter.cpp.h>

    using namespace GPE;
using namespace GPG;
using namespace GPM;

BaseCharacter::BaseCharacter(GameObject& owner) : BehaviourComponent(owner)
{
}

void BaseCharacter::onPostLoad()
{
    enableFixedUpdate(true);
    controller = &getOwner().getOrCreateComponent<GPE::CharacterController>();

    BehaviourComponent::onPostLoad();
}

void BaseCharacter::start()
{
    GAME_ASSERT(controller, "null");

    controller->setSpeed(m_baseSpeed);
    controller->setAngularSpeed(HALF_PI / 8.f);
}

void BaseCharacter::rotateYToward(const Vec3& target, float deltaTime)
{
    const Vec3 selfToTarget     = target - transform().getGlobalPosition();
    const Vec2 selfToTargetVec2 = Vec2{selfToTarget.x, selfToTarget.z}.normalized();

    const Vec3 forward                   = transform().getVectorForward();
    const Vec2 perpforwardVec2Normalized = Vec2{forward.z, -forward.x}.normalized();

    const float dot = perpforwardVec2Normalized.dot(selfToTargetVec2);

    // get angle between [-180, 180]
    const float angle = std::acos(dot) - HALF_PI;

    // Get direction between [-1, 1]
    const float direction = (dot < 0) * 2.f - 1;

    float angleTravelByCharacter = direction * controller->getAngularSpeed() * deltaTime;

    // Clamp angleTravelByCharacter with angle
    if (std::abs(angleTravelByCharacter) > std::abs(angle))
        angleTravelByCharacter = angle;

    // Apply rotation on Y
    const Quat rotY{Quat::angleAxis(angleTravelByCharacter, Vec3::up())};

    // Roation in local space
    transform().setRotation(rotY * transform().getSpacialAttribut().rotation);
}

void BaseCharacter::moveToward(const GPM::Vec3& target)
{
    GPM::Vec3 displacement = (target - transform().getGlobalPosition()).normalized();
    displacement.y         = .0f;
    controller->move(displacement);
}

void BaseCharacter::moveAndRotateToward(const GPM::Vec3& target, float deltaTime)
{
    backward();
    rotateYToward(target, deltaTime);
}

void BaseCharacter::jump()
{
    if (controller->getCanJump() == true)
    {
        controller->addForce(GPM::Vec3::up() * m_jumpStrength);
        controller->startJumpTimer();
    }
}

void BaseCharacter::forward()
{
    GPM::Vec3 vec = transform().getVectorForward();
    vec.y         = .0f;
    controller->move(vec * Engine::getInstance()->timeSystem.getDeltaTime());
}

void BaseCharacter::backward()
{
    GPM::Vec3 vec = transform().getVectorForward();
    vec.y         = .0f;
    controller->move(-vec * Engine::getInstance()->timeSystem.getDeltaTime());
}

void BaseCharacter::left()
{
    GPM::Vec3 vec = transform().getVectorRight();
    vec.y         = .0f;
    controller->move(-vec * Engine::getInstance()->timeSystem.getDeltaTime());
}

void BaseCharacter::right()
{
    GPM::Vec3 vec = transform().getVectorRight();
    vec.y         = .0f;
    controller->move(vec * Engine::getInstance()->timeSystem.getDeltaTime());
}

void BaseCharacter::sprintStart()
{
    controller->setSpeed(m_baseSpeed * m_sprintAcceleration);
    m_isSprint = true;
}

void BaseCharacter::sprintEnd()
{
    controller->setSpeed(m_baseSpeed);
    m_isSprint = false;
}

void BaseCharacter::fixedUpdate(double deltaTime)
{
    if (controller->isMoving())
    {
        m_bodyBalancing =
            lerpf(m_bodyBalancing,
                  cosf(Engine::getInstance()->timeSystem.getAccumulatedTime() * (m_isSprint + 1) * TWO_PI), deltaTime);
    }
    else
    {
        m_bodyBalancing = lerpf(m_bodyBalancing, 0.f, deltaTime);
    }

    // controller->update(deltaTime);
}

void BaseCharacter::takeDamage(float damage)
{
    m_currentLife = std::max(0.f, m_currentLife - damage);

    if (!m_isDead && m_currentLife <= std::numeric_limits<float>::epsilon())
    {
        onDeath();
    }
}

bool BaseCharacter::isDead()
{
    return m_isDead;
}

void BaseCharacter::takeLife(float addedlife)
{
    m_currentLife = std::max(m_maxLife, m_currentLife + addedlife);
}

void BaseCharacter::onDeath()
{
    m_isDead = true;
}