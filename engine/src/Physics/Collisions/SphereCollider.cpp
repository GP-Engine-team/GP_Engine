#include <Engine/ECS/Component/Physics/Collisions/SphereCollider.hpp>
#include <Engine/Engine.hpp>

using namespace GPE;
using namespace physx;

SphereCollider::SphereCollider(GameObject& owner) noexcept : Collider(owner), m_center(0, 0, 0), m_radius(1)
{
    // Engine::getInstance()->physXSystem.addComponent(this);
}
