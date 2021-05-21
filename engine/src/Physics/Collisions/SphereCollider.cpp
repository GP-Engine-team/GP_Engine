#include <Engine/Core/Physics/Collisions/SphereCollider.hpp>

#include <Engine/Engine.hpp>
#include <PhysX/PxPhysics.h>

#include <Generated/SphereCollider.rfk.h>

File_GENERATED

    using namespace GPE;
using namespace physx;
using namespace std;

SphereCollider::SphereCollider(GameObject& _owner) noexcept : Collider(), m_radius(1.f)
{
    material = Engine::getInstance()->physXSystem.physics->createMaterial(1.f, 1.f, .0f);

    float     tempValue = 0;
    GPM::Vec3 tempVect  = {abs(_owner.getTransform().getGlobalScale().x), abs(_owner.getTransform().getGlobalScale().y),
                          abs(_owner.getTransform().getGlobalScale().z)};

    tempValue = std::max(std::max(std::max(tempValue, tempVect.x), tempVect.y), tempVect.z);

    shape = Engine::getInstance()->physXSystem.physics->createShape(PxSphereGeometry((tempValue + m_radius) * 0.5),
                                                                    *material, true);
}

void SphereCollider::setRadius(float newRadius) noexcept
{
    m_radius = newRadius;
    updateShape();
}

void SphereCollider::updateShape() noexcept
{
    float     tempValue = 0;
    GPM::Vec3 tempVect  = {abs(owner->getTransform().getGlobalScale().x), abs(owner->getTransform().getGlobalScale().y),
                          abs(owner->getTransform().getGlobalScale().z)};

    tempValue = std::max(std::max(std::max(tempValue, tempVect.x), tempVect.y), tempVect.z);

    shape->setGeometry(PxSphereGeometry((tempValue + m_radius) * 0.5f));
}

SphereCollider::~SphereCollider()
{
    owner->getTransform().OnUpdate -= Function::make(this, "updateShape");
}
