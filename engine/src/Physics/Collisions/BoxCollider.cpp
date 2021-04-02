#include <Engine/ECS/Component/Physics/Collisions/BoxCollider.hpp>
#include <Engine/Engine.hpp>

using namespace GPE;
using namespace GPM;
using namespace physx;

BoxCollider::BoxCollider(GameObject& owner) noexcept : Collider(owner), m_center(0, 0, 0), m_dimensions(10, 10, 10)
{
    material = Engine::getInstance()->physXSystem.physics->createMaterial(1, 1, 1);
    shape    = Engine::getInstance()->physXSystem.physics->createShape(
        PxBoxGeometry(m_dimensions.x / 2.f, m_dimensions.y / 2.f, m_dimensions.z / 2.f), *material, true);
}

void BoxCollider::setDimensions(Vec3 newDimensions) noexcept
{
    m_dimensions = newDimensions;
    shape->setGeometry(PxBoxGeometry(m_dimensions.x / 2.f, m_dimensions.y / 2.f, m_dimensions.z / 2.f));
}
